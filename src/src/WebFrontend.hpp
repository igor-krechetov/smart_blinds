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
#ifndef WEBFRONTEND_HPP
#define WEBFRONTEND_HPP

#include <Arduino.h>
#include <ESP8266WebServer.h>

#define HTTP_RESULT_OK (200)
#define HTTP_RESULT_BAD_REQUEST (400)
#define HTTP_RESULT_NOT_FOUND (404)
#define HTTP_RESULT_SERVER_ERROR (500)

class IWebFrontendListener {
public:
    // sent when a server request is received. arguments can be accessed through getArgValue()
    virtual int onFrontendRequest(const String& command) = 0;
    // sent when a server request that requires response is received. arguments can be accessed through getArgValue()
    virtual int onFrontendRequestData(const String& command, String& outData) = 0;
    // sent after server have sent reply to a client
    virtual void onFrontendResponseSent(const String& command, const int statusCode) = 0;
    // sent before starting receiving file. client must return path where file should be store
    virtual const char* onFileUploadRequest(const String& id, const String& file) = 0;
    // sent when file is fully received
    virtual void onFileUploaded(const String& id) = 0;
};

class WebFrontend {
public:
    WebFrontend();

    void initialize(IWebFrontendListener* listener);
    void processEvents();

    void startFrontend(PGM_P rootContent);
    void stopFrontend();

    bool hasArgValue(const String& key) const;           // POST + GET
    const String& getArgValue(const String& key) const;  // POST + GET
    const String& getArgValue(const int index) const;    // for GET requests
    int getArgsCount() const;                            // for GET requests

private:
    void handlePathRoot();
    void handlePath404();
    void handleFileUpload();
    void sendResponse(const int code);
    void sendResponse(const int code, const char* message);
    void sendResponse(const int code, const String& message);

private:
    IWebFrontendListener* mListener = nullptr;
    bool mIsRunning = false;
    PGM_P mRootContent = nullptr;
    File mUploadingFile;

#ifdef ESP32
    WebServer server;
#else
    ESP8266WebServer server;
#endif
};

#endif  // WEBFRONTEND_HPP