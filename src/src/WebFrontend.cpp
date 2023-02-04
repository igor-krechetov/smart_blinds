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

#include "WebFrontend.hpp"

#include <LittleFS.h>

#include "LoggingHelper.hpp"
#include "resources/bootstrap.bundle.min.js.mem"
#include "resources/bootstrap.min.css.mem"

// TODO: add authorisation

WebFrontend::WebFrontend() : server(80) {}

void WebFrontend::initialize(IWebFrontendListener* listener) {
    mListener = listener;

    server.on("/", [&]() { handlePathRoot(); });
    server.on(
        "/configure",
        HTTPMethod::HTTP_POST,
        [&]() { sendResponse(mListener->onFrontendRequest(server.uri())); },
        [&]() { handleFileUpload(); });
    server.on("/enable_installation", HTTPMethod::HTTP_GET, [&]() {
        sendResponse(mListener->onFrontendRequest(server.uri()));
    });
    server.on("/finish_installation", HTTPMethod::HTTP_GET, [&]() {
        sendResponse(mListener->onFrontendRequest(server.uri()));
    });
    server.on("/reboot", HTTPMethod::HTTP_GET, [&]() { sendResponse(mListener->onFrontendRequest(server.uri())); });
    server.on("/reset", HTTPMethod::HTTP_GET, [&]() { sendResponse(mListener->onFrontendRequest(server.uri())); });
    server.on("/open", HTTPMethod::HTTP_GET, [&]() { sendResponse(mListener->onFrontendRequest(server.uri())); });
    server.on("/close", HTTPMethod::HTTP_GET, [&]() { sendResponse(mListener->onFrontendRequest(server.uri())); });
    server.on("/stop", HTTPMethod::HTTP_GET, [&]() { sendResponse(mListener->onFrontendRequest(server.uri())); });
    server.on("/set_pos", HTTPMethod::HTTP_GET, [&]() { sendResponse(mListener->onFrontendRequest(server.uri())); });
    server.on("/move", HTTPMethod::HTTP_GET, [&]() { sendResponse(mListener->onFrontendRequest(server.uri())); });
    server.on("/calibration_begin", HTTPMethod::HTTP_GET, [&]() { sendResponse(mListener->onFrontendRequest(server.uri())); });
    server.on("/calibration_move", HTTPMethod::HTTP_GET, [&]() { sendResponse(mListener->onFrontendRequest(server.uri())); });
    server.on("/calibration_finish", HTTPMethod::HTTP_GET, [&]() { sendResponse(mListener->onFrontendRequest(server.uri())); });
    server.on("/bootstrap.min.css", HTTPMethod::HTTP_GET, [&]() { server.send_P(200, "text/css", gResourceBootstrapCss); });
    server.on("/bootstrap.bundle.min.js", HTTPMethod::HTTP_GET, [&]() {
        server.send_P(200, "text/javascript", gResourceBootstrapJs);
    });
    server.on("/get_status", HTTPMethod::HTTP_GET, [&]() {
        String response;
        const int code = mListener->onFrontendRequestData(server.uri(), response);
        sendResponse(code, response);
    });
    server.onNotFound([&]() { handlePath404(); });
}

void WebFrontend::processEvents() {
    if (true == mIsRunning) {
        server.handleClient();
    }
}

void WebFrontend::startFrontend(PGM_P rootContent) {
    mRootContent = rootContent;
    server.begin();
    mIsRunning = true;
}

void WebFrontend::stopFrontend() {
    mIsRunning = false;
    server.stop();
}

bool WebFrontend::hasArgValue(const String& key) const {
    return server.hasArg(key);
}

const String& WebFrontend::getArgValue(const String& key) const {
    return server.arg(key);
}

const String& WebFrontend::getArgValue(const int index) const {
    return server.arg(index);
}

int WebFrontend::getArgsCount() const {
    return server.args();
}

void WebFrontend::handlePathRoot() {
    server.send_P(HTTP_RESULT_OK, "text/html", mRootContent);
}

void WebFrontend::handlePath404() {
    // TODO: use static pages and send_P
    String message = "File Not Found\n\n";

    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";

    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }

    sendResponse(HTTP_RESULT_NOT_FOUND, message);
}

void WebFrontend::handleFileUpload() {
    HTTPUpload& upload = server.upload();

    if (upload.status == UPLOAD_FILE_START) {
        const char* path = mListener->onFileUploadRequest(upload.name, upload.filename);

        if (nullptr != path) {
            TRACE_ARGS("handleFileUpload: <%s>=<%s>", upload.name.c_str(), path);

            mUploadingFile.close();
            mUploadingFile = LittleFS.open(path, "w");

            if (false == mUploadingFile.isFile()) {
                ERROR_ARGS("failed to open file <%s>", path);
                sendResponse(HTTP_RESULT_SERVER_ERROR, "Failed to store file");
            }
        } else {
            WARNING("file skipped because it was not recognized");
        }
    } else if ((upload.status == UPLOAD_FILE_WRITE) && (true == mUploadingFile.isFile())) {
        mUploadingFile.write(upload.buf, upload.currentSize);
    } else if (upload.status == UPLOAD_FILE_END) {
        TRACE("handleFileUpload: DONE");
        if (mUploadingFile.isFile()) {
            mUploadingFile.close();
        } else {
            // server.send(HTTP_RESULT_SERVER_ERROR, "text/plain", "HTTP_RESULT_SERVER_ERROR: upload failed");
        }

        mListener->onFileUploaded(upload.name);
    }
}

void WebFrontend::sendResponse(const int code) {
    server.send(code);
    mListener->onFrontendResponseSent(server.uri(), code);
}

void WebFrontend::sendResponse(const int code, const char* message) {
    server.send(code, "text/plain", message);
    mListener->onFrontendResponseSent(server.uri(), code);
}

void WebFrontend::sendResponse(const int code, const String& message) {
    sendResponse(code, message.c_str());
}