/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#include "MqttClient.hpp"
#include "LoggingHelper.hpp"

// NOTE: everything should be in PEM format

static const char caCert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
...
-----END CERTIFICATE-----
")EOF";

static const char clientCert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
...
-----END CERTIFICATE-----
)EOF";

static const char clientKey[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
...
-----END RSA PRIVATE KEY-----
)KEY";

MqttClient::MqttClient()
    : mClient(mSocket)
    , mCaCert(caCert)
    , mClientCrt(clientCert)
    , mClientKey(clientKey)
{}

MqttClient::~MqttClient()
{
    if (true == mClient.connected())
    {
        mClient.disconnect();
    }
}

bool MqttClient::initialize(IMqttClientListener *listener, const String &brokerHost, const uint16_t brokerPort, const std::list<String>& subscriptions)
{
    TRACE_ARGS("MqttClient::initialize: host=%s, port=%d", brokerHost.c_str(), (int)brokerPort);
    bool res = false;

    if (false == brokerHost.isEmpty())
    {
        mListener = listener;

        if (nullptr != mListener) {
            mSocket.setSSLVersion(BR_TLS12, BR_TLS12);
            mSocket.setTrustAnchors(&mCaCert);
            mSocket.setClientRSACert(&mClientCrt, &mClientKey);

            mClient.setBufferSize(2048);
            mClient.setServer(brokerHost.c_str(), brokerPort);
            mClient.setCallback([&](char *topic, uint8_t *payload, unsigned int payloadSize) {
                mListener->onTopicUpdated(topic, reinterpret_cast<char*>(payload), payloadSize);
            });

            mDefaultSubscriptions = subscriptions;
            res = connect();
        }
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
            //TODO: send availability on timeout
        }  
        else
        {
            // TODO: add timeout
            // connect();
            // TODO: reconnect
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

        // TODO: move time sync to a different place
        TRACE("trying to sync time...");
        const char *ntp1 = "time.windows.com";
        const char *ntp2 = "pool.ntp.org";
        time_t now = 0;
        
        configTime(2 * 3600, 1, ntp1, ntp2);
        while(now < 2 * 3600)
        {
            Serial.print(".");
            delay(500);
            now = time(nullptr);
        }
        TRACE("Time sync DONE");

        // TODO: add auth
        if (true == mClient.connect(clientId.c_str()))
        {
            NOTICE("MqttClient connected");

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
