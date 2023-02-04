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
#ifndef HOMEASSISTANTINTEGRATION_HPP
#define HOMEASSISTANTINTEGRATION_HPP

#include <Arduino.h>

#include "ConfigurationManager.hpp"
#include "MqttClient.hpp"

enum class CoverState { OPEN, CLOSED, OPENING, CLOSING, STOPPED };

enum class DeviceAvailability { ONLINE, OFFLINE };

#define INVALID_LAST_POSITION (-1)

class IHomeAssistantListener {
public:
    virtual ~IHomeAssistantListener() = default;

    virtual void onMqttConnectionLost() = 0;

    virtual void onHaConnected() = 0;
    virtual void onHaDisconnected() = 0;
    virtual void onHaRegistrationDone() = 0;

    virtual void onHaRequestOpen() = 0;
    virtual void onHaRequestClose() = 0;
    virtual void onHaRequestStop() = 0;
    virtual void onHaRequestSetPosition(const uint32_t newPosition) = 0;
    virtual void onHaLastPositionLoaded() = 0;
};

class HomeAssistantIntegration : public IMqttClientListener {
public:
    HomeAssistantIntegration(ConfigurationManager& config);
    virtual ~HomeAssistantIntegration() = default;

    bool initialize(IHomeAssistantListener* listener);
    bool start();
    void reconnect();
    void disconnect();
    bool isConnected();

    void processEvents();

    void updateAvailability(const DeviceAvailability availability);
    void updateDeviceState(const CoverState state);
    void updateCurrentPosition(const int pos);
    int getLastPosition() const;

    // IMqttClientListener
private:
    void onMqttConnected() override;
    void onMqttDisconnected() override;
    void onTopicUpdated(char* topic, char* payload, unsigned int length) override;

private:
    bool comparePayload(const char* str, char* payload, unsigned int payloadSize) const;
    String getTopicPath(const __FlashStringHelper* suffix) const;

    void sendDiscoveryPayload();

private:
    IHomeAssistantListener* mListener = nullptr;
    ConfigurationManager& mConfig;
    MqttClient mClient;
    int mLastPosition = INVALID_LAST_POSITION;
    char mObjectID[10] = {0};
    char mTopicPrefix[14 + 10 + 1] = {0};  // TOPIC_PREFIX + mObjectID
    // TODO: generate topicConfig in constructor and change type to char[]
    char mTopicConfig[13 + sizeof(mObjectID) + 14 + 1] = {0};  // HA_DISCOVERY_PREFIX + mObjectID + "/cover/%s/config"
    // String haConfig;// ~630 bytes
    // char* haConfig = nullptr;
};

#endif  // HOMEASSISTANTINTEGRATION_HPP