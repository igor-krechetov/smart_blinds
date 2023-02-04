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

#include "UpdateManager.hpp"

#include <ArduinoOTA.h>

#include "LoggingHelper.hpp"

// TODO: web ui update: https://www.esp8266.com/viewtopic.php?p=63855

UpdateManager::UpdateManager(IUpdateListener* listener) : mListener(listener) {}

void UpdateManager::start(const ConfigurationManager& config) {
    ArduinoOTA.setRebootOnSuccess(false);
    ArduinoOTA.setPort(config.getOtaPort());
    ArduinoOTA.setPassword(config.getOtaPassword());

    ArduinoOTA.onStart([&]() { mListener->onUpdateStarted(); });

    ArduinoOTA.onEnd([&]() { mListener->onUpdateReadyToReboot(); });

    ArduinoOTA.onProgress(
        [](unsigned int progress, unsigned int total) { TRACE_ARGS("Progress: %u%%", (progress / (total / 100))); });

    ArduinoOTA.onError([&](ota_error_t error) { mListener->onUpdateFailed(); });

    ArduinoOTA.begin(false);
}

void UpdateManager::processEvents() {
    ArduinoOTA.handle();
}