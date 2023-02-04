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
#ifndef MQTTCLIENT_HPP
#define MQTTCLIENT_HPP

#define MQTT_PUBSUBCLIENT
// NOTE: tested altervative libraries
//  AsyncMqttClient
//      marvinroger/AsyncMqttClient@^0.9.0
//      - stack corruption
//      ottowinter/AsyncMqttClient-esphome@^0.8.6
//      - stack & memory issues
//      - old

#include <Arduino.h>
#ifdef MQTT_PUBSUBCLIENT
    #include <PubSubClient.h>
#else
    #include <AsyncMqttClient.hpp>
    #include <AsyncMqtt_Generic.h>
#endif
#include <ESP8266WiFi.h>

#include <list>
#include <vector>

#include "ConfigurationManager.hpp"

class IMqttClientListener {
public:
    virtual void onMqttConnected() = 0;
    virtual void onMqttDisconnected() = 0;
    virtual void onTopicUpdated(char* topic, char* payload, unsigned int length) = 0;
};

class MqttClient {
public:
    MqttClient();
    ~MqttClient();

    bool initialize(ConfigurationManager& config, IMqttClientListener* listener, const std::list<String>& subscriptions);
    void processEvents();

    bool connect();
    void disconnect();
    bool isConnected();
    bool publishTopic(const String& topic, const String& payload, const bool permanent);
    bool publishTopic(const char* topic, const char* payload, const unsigned int payloadSize, const bool permanent);

    String getTopicPath(const String& suffix) const;

private:
    IMqttClientListener* mListener = nullptr;
    ConfigurationManager* mConfig = nullptr;
#ifdef MQTT_PUBSUBCLIENT
    PubSubClient mClient;
        // BearSSL::WiFiClientSecure mSocket;
#else
        // AsyncMqttClient mClient;
#endif
    WiFiClient mSocket;
    std::list<String> mDefaultSubscriptions;  // TODO: replace with char*

    std::vector<char> mClientID;
    std::vector<char> mSendTopicBuffer;
    std::vector<char> mSendDataBuffer;
    // bool pending
};

#endif  // MQTTCLIENT_HPP
