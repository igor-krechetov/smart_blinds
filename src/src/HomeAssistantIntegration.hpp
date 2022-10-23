/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */
#ifndef HOMEASSISTANTINTEGRATION_HPP
#define HOMEASSISTANTINTEGRATION_HPP

#include <Arduino.h>
#include "MqttClient.hpp"
#include "ConfigurationManager.hpp"

enum class CoverState {
    OPEN,
    CLOSED,
    OPENING,
    CLOSING,
    STOPPED
};

enum class DeviceAvailability {
    ONLINE,
    OFFLINE
};

class IHomeAssistantListener {
public:
    virtual ~IHomeAssistantListener() = default;

    virtual void onHaConnected() = 0;
    virtual void onHaDisconnected() = 0;
    virtual void onHaRegistrationDone() = 0;

    virtual void onHaRequestOpen() = 0;
    virtual void onHaRequestClose() = 0;
    virtual void onHaRequestStop() = 0;
    virtual void onHaRequestSetPosition(const uint32_t newPosition) = 0;
};

class HomeAssistantIntegration: public IMqttClientListener {
public:
    HomeAssistantIntegration() = default;
    virtual ~HomeAssistantIntegration() = default;

    void initialize(IHomeAssistantListener *listener);
    void start(const ConfigurationManager& config);

    void processEvents();

    void updateAvailability(const DeviceAvailability availability);
    void updateDeviceState(const CoverState state);
    void updateCurrentPosition(const int pos);
    int getLastPosition() const;

// IMqttClientListener
private:
    void onMqttConnected() override;
    void onMqttDisconnected() override;
    void onTopicUpdated(char *topic, char *payload, unsigned int length) override;

private:
    bool comparePayload(const char* str, char* payload, unsigned int payloadSize) const;
    String getTopicPath(const __FlashStringHelper* suffix) const;
    
    void sendDiscoveryPayload();

    // TODO
private:
    IHomeAssistantListener* mListener = nullptr;
    MqttClient mClient;
    int mLastPosition = -1;
    char mObjectID[10] = {0};
    char mTopicPrefix[14 + 10 + 1] = {0};// TOPIC_PREFIX + mObjectID
};

#endif // HOMEASSISTANTINTEGRATION_HPP