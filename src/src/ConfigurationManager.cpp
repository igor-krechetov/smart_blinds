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

#include "ConfigurationManager.hpp"

#include <LittleFS.h>

#include "LoggingHelper.hpp"

#define PATH_CONFIG "/cfg.json"
#define EXPECTED_CONFIG_SIZE (500)

#define RETURN_CONFIG_VALUE(_key, _type, _defaultValue) \
    _type res = _defaultValue;                          \
    if (true == mConfigValues.containsKey(F(_key))) {   \
        res = mConfigValues[F(_key)].as<_type>();       \
    }                                                   \
    return res;

// TODO: is size enough?
ConfigurationManager::ConfigurationManager() : mConfigValues(JSON_OBJECT_SIZE(9) + EXPECTED_CONFIG_SIZE) {}

bool ConfigurationManager::initialize() {
    return LittleFS.begin();
    // NOTE: for debug when hard config reset is needed
    // LittleFS.begin();
    // resetConfiguration();
    // return true;
}

void ConfigurationManager::processEvents() {
    // NOTE: do nothing
}

bool ConfigurationManager::loadConfiguration() {
    bool res = false;
    File configFile = LittleFS.open(PATH_CONFIG, "r");

    if (configFile) {
        size_t size = configFile.size();

        if (size <= EXPECTED_CONFIG_SIZE) {
            if (DeserializationError::Ok == deserializeJson(mConfigValues, configFile)) {
                res = true;
            } else {
                ERROR("Failed to parse config json");
            }
        } else {
            ERROR_ARGS("invalid config: too big (%d)", static_cast<int>(size));
        }
    } else {
        TRACE("no config file");
    }

    // default values
    if (false == res) {
        setMotorSpeed(5);
        setCurrentBlindsPosition(0);
        setMaxBlindsPosition(100);
    }

    return res;
}

bool ConfigurationManager::saveConfiguration() {
    TRACE("Saving configuration");

    bool res = false;
    File configFile = LittleFS.open(PATH_CONFIG, "w");

    if (configFile) {
        res = (serializeJson(mConfigValues, configFile) > 0);
        configFile.flush();
        configFile.close();
    } else {
        ERROR("Failed to open config file for writing");
    }

    return res;
}

void ConfigurationManager::resetConfiguration() {
    TRACE("ConfigurationManager::resetConfiguration");
    LittleFS.format();

    mConfigValues.clear();
    mMqttCaCert.reset();
    mMqttClientCert.reset();
    mMqttClientKey.reset();
}

bool ConfigurationManager::validateConfiguration() const {
    return ((nullptr != getWiFiSSID()) && (nullptr != getWiFiPassword()) && (nullptr != getMqttServerHost()) &&
            (getMqttServerPort() > 0) && (nullptr != getOtaPassword()) && (getOtaPort() > 0));
}

const char* ConfigurationManager::getFirmwareVersion() const {
    return "1.0";
}

int ConfigurationManager::getTimezone() const {
    // TODO: maybe need to make this configurable from UI
    return 9;  // KR
}

void ConfigurationManager::setInstallationMode(const bool enable) {
    mConfigValues[F("installation")] = enable;
}

bool ConfigurationManager::isInstallationMode() const {
    RETURN_CONFIG_VALUE("installation", bool, true);
}

const char* ConfigurationManager::getPrivateWiFiSSID() const {
    return "SmartCurtains";
}

const char* ConfigurationManager::getPrivateWiFiPassword() const {
    return "SM458027";
}

const char* ConfigurationManager::getWiFiSSID() const {
    RETURN_CONFIG_VALUE("wifi_ssid", const char*, nullptr);
}

const char* ConfigurationManager::getWiFiPassword() const {
    RETURN_CONFIG_VALUE("wifi_pwd", const char*, nullptr);
}

const char* ConfigurationManager::getMqttServerHost() const {
    RETURN_CONFIG_VALUE("mqtt_host", const char*, nullptr);
}

int ConfigurationManager::getMqttServerPort() const {
    RETURN_CONFIG_VALUE("mqtt_port", int, 0);
}

const char* ConfigurationManager::getMqttUser() const {
    RETURN_CONFIG_VALUE("mqtt_user", const char*, nullptr);
}

const char* ConfigurationManager::getMqttPassword() const {
    RETURN_CONFIG_VALUE("mqtt_pwd", const char*, nullptr);
}

const char* ConfigurationManager::getOtaPassword() const {
    RETURN_CONFIG_VALUE("ota_pwd", const char*, nullptr);
}

int ConfigurationManager::getOtaPort() const {
    RETURN_CONFIG_VALUE("ota_port", int, 0);
}

// NOTE: const_cast<char*> is needed to force ArduinoJson to make a copy of a string
//       otherwise it just stores the const char* pointer which leads to undefined behaviour

void ConfigurationManager::setWiFiSSID(const char* ssid) {
    mConfigValues[F("wifi_ssid")] = const_cast<char*>(ssid);
}

void ConfigurationManager::setWiFiPassword(const char* password) {
    mConfigValues[F("wifi_pwd")] = const_cast<char*>(password);
}

void ConfigurationManager::setMqttServerHost(const char* host) {
    mConfigValues[F("mqtt_host")] = const_cast<char*>(host);
}

void ConfigurationManager::setMqttServerPort(const int port) {
    mConfigValues[F("mqtt_port")] = port;
}

void ConfigurationManager::setMqttUser(const char* user) {
    mConfigValues[F("mqtt_user")] = const_cast<char*>(user);
}

void ConfigurationManager::setMqttPassword(const char* password) {
    mConfigValues[F("mqtt_pwd")] = const_cast<char*>(password);
}

void ConfigurationManager::setOtaPassword(const char* password) {
    mConfigValues[F("ota_pwd")] = const_cast<char*>(password);
}

void ConfigurationManager::setOtaPort(const int port) {
    mConfigValues[F("ota_port")] = port;
}

void ConfigurationManager::setMotorSpeed(const int rpm) {
    mConfigValues[F("motor_spd")] = rpm;
}

int ConfigurationManager::getMotorSpeed() const {
    // TODO: implement speed configuration from WebUI
    // RETURN_CONFIG_VALUE("motor_spd", int, 0);
    return 5;
}

uint32_t ConfigurationManager::getCurrentBlindsPosition() const {
    RETURN_CONFIG_VALUE("curtains_last_pos", uint32_t, 0);
}

uint32_t ConfigurationManager::getMaxBlindsPosition() const {
    RETURN_CONFIG_VALUE("curtains_max_pos", uint32_t, 0);
}

void ConfigurationManager::setCurrentBlindsPosition(const int position) {
    mConfigValues[F("curtains_last_pos")] = position;
}

void ConfigurationManager::setMaxBlindsPosition(const int maxPosition) {
    mConfigValues[F("curtains_max_pos")] = maxPosition;
}

const char* ConfigurationManager::getMqttCaCertPath() const {
    return "/ca.pem";
}

const char* ConfigurationManager::getMqttClientCertPath() const {
    return "/client.pem";
}

const char* ConfigurationManager::getMqttClientKeyPath() const {
    return "/client.key";
}

std::shared_ptr<BearSSL::X509List> ConfigurationManager::getMqttCaCert() {
    if (!mMqttCaCert) {
        mMqttCaCert = loadCertificate(getMqttCaCertPath());
    }

    return mMqttCaCert;
}

std::shared_ptr<BearSSL::X509List> ConfigurationManager::getMqttClientCert() {
    if (!mMqttClientCert) {
        mMqttClientCert = loadCertificate(getMqttClientCertPath());
    }

    return mMqttClientCert;
}

std::shared_ptr<BearSSL::PrivateKey> ConfigurationManager::getMqttClientKey() {
    if (!mMqttClientKey) {
        mMqttClientKey = loadPrivateKey(getMqttClientKeyPath());
    }

    return mMqttClientKey;
}

std::shared_ptr<BearSSL::X509List> ConfigurationManager::loadCertificate(const char* path) const {
    std::shared_ptr<BearSSL::X509List> res;
    File f = LittleFS.open(path, "r");

    if (true == f.isFile()) {
        res = std::make_shared<BearSSL::X509List>(f);
        f.close();
    }

    return res;
}

std::shared_ptr<BearSSL::PrivateKey> ConfigurationManager::loadPrivateKey(const char* path) const {
    std::shared_ptr<BearSSL::PrivateKey> res;
    File f = LittleFS.open(path, "r");

    if (true == f.isFile()) {
        res = std::make_shared<BearSSL::PrivateKey>(f);
        f.close();
    }

    return res;
}
