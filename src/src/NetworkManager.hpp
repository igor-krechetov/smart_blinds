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
#ifndef NETWORKMANAGER_HPP
#define NETWORKMANAGER_HPP

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "ConfigurationManager.hpp"

class INetworkListener {
public:
    virtual void onNetworkConnected() = 0;
    virtual void onNetworkDisconnected(const WiFiDisconnectReason reason) = 0;

    virtual void onNetworkAccessPointStarted() = 0;
    virtual void onNetworkAccessPointStopped() = 0;
};

class NetworkManager {
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

#endif  // NETWORKMANAGER_HPP