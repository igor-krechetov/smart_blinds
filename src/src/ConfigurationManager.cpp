/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#include "ConfigurationManager.hpp"
#include <ArduinoLog.h>

void ConfigurationManager::initialize(IConfigurationListener *listener)
{
    mListener = listener;
}

void ConfigurationManager::processEvents()
{
    //TODO: impl
}

bool ConfigurationManager::loadConfiguration()
{
    // TODO: implement

    return true;
}

bool ConfigurationManager::saveConfiguration()
{
    // TODO: implement
    return true;
}

void ConfigurationManager::startConfigurationPortal()
{
    // TODO: implement
}

void ConfigurationManager::stopConfigurationPortal()
{
    // TODO: implement
}

String ConfigurationManager::getFirmwareVersion() const
{
    // TODO: implement
    return "0.1";
}

String ConfigurationManager::getWiFiSSID() const
{
    return "";
}

String ConfigurationManager::getWiFiPassword() const
{
    return "";
}

String ConfigurationManager::getMqttServerHost() const
{
    return "";
}

int ConfigurationManager::getMqttServerPort() const
{
    return 0; // TODO: check
}

void ConfigurationManager::setWiFiSSID(const String &ssid)
{
    //TODO: impl
}

void ConfigurationManager::setWiFiPassword(const String &password)
{
    //TODO: impl
}

void ConfigurationManager::setMqttServerHost(const String &host)
{
    //TODO: impl
}

void ConfigurationManager::setMqttServerPort(const int port)
{
    //TODO: impl
}

void ConfigurationManager::setMotorSpeed(const int rpm)
{
    // TODO: impl
}

int ConfigurationManager::getMotorSpeed() const
{
    // TODO: impl
    return 120;
}

int ConfigurationManager::getCurrentBlindsPosition() const
{
    return 0;
}

int ConfigurationManager::getMaxBlindsPosition() const
{
    return 0;
}

void ConfigurationManager::setCurrentBlindsPosition(const int position) const
{
    //TODO: impl
}

void ConfigurationManager::setMaxBlindsPosition(const int maxPosition) const
{
    //TODO: impl
}
