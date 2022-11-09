/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */
#ifndef NETWORKMANAGER_HPP
#define NETWORKMANAGER_HPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "ConfigurationManager.hpp"

class INetworkListener
{
public:
    virtual void onNetworkConnected() = 0;
    virtual void onNetworkDisconnected() = 0;

    virtual void onNetworkAccessPointStarted() = 0;
    virtual void onNetworkAccessPointStopped() = 0;
};

class NetworkManager
{
public:
    // NetworkManager();
    void initialize(INetworkListener *listener);
    void processEvents();

    void connectWiFi(const ConfigurationManager &config);
    void disconnectWifi();

    bool startWiFiAP(const ConfigurationManager &config);
    void stopWifiAP();

private:
    INetworkListener *mListener;
    WiFiEventHandler mHandlerOnIP;
    WiFiEventHandler mHandlerOnDisconnected;
    WiFiEventHandler mHandlerOnApStarted;
    WiFiEventHandler mHandlerOnApStopped;
};

#endif // NETWORKMANAGER_HPP