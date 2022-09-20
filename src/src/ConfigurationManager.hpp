/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#ifndef CONFIGURATIONMANAGER_HPP
#define CONFIGURATIONMANAGER_HPP

#include <Arduino.h>
#include <functional>

class IConfigurationListener {
public:
    virtual void onConfigured() = 0;
};

class ConfigurationManager {
public:
    // ConfigurationManager();
    void initialize(IConfigurationListener* listener);
    void processEvents();

    bool loadConfiguration();
    bool saveConfiguration();

    void startConfigurationPortal();
    void stopConfigurationPortal();

    String getFirmwareVersion() const;// TODO: here?

    String getWiFiSSID() const;
    String getWiFiPassword() const;
    String getMqttServerHost() const;
    int getMqttServerPort() const;

    void setWiFiSSID(const String& ssid);
    void setWiFiPassword(const String& password);
    void setMqttServerHost(const String& host);
    void setMqttServerPort(const int port);

    void setMotorSpeed(const int rpm);
    int getMotorSpeed() const;
    
    int getCurrentBlindsPosition() const;
    int getMaxBlindsPosition() const;
    void setCurrentBlindsPosition(const int position) const;
    void setMaxBlindsPosition(const int maxPosition) const;

private:
    IConfigurationListener* mListener = nullptr;
};

#endif // CONFIGURATIONMANAGER_HPP