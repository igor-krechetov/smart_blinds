/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */
#ifndef HOMEASSISTANTINTEGRATION_HPP
#define HOMEASSISTANTINTEGRATION_HPP

#include <Arduino.h>
#include "MqttClient.hpp"
#include "ConfigurationManager.hpp"

class IHomeAssistantListener {
public:
    virtual ~IHomeAssistantListener() = default;

    virtual void onHaConnected() = 0;
    virtual void onHaDisconnected() = 0;

    virtual void onHaRequestOpen() = 0;
    virtual void onHaRequestClose() = 0;
    virtual void onHaRequestSetPosition(const int newPosition) = 0;
};

class HomeAssistantIntegration: public IMqttClientListener {
public:
    HomeAssistantIntegration() = default;
    virtual ~HomeAssistantIntegration() = default;

    void initialize(IHomeAssistantListener *listener);
    void start(const ConfigurationManager& config);

    void processEvents();

    void update

// IMqttClientListener
private:
    void onMqttConnected() override;
    void onMqttDisconnected() override;
    void onTopicUpdated(char *topic, byte *payload, unsigned int length) override;

private:
    String getTopicPrefix() const;
    String getTopicPath(const String &suffix) const;
    void sendAvailability();
    void sendDiscoveryPayload();

    // TODO
private:
    const String cTopicPrefix = "SmartBlinds";

    IHomeAssistantListener* mListener = nullptr;
    MqttClient mClient;
};

#endif // HOMEASSISTANTINTEGRATION_HPP