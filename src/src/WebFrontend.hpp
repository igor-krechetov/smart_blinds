/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */
#ifndef WEBFRONTEND_HPP
#define WEBFRONTEND_HPP

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "ConfigurationManager.hpp"

#define HTTP_RESULT_OK                  (200)
#define HTTP_RESULT_BAD_REQUEST         (400)
#define HTTP_RESULT_NOT_FOUND           (404)
#define HTTP_RESULT_SERVER_ERROR        (500)

class IWebFrontendListener
{
public:
    // sent when a server requesst is received. arguments can beaccessed through getArgValue()
    virtual int onFrontendRequest(const String& command) = 0;
    // sent after server have sent reply to a client
    virtual void onFrontendResponseSent(const int statusCode) = 0;
    // sent before starting receiving file. client must return path where file should be store
    virtual const char* onFileUploadRequest(const String& id, const String& file) = 0;
    // sent when file is fully received
    virtual void onFileUploaded(const String& id) = 0;
};

class WebFrontend
{
public:
    WebFrontend();

    void initialize(IWebFrontendListener *listener);
    void processEvents();

    void startFrontend(PGM_P rootContent);
    void stopFrontend();

    bool hasArgValue(const String& key) const;// POST + GET
    const String& getArgValue(const String& key) const;// POST + GET
    const String& getArgValue(const int index) const;// for GET requests
    int getArgsCount() const;// for GET requests

private:
    void handlePathRoot();
    void handlePath404();
    void handleFileUpload();
    void sendResponse(const int code);
    void sendResponse(const int code, const char* message);
    void sendResponse(const int code, const String& message);

private:
    IWebFrontendListener *mListener = nullptr;
    bool mIsRunning = false;
    PGM_P mRootContent = nullptr;
    File mUploadingFile;

#ifdef ESP32
    WebServer server;
#else
    ESP8266WebServer server;
#endif
};

#endif // WEBFRONTEND_HPP