/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#include "NetworkManager.hpp"
#include "LoggingHelper.hpp"

void NetworkManager::initialize(INetworkListener *listener)
{
    mListener = listener;

    // Don't save WiFi configuration in flash
    WiFi.persistent(false);
    Serial.setDebugOutput(true);

    mHandlerOnDisconnected = WiFi.onStationModeDisconnected([&](const WiFiEventStationModeDisconnected &event)
                                                            {
                                                                WiFi.printDiag(Serial);
                                                                mListener->onNetworkDisconnected();
                                                            });
    // WiFi.onStationModeConnected([&](const WiFiEventStationModeConnected &event){});
    mHandlerOnIP = WiFi.onStationModeGotIP([&](const WiFiEventStationModeGotIP &event)
                                           {
                                               TRACE_ARGS("Station connected, IP: %s", WiFi.localIP().toString().c_str());
                                               mListener->onNetworkConnected();
                                           });
}

void NetworkManager::processEvents()
{
    // checkWiFiConnection();
}

void NetworkManager::startWiFi(const ConfigurationManager &config)
{
    // WiFi.mode(WIFI_STA);
    WiFi.mode(WIFI_STA);
    WiFi.begin(config.getWiFiSSID(), config.getWiFiPassword(), 9);
}

void NetworkManager::stopWifi()
{
    WiFi.mode(WIFI_OFF);
}