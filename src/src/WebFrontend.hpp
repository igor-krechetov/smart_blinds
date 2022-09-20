/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */
#ifndef WEBFRONTEND_HPP
#define WEBFRONTEND_HPP

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "ConfigurationManager.hpp"

class IWebFrontendListener
{
public:
    //TODO
    virtual void onFrontendRequest(const String& command, const String& args) = 0;
};

class WebFrontend
{
public:
    WebFrontend();

    void initialize(IWebFrontendListener *listener);
    void processEvents();

    void startFrontend();
    void stopFrontend();

private:
    void handlePathRoot();
    void handlePath404();

private:
    IWebFrontendListener *mListener = nullptr;
    bool mIsRunning = false;

#ifdef ESP32
    WebServer server;
#else
    ESP8266WebServer server;
#endif
};

#endif // WEBFRONTEND_HPP