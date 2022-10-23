/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#include "WebFrontend.hpp"

// TODO: add authorisation

WebFrontend::WebFrontend() : server(80)
{
}

void WebFrontend::initialize(IWebFrontendListener *listener)
{
    mListener = listener;
    
    server.on("/", [&]() { handlePathRoot(); });
    server.on("/open", HTTPMethod::HTTP_GET, [&]() {
        mListener->onFrontendRequest("open", server.arg("distance"));
        server.send(200, "OK");
    });
    server.on("/close", HTTPMethod::HTTP_GET, [&]() { 
        mListener->onFrontendRequest("close", server.arg("distance"));
        server.send(200, "OK");
    });
    server.on("/reset", HTTPMethod::HTTP_GET, [&]() { mListener->onFrontendRequest("reset", ""); server.send(200, "OK");});
    server.onNotFound([&](){ handlePath404(); });

    // TODO: prepare HTML
}

void WebFrontend::processEvents()
{
    if (true == mIsRunning)
    {
        server.handleClient();
    }
}

void WebFrontend::startFrontend()
{
    server.begin();
    mIsRunning = true;
}

void WebFrontend::stopFrontend()
{
    mIsRunning = false;
    server.stop();
}

void WebFrontend::handlePathRoot()
{
    server.send(200, "text/html", "ROOT: OK");
}

void WebFrontend::handlePath404()
{
    // TODO: use static pages and send_P
    String message = "File Not Found\n\n";

    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";

    for (uint8_t i = 0; i < server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }

    server.send(404, "text/plain", message);
}