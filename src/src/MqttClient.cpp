/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#include "MqttClient.hpp"
#include <ArduinoLog.h>

MqttClient::MqttClient() : mClient(mSocket) {}

MqttClient::~MqttClient()
{
    if (true == mClient.connected())
    {
        mClient.disconnect();
    }
}

bool MqttClient::initialize(IMqttClientListener *listener, const String &brokerHost, const uint16_t brokerPort, const std::list<String>& subscriptions)
{
    bool res = false;

    if (false == brokerHost.isEmpty())
    {
        mListener = listener;

        mClient.setBufferSize(1024);
        mClient.setServer(brokerHost.c_str(), brokerPort);
        mClient.setCallback([&](char *, uint8_t *, unsigned int) {});

        mDefaultSubscriptions = subscriptions;
        connect();
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
    bool result = false;

    if (false == mClient.connected())
    {
        String clientId = "SmartBlinds-" + String(ESP.getChipId());

        // TODO: add auth
        if (true == mClient.connect(clientId.c_str()))
        {
            Log.noticeln("MqttClient connected");

            if (false == mDefaultSubscriptions.empty())
            {
                for (const String &topic : mDefaultSubscriptions)
                {
                    mClient.subscribe(topic.c_str());
                    Log.traceln("MqttClient: subscribed to <%s>", topic.c_str());
                }
            }

            mListener->onMqttConnected();
            // TODO: send availability
            result = true;
        }
        else
        {
            Log.errorln("Failed to connect to MQTT broker");
        }
    }

    return result;
}

void MqttClient::publishTopic(const String &topic, const String &payload, const bool permanent)
{

    if (true == mClient.connected())
    {
        Log.traceln("Publishing topic <%s>", topic.c_str());

        if (false == mClient.publish(topic.c_str(), payload.c_str(), permanent))
        {
            Log.traceln("ERROR: Failed to publish");
        }
    }
    else
    {
        Log.traceln("Cannot send message - MQTT client is not connected.");
    }
}
