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

class IMqttClientListener {
public:
    virtual void onMqttConnected() = 0;
    virtual void onMqttDisconnected() = 0;
    virtual void onTopicUpdated(char *topic, byte *payload, unsigned int length) = 0;
};

class MqttClient {
public:
    MqttClient();
    ~MqttClient();

    bool initialize(IMqttClientListener* listener, const String& brokerHost, const uint16_t brokerPort, const std::list<String>& subscriptions);
    void processEvents();

    bool connect();
    void publishTopic(const String& topic, const String& payload, const bool permanent);

    String getTopicPath(const String &suffix) const;
    String getTopicPath(const String &suffix) const;

private:
    IMqttClientListener* mListener;
    PubSubClient mClient;
    WiFiClient mSocket;
    std::list<String> mDefaultSubscriptions;
};

#endif // MQTTCLIENT_HPP
