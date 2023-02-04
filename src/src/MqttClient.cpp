/*
Copyright (C) 2023 Ihor Krechetov

This file is part of SmartCurtains program.

SmartCurtains program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SmartCurtains program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "MqttClient.hpp"

#include "LoggingHelper.hpp"

MqttClient::MqttClient()
    :
#ifdef MQTT_PUBSUBCLIENT
    mClient(mSocket) ,
#endif
    mClientID(25, 0) {
}

MqttClient::~MqttClient() {
    if (true == mClient.connected()) {
        mClient.disconnect();
    }
}

bool MqttClient::initialize(ConfigurationManager &config,
                            IMqttClientListener *listener,
                            const std::list<String> &subscriptions) {
    TRACE("MqttClient::initialize");
    bool res = false;

    mListener = listener;
    mConfig = &config;

    if (nullptr != mListener) {
        PRINT_FREE_HEAP();

        mClient.setServer(config.getMqttServerHost(), config.getMqttServerPort());

#ifdef MQTT_PUBSUBCLIENT
        // NOTE: SSL works, but there is not enough free RAM. So changed to user/password
        // mSocket.setSSLVersion(BR_TLS12, BR_TLS12);
        // mSocket.setTrustAnchors(config.getMqttCaCert().get());
        // mSocket.setClientRSACert(config.getMqttClientCert().get(), config.getMqttClientKey().get());

        // NOTE: it's critical to set buffer size big enough to always be albe to read received data
        // mClient.setBufferSize(1000);// src: 700
        snprintf(mClientID.data(), mClientID.size(), "SmartCurtains-%" PRIu32, ESP.getChipId());

        // int r = mSocket.connect(config.getMqttServerHost(), config.getMqttServerPort());

        mSocket.setTimeout(3000);
        mClient.setSocketTimeout(3);
        mClient.setCallback([&](char *topic, uint8_t *payload, unsigned int payloadSize) {
            TRACE("---------- MQTT CB received");
            PRINT_FREE_HEAP();
            mListener->onTopicUpdated(topic, reinterpret_cast<char *>(payload), payloadSize);
        });
#else
        mClient.setClientId(mClientID.data());
        mClient.setCleanSession(true);
        mClient.setMaxTopicLength(1500);  // TODO: debug
        mClient.setCredentials(mConfig->getMqttUser(), mConfig->getMqttPassword());

        mClient.onConnect([&](bool sessionPresent) {
            NOTICE("MqttClient connected");
            PRINT_FREE_HEAP();

            if (false == mDefaultSubscriptions.empty()) {
                for (const String &topic : mDefaultSubscriptions) {
                    mClient.subscribe(topic.c_str(), 0);
                    TRACE_ARGS("MqttClient: subscribed to <%s>", topic.c_str());
                }
            }

            mListener->onMqttConnected();
        });
        mClient.onDisconnect([&](AsyncMqttClientDisconnectReason reason) {
            NOTICE("MqttClient disconnected");
            mListener->onMqttDisconnected();
        });
        mClient.onMessage([&](char *topic,
                              char *payload,
                              AsyncMqttClientMessageProperties properties,
                              size_t len,
                              size_t index,
                              size_t total) { mListener->onTopicUpdated(topic, payload, len); });

        mClient.onPublish([&](uint16_t packetId) { TRACE("---- OnPublishUserCallback"); });
#endif

        mDefaultSubscriptions = subscriptions;

        PRINT_FREE_HEAP();
        res = connect();
        PRINT_FREE_HEAP();
    }

    return res;
}

void MqttClient::processEvents() {
#ifdef MQTT_PUBSUBCLIENT
    if (nullptr != mListener) {
        if (true == mClient.connected()) {
            mClient.loop();
            // TODO: send availability on timeout
        } else {
            if (MQTT_CONNECTION_LOST == mClient.state()) {
                mClient.disconnect();
                mListener->onMqttDisconnected();
            }
        }
    }
#endif
}

bool MqttClient::connect() {
    TRACE("MqttClient::connect");
    bool result = false;

    if (false == mClient.connected()) {
#ifdef MQTT_PUBSUBCLIENT
        if (true == mClient.connect(mClientID.data(), mConfig->getMqttUser(), mConfig->getMqttPassword())) {
            NOTICE("MqttClient connected");
            PRINT_FREE_HEAP();

            if (false == mDefaultSubscriptions.empty()) {
                for (const String &topic : mDefaultSubscriptions) {
                    mClient.subscribe(topic.c_str());
                    // (https://github.com/knolleary/pubsubclient/issues/184#issuecomment-314506007) delay(10); ESP.wdtFeed();//
                }
            }

            mListener->onMqttConnected();
            result = true;
        } else {
            ERROR("Failed to connect to MQTT broker");
        }
#else
        mClient.connect();
        result = true;
#endif
    }

    return result;
}

void MqttClient::disconnect() {
    TRACE("MqttClient::disconnect");
    mClient.disconnect();
}

bool MqttClient::isConnected() {
    return mClient.connected();
}

bool MqttClient::publishTopic(const String &topic, const String &payload, const bool permanent) {
    return publishTopic(topic.c_str(), payload.c_str(), payload.length(), permanent);
}

bool MqttClient::publishTopic(const char *topic, const char *payload, const unsigned int payloadSize, const bool permanent) {
    bool res = false;

    if (true == mClient.connected()) {
        TRACE_ARGS("Publishing topic <%s>, payloadLen=%u", topic, payloadSize);

#ifdef MQTT_PUBSUBCLIENT
        if (true == mClient.beginPublish(topic, payloadSize, permanent)) {
            const unsigned int tx = mClient.write(reinterpret_cast<const uint8_t *>(payload), payloadSize);
            mClient.endPublish();
            TRACE_ARGS("TX: %u", tx);
            res = (tx == payloadSize);
        }
        // res = mClient.publish(topic, reinterpret_cast<const uint8_t*>(payload), payloadSize, permanent); // PubSubClient
#else
        res = mClient.publish(topic, /*qos*/ 0, permanent, payload, payloadSize);  // AsyncMqttClient
#endif

        if (false == res) {
            TRACE("ERROR: Failed to publish topic");
        }
    } else {
        TRACE("Cannot send message - MQTT client is not connected.");
    }

    return res;
}