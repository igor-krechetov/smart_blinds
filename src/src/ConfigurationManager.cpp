/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#include "ConfigurationManager.hpp"
#include "LoggingHelper.hpp"
#include <LittleFS.h>

#define PATH_CONFIG             "/cfg.json"

#define RETURN_CONFIG_VALUE(_key, _type, _defaultValue)     \
    _type res = _defaultValue;                              \
    if (true == mConfigValues.containsKey(F(_key)))         \
    {                                                       \
        res = mConfigValues[F(_key)].as<_type>();           \
    }                                                       \
    return res;

ConfigurationManager::ConfigurationManager() : mConfigValues(JSON_OBJECT_SIZE(9) + 300) {
}

// ConfigurationManager::~ConfigurationManager() {
// }

bool ConfigurationManager::initialize()
{
    // LittleFS.format();// TODO: debug
    return LittleFS.begin();
}

void ConfigurationManager::processEvents()
{
    // NOTE: do nothing
}

bool ConfigurationManager::loadConfiguration()
{
    bool res = false;
    File configFile = LittleFS.open(PATH_CONFIG, "r");

    if (configFile) {
        size_t size = configFile.size();

        if (size <= 300) {
            if (DeserializationError::Ok == deserializeJson(mConfigValues, configFile)) {
                serializeJson(mConfigValues, Serial);// TODO: debug
                res = true;
            } else {
                ERROR("Failed to parse config json");
            }
        } else {
            ERROR("invalid config: too big");
        }
    } else {
        TRACE("no config file");
    }

    // default values
    if (false == res) {
        setMotorSpeed(120);
        setCurrentBlindsPosition(0);
        setMaxBlindsPosition(100);
    }

    return res;
}

bool ConfigurationManager::saveConfiguration()
{
    bool res = false;
    File configFile = LittleFS.open(PATH_CONFIG, "w");

    if (configFile) {
        res = (serializeJson(mConfigValues, configFile) > 0);
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

bool ConfigurationManager::validateConfiguration() const
{
    // const char* wifiSSID = getWiFiSSID();
    // const char* wifiPassword = getWiFiPassword();
    // const char* mqttHost = getMqttServerHost();
    // const char* otaPassword = getOtaPassword();

    // TODO: for debug
    // TRACE_ARGS("wifiSSID=%s", wifiSSID);
    // TRACE_ARGS("wifiPassword=%s", wifiPassword);
    // TRACE_ARGS("mqttHost=%s", mqttHost);
    // TRACE_ARGS("otaPassword=%s", otaPassword);
    // TRACE_ARGS("getMqttServerPort=%d", getMqttServerPort());
    // TRACE_ARGS("getOtaPort=%d", getOtaPort());

    return ((nullptr != getWiFiSSID()) &&
            (nullptr != getWiFiPassword()) &&
            (nullptr != getMqttServerHost()) &&
            (getMqttServerPort() > 0) &&
            (nullptr != getOtaPassword()) &&
            (getOtaPort() > 0));
}

const char* ConfigurationManager::getFirmwareVersion() const
{
    return "0.1";
}

const char* ConfigurationManager::getPrivateWiFiSSID() const
{
    return "SmartCurtains";
}

const char* ConfigurationManager::getPrivateWiFiPassword() const
{
    return "SM458027";
}

const char* ConfigurationManager::getWiFiSSID() const
{
    RETURN_CONFIG_VALUE("wifi_ssid", const char*, nullptr);
}

const char* ConfigurationManager::getWiFiPassword() const
{
    RETURN_CONFIG_VALUE("wifi_pwd", const char*, nullptr);
}

const char* ConfigurationManager::getMqttServerHost() const
{
    RETURN_CONFIG_VALUE("mqtt_host", const char*, nullptr);
}

int ConfigurationManager::getMqttServerPort() const
{
    RETURN_CONFIG_VALUE("mqtt_port", int, 0);
}

const char* ConfigurationManager::getOtaPassword() const
{
    RETURN_CONFIG_VALUE("ota_pwd", const char*, nullptr);
}

int ConfigurationManager::getOtaPort() const
{
    RETURN_CONFIG_VALUE("ota_port", int, 0);
}

void ConfigurationManager::setWiFiSSID(const char* ssid)
{
    mConfigValues[F("wifi_ssid")] = ssid;
}

void ConfigurationManager::setWiFiPassword(const char* password)
{
    mConfigValues[F("wifi_pwd")] = password;    
}

void ConfigurationManager::setMqttServerHost(const char* host)
{
    mConfigValues[F("mqtt_host")] = host;
}

void ConfigurationManager::setMqttServerPort(const int port)
{
    mConfigValues[F("mqtt_port")] = port;
}

void ConfigurationManager::setOtaPassword(const char* password)
{
    mConfigValues[F("ota_pwd")] = password;
}

void ConfigurationManager::setOtaPort(const int port)
{
    mConfigValues[F("ota_port")] = port;
}

void ConfigurationManager::setMotorSpeed(const int rpm)
{
    mConfigValues[F("motor_spd")] = rpm;
}

int ConfigurationManager::getMotorSpeed() const
{
    RETURN_CONFIG_VALUE("motor_spd", int, 0);
}

int ConfigurationManager::getCurrentBlindsPosition() const
{
    RETURN_CONFIG_VALUE("curtains_last_pos", int, 0);
}

int ConfigurationManager::getMaxBlindsPosition() const
{
    RETURN_CONFIG_VALUE("curtains_max_pos", int, 0);
}

void ConfigurationManager::setCurrentBlindsPosition(const int position)
{
    mConfigValues[F("curtains_last_pos")] = position;
}

void ConfigurationManager::setMaxBlindsPosition(const int maxPosition)
{
    mConfigValues[F("curtains_max_pos")] = maxPosition;
}

const char* ConfigurationManager::getMqttCaCertPath() const
{
    return "/ca.pem";
}

const char* ConfigurationManager::getMqttClientCertPath() const
{
    return "/client.pem";
}

const char* ConfigurationManager::getMqttClientKeyPath() const
{
    return "/client.key";
}

std::shared_ptr<BearSSL::X509List> ConfigurationManager::getMqttCaCert()
{
    if (!mMqttCaCert) {
        mMqttCaCert = loadCertificate(getMqttCaCertPath());
    }

    return mMqttCaCert;
}

std::shared_ptr<BearSSL::X509List> ConfigurationManager::getMqttClientCert()
{
    if (!mMqttClientCert) {
        mMqttClientCert = loadCertificate(getMqttClientCertPath());
    }

    return mMqttClientCert;
}

std::shared_ptr<BearSSL::PrivateKey> ConfigurationManager::getMqttClientKey()
{
    if (!mMqttClientKey) {
        mMqttClientKey = loadPrivateKey(getMqttClientKeyPath());
    }

    return mMqttClientKey;
}

std::shared_ptr<BearSSL::X509List> ConfigurationManager::loadCertificate(const char* path) const
{
    std::shared_ptr<BearSSL::X509List> res;
    File f = LittleFS.open(path, "r");

    if (true == f.isFile()) {
        res = std::make_shared<BearSSL::X509List>(f);
        f.close();
    }

    return res;
}

std::shared_ptr<BearSSL::PrivateKey> ConfigurationManager::loadPrivateKey(const char* path) const
{
    std::shared_ptr<BearSSL::PrivateKey> res;
    File f = LittleFS.open(path, "r");

    if (true == f.isFile()) {
        res = std::make_shared<BearSSL::PrivateKey>(f);
        f.close();
    }

    return res;
}
