/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
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
    // ~ConfigurationManager();

    bool initialize();
    void processEvents();

    bool loadConfiguration();
    bool saveConfiguration();
    void resetConfiguration();
    bool validateConfiguration() const;

    const char* getFirmwareVersion() const;// TODO: here?

    const char* getPrivateWiFiSSID() const;
    const char* getPrivateWiFiPassword() const;
    const char* getWiFiSSID() const;
    const char* getWiFiPassword() const;
    const char* getMqttServerHost() const;
    int getMqttServerPort() const;
    const char* getOtaPassword() const;
    int getOtaPort() const;

    void setWiFiSSID(const char* ssid);
    void setWiFiPassword(const char* password);
    void setMqttServerHost(const char* host);
    void setMqttServerPort(const int port);
    void setOtaPassword(const char* password);
    void setOtaPort(const int port);

    void setMotorSpeed(const int rpm);
    int getMotorSpeed() const;
    
    int getCurrentBlindsPosition() const;
    int getMaxBlindsPosition() const;
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

#endif // CONFIGURATIONMANAGER_HPP