/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#include "WebFrontend.hpp"
#include "LoggingHelper.hpp"
#include <LittleFS.h>

// TODO: add authorisation

WebFrontend::WebFrontend() : server(80)
{
}

void WebFrontend::initialize(IWebFrontendListener *listener)
{
    mListener = listener;

    server.on("/", [&]()
              { handlePathRoot(); });
    // server.on("/open", HTTPMethod::HTTP_GET, [&]() {
    //     mListener->onFrontendRequest("open", server.arg("distance"));
    //     server.send(HTTP_RESULT_OK, "OK");
    // });
    // server.on("/close", HTTPMethod::HTTP_GET, [&]() {
    //     mListener->onFrontendRequest("close", server.arg("distance"));
    //     server.send(HTTP_RESULT_OK, "OK");
    // });
    // server.on("/reset", HTTPMethod::HTTP_GET, [&]() { mListener->onFrontendRequest("reset", ""); server.send(HTTP_RESULT_OK, "OK");});

    server.on("/configure", HTTPMethod::HTTP_POST, [&]() {
                                                        sendResponse(mListener->onFrontendRequest(server.uri()));
                                                    },
                                                   [&](){ handleFileUpload(); });

    server.onNotFound([&]()
                      {
                        handlePath404();
                      });
}

void WebFrontend::processEvents()
{
    if (true == mIsRunning)
    {
        server.handleClient();
    }
}

void WebFrontend::startFrontend(PGM_P rootContent)
{
    mRootContent = rootContent;
    server.begin();
    mIsRunning = true;
}

void WebFrontend::stopFrontend()
{
    mIsRunning = false;
    server.stop();
}

bool WebFrontend::hasArgValue(const String& key) const
{
    return server.hasArg(key);
}

const String& WebFrontend::getArgValue(const String &key) const
{
    return server.arg(key);
}

const String& WebFrontend::getArgValue(const int index) const
{
    return server.arg(index);
}

int WebFrontend::getArgsCount() const
{
    return server.args();
}

void WebFrontend::handlePathRoot()
{
    server.send_P(HTTP_RESULT_OK, "text/html", mRootContent);
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

    sendResponse(HTTP_RESULT_NOT_FOUND, message);
}

void WebFrontend::handleFileUpload()
{
    HTTPUpload &upload = server.upload();

    if (upload.status == UPLOAD_FILE_START)
    {
        const char* path = mListener->onFileUploadRequest(upload.name, upload.filename);

        if (nullptr != path) {
            TRACE_ARGS("handleFileUpload: <%s>=<%s>", upload.name.c_str(), path);

            mUploadingFile.close();
            mUploadingFile = LittleFS.open(path, "w");

            if (false == mUploadingFile.isFile()){
                ERROR_ARGS("failed to open file <%s>", path);
                sendResponse(HTTP_RESULT_SERVER_ERROR, "Failed to store file");
            }
        } else {
            WARNING("file skipped because it was not recognized");
        }
    }
    else if ((upload.status == UPLOAD_FILE_WRITE) && (true == mUploadingFile.isFile()))
    {
        mUploadingFile.write(upload.buf, upload.currentSize);
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        TRACE("handleFileUpload: DONE");
        if (mUploadingFile.isFile())
        {
            mUploadingFile.close();
        }
        else
        {
            // server.send(HTTP_RESULT_SERVER_ERROR, "text/plain", "HTTP_RESULT_SERVER_ERROR: upload failed");
        }

        mListener->onFileUploaded(upload.name);
    }
}

void WebFrontend::sendResponse(const int code) {
    server.send(code);
    mListener->onFrontendResponseSent(code);
}

void WebFrontend::sendResponse(const int code, const char* message){
    server.send(code, "text/plain", message);
    mListener->onFrontendResponseSent(code);
}

void WebFrontend::sendResponse(const int code, const String& message){
    sendResponse(code, message.c_str());
}