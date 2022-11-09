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
#ifndef DISABLE_LOGGING
    Serial.setDebugOutput(true);
#endif

    mHandlerOnDisconnected = WiFi.onStationModeDisconnected([&](const WiFiEventStationModeDisconnected &event)
                                            {
                                                WiFi.printDiag(Serial);
                                                mListener->onNetworkDisconnected();
                                            });
    mHandlerOnIP = WiFi.onStationModeGotIP([&](const WiFiEventStationModeGotIP &event)
                                           {
                                               TRACE_ARGS("Station connected, IP: %s", WiFi.localIP().toString().c_str());
                                               mListener->onNetworkConnected();
                                           });
    mHandlerOnApStarted = WiFi.onSoftAPModeStationConnected([&](const WiFiEventSoftAPModeStationConnected &event)
                                            {
                                                TRACE_ARGS("WiFi AP started, IP: %s", WiFi.localIP().toString().c_str());
                                                mListener->onNetworkAccessPointStarted();
                                            });
    mHandlerOnApStopped = WiFi.onSoftAPModeStationDisconnected([&](const WiFiEventSoftAPModeStationDisconnected &event)
                                            {
                                                mListener->onNetworkAccessPointStopped();
                                            });
}

void NetworkManager::processEvents()
{
    // checkWiFiConnection();
}

void NetworkManager::connectWiFi(const ConfigurationManager &config)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(config.getWiFiSSID(), config.getWiFiPassword(), 9);
}

void NetworkManager::disconnectWifi()
{
    WiFi.mode(WIFI_OFF);
}

bool NetworkManager::startWiFiAP(const ConfigurationManager &config)
{
    static const IPAddress ip(192,168,7,1);
    static const IPAddress gateway(192,168,7,1);
    static const IPAddress network(255,255,255,0);

    // WiFi.mode(WIFI_AP);// TODO: is this needed?
    WiFi.softAPConfig(ip, gateway, network);
    return WiFi.softAP(config.getPrivateWiFiSSID(), config.getPrivateWiFiPassword());
}

void NetworkManager::stopWifiAP()
{
    WiFi.mode(WIFI_OFF);
}
