/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#include "MqttClient.hpp"
#include "LoggingHelper.hpp"

MqttClient::MqttClient()
    : mClient(mSocket)
{}

MqttClient::~MqttClient()
{
    if (true == mClient.connected())
    {
        mClient.disconnect();
    }
}

bool MqttClient::initialize(ConfigurationManager &config, IMqttClientListener *listener, const std::list<String>& subscriptions)
{
    TRACE("MqttClient::initialize");
    bool res = false;

    mListener = listener;

    if (nullptr != mListener) {
        mSocket.setSSLVersion(BR_TLS12, BR_TLS12);
        mSocket.setTrustAnchors(config.getMqttCaCert().get());
        mSocket.setClientRSACert(config.getMqttClientCert().get(), config.getMqttClientKey().get());

        mClient.setBufferSize(700);
        mClient.setServer(config.getMqttServerHost(), config.getMqttServerPort());
        mClient.setCallback([&](char *topic, uint8_t *payload, unsigned int payloadSize) {
            mListener->onTopicUpdated(topic, reinterpret_cast<char*>(payload), payloadSize);
        });

        mDefaultSubscriptions = subscriptions;
        res = connect();
    }

    return res;
}

void MqttClient::processEvents()
{
    if (nullptr != mListener)
    {
        if (true == mClient.connected())
        {
            mClient.loop();
            // TODO: send availability on timeout
        }  
        else
        {
            if (MQTT_CONNECTION_LOST == mClient.state())
            {
                mClient.disconnect();
                mListener->onMqttDisconnected();
            }
        }
    }
}

bool MqttClient::connect()
{
    TRACE("MqttClient::connect");
    bool result = false;

    if (false == mClient.connected())
    {
        String clientId = "SmartCurtains-" + String(ESP.getChipId());

        if (true == mClient.connect(clientId.c_str()))
        {
            NOTICE("MqttClient connected");
            PRINT_FREE_HEAP();

            if (false == mDefaultSubscriptions.empty())
            {
                for (const String &topic : mDefaultSubscriptions)
                {
                    mClient.subscribe(topic.c_str());
                    TRACE_ARGS("MqttClient: subscribed to <%s>", topic.c_str());
                }
            }

            mListener->onMqttConnected();
            result = true;
        }
        else
        {
            ERROR("Failed to connect to MQTT broker");
        }
    }

    return result;
}

bool MqttClient::publishTopic(const String &topic, const String &payload, const bool permanent)
{
    bool res = false;

    if (true == mClient.connected())
    {
        TRACE_ARGS("Publishing topic <%s>", topic.c_str());
        res = mClient.publish(topic.c_str(), payload.c_str(), permanent);

        if (false == res)
        {
            TRACE("ERROR: Failed to publish topic");
        }
    }
    else
    {
        TRACE("Cannot send message - MQTT client is not connected.");
    }

    return res;
}
