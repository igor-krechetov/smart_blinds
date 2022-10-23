/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#include "UpdateManager.hpp"
#include <ArduinoOTA.h>
#include "LoggingHelper.hpp"

UpdateManager::UpdateManager(IUpdateListener* listener) : mListener(listener) {
}

void UpdateManager::start(const ConfigurationManager &config){
    ArduinoOTA.setRebootOnSuccess(false);
    ArduinoOTA.setPort(config.getOtaPort());
    ArduinoOTA.setPassword(config.getOtaPassword().c_str());

    ArduinoOTA.onStart([&]() {
        mListener->onUpdateStarted();
    });

    ArduinoOTA.onEnd([&]() {
        mListener->onUpdateReadyToReboot();
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        TRACE_ARGS("Progress: %u%%", (progress / (total / 100)));
    });

    ArduinoOTA.onError([&](ota_error_t error) {
        mListener->onUpdateFailed();
    });

    ArduinoOTA.begin(false);
}

void UpdateManager::processEvents(){
    ArduinoOTA.handle();
}