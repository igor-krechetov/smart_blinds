/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */
#ifndef MQTTCLIENT_HPP
#define MQTTCLIENT_HPP

#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <list>
#include "ConfigurationManager.hpp"

class IMqttClientListener {
public:
    virtual void onMqttConnected() = 0;
    virtual void onMqttDisconnected() = 0;
    virtual void onTopicUpdated(char *topic, char *payload, unsigned int length) = 0;
};

class MqttClient {
public:
    MqttClient();
    ~MqttClient();

    bool initialize(ConfigurationManager &config, IMqttClientListener* listener, const std::list<String>& subscriptions);
    void processEvents();

    bool connect();
    bool publishTopic(const String& topic, const String& payload, const bool permanent);

    String getTopicPath(const String &suffix) const;

private:
    IMqttClientListener* mListener;
    PubSubClient mClient;
    BearSSL::WiFiClientSecure mSocket;
    std::list<String> mDefaultSubscriptions;// TODO: replace with char*
};

#endif // MQTTCLIENT_HPP
