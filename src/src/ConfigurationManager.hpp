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

#ifndef CONFIGURATIONMANAGER_HPP
#define CONFIGURATIONMANAGER_HPP

#include <Arduino.h>
#include <ArduinoJson.h>
#include <BearSSLHelpers.h>

#include <memory>

class ConfigurationManager {
public:
    ConfigurationManager();

    bool initialize();
    void processEvents();

    bool loadConfiguration();
    bool saveConfiguration();
    void resetConfiguration();
    bool validateConfiguration() const;

    const char* getFirmwareVersion() const;
    int getTimezone() const;

    void setInstallationMode(const bool enable);
    bool isInstallationMode() const;

    const char* getPrivateWiFiSSID() const;
    const char* getPrivateWiFiPassword() const;
    const char* getWiFiSSID() const;
    const char* getWiFiPassword() const;
    const char* getMqttServerHost() const;
    int getMqttServerPort() const;
    const char* getMqttUser() const;
    const char* getMqttPassword() const;
    const char* getOtaPassword() const;
    int getOtaPort() const;

    void setWiFiSSID(const char* ssid);
    void setWiFiPassword(const char* password);
    void setMqttServerHost(const char* host);
    void setMqttServerPort(const int port);
    void setMqttUser(const char* user);
    void setMqttPassword(const char* password);
    void setOtaPassword(const char* password);
    void setOtaPort(const int port);

    void setMotorSpeed(const int rpm);
    int getMotorSpeed() const;

    uint32_t getCurrentBlindsPosition() const;
    uint32_t getMaxBlindsPosition() const;
    void setCurrentBlindsPosition(const int position);
    void setMaxBlindsPosition(const int maxPosition);

    const char* getMqttCaCertPath() const;
    const char* getMqttClientCertPath() const;
    const char* getMqttClientKeyPath() const;

    std::shared_ptr<BearSSL::X509List> getMqttCaCert();
    std::shared_ptr<BearSSL::X509List> getMqttClientCert();
    std::shared_ptr<BearSSL::PrivateKey> getMqttClientKey();

private:
    std::shared_ptr<BearSSL::X509List> loadCertificate(const char* path) const;
    std::shared_ptr<BearSSL::PrivateKey> loadPrivateKey(const char* path) const;

private:
    DynamicJsonDocument mConfigValues;

    // NOTE: everything should be in PEM format
    std::shared_ptr<BearSSL::X509List> mMqttCaCert;
    std::shared_ptr<BearSSL::X509List> mMqttClientCert;
    std::shared_ptr<BearSSL::PrivateKey> mMqttClientKey;
};

#endif  // CONFIGURATIONMANAGER_HPP