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

#include "NetworkManager.hpp"

#include "LoggingHelper.hpp"

void NetworkManager::initialize(INetworkListener *listener) {
    mListener = listener;

    // NOTE: in case weird crashes will occur, try:
    // https://forum.arduino.cc/t/wemos-d1-mini-pro-crashing-with-wdt-on-enabling-wifi/1073222/12 WiFi.setOutputPower(15);

    // Don't save WiFi configuration in flash
    WiFi.persistent(false);
#if !defined(DISABLE_APP_LOGGING)
    Serial.setDebugOutput(true);
#endif

    mHandlerOnDisconnected = WiFi.onStationModeDisconnected([&](const WiFiEventStationModeDisconnected &event) {
#if !defined(DISABLE_APP_LOGGING)
        WiFi.printDiag(Serial);
#endif
        mListener->onNetworkDisconnected(event.reason);
    });
    mHandlerOnIP = WiFi.onStationModeGotIP([&](const WiFiEventStationModeGotIP &event) {
        TRACE_ARGS("Station connected, IP: %s", WiFi.localIP().toString().c_str());
        mListener->onNetworkConnected();
    });
    mHandlerOnApStarted = WiFi.onSoftAPModeStationConnected([&](const WiFiEventSoftAPModeStationConnected &event) {
        TRACE_ARGS("WiFi AP started, IP: %s", WiFi.localIP().toString().c_str());
        mListener->onNetworkAccessPointStarted();
    });
    mHandlerOnApStopped = WiFi.onSoftAPModeStationDisconnected(
        [&](const WiFiEventSoftAPModeStationDisconnected &event) { mListener->onNetworkAccessPointStopped(); });
}

void NetworkManager::processEvents() {
    // checkWiFiConnection();
}

void NetworkManager::connectWiFi(const ConfigurationManager &config) {
    disconnectWifi();
    WiFi.mode(WIFI_STA);
    WiFi.begin(config.getWiFiSSID(), config.getWiFiPassword(), 9);
}

void NetworkManager::disconnectWifi() {
    WiFi.mode(WIFI_OFF);
}

bool NetworkManager::startWiFiAP(const ConfigurationManager &config) {
    // WiFi.mode(WIFI_AP);// TODO: is this needed?
    disconnectWifi();
    WiFi.softAPConfig(IPAddress(192, 168, 7, 1), IPAddress(192, 168, 7, 1), IPAddress(255, 255, 255, 0));
    return WiFi.softAP(config.getPrivateWiFiSSID(), config.getPrivateWiFiPassword());
}

void NetworkManager::stopWifiAP() {
    WiFi.mode(WIFI_OFF);
}
