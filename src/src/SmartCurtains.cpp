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

#include "SmartCurtains.hpp"

#include "LoggingHelper.hpp"
#include "resources/frontend.html.mem"
#include "resources/setup.html.mem"

#define POS_CLOSED (100)
#define POS_OPEN (0)

#define LOCAL_TIMEZONE (9)

// TODO: test if parents' exit callback is called when exiting from top level state

SmartCurtains::SmartCurtains() : mHaIntegration(mConfiguration), mUpdateManager(this) {}

void SmartCurtains::initialize() {
    bool res = false;
    mDispatcher = std::make_shared<hsmcpp::HsmEventDispatcherArduino>();
    SmartCurtainsHsmBase::initialize(mDispatcher);

    if (true == mConfiguration.initialize()) {
        mNetworkManager.initialize(this);

        if (true == mHaIntegration.initialize(this)) {
            mWebFrontend.initialize(this);
            mBuzzer.initialize();

            TRACE_ARGS("FIRMWARE VERSION: %s", mConfiguration.getFirmwareVersion());
            res = true;
        }
    }

    if (true == res) {
        transition(SmartCurtainsHsmEvents::BOOTING_FINISHED);
    } else {
        transition(SmartCurtainsHsmEvents::BOOTING_FAILED);
    }
}

void SmartCurtains::processEvents() {
    mConfiguration.processEvents();
    mNetworkManager.processEvents();
    mUpdateManager.processEvents();
    mWebFrontend.processEvents();
    mHaIntegration.processEvents();
    mMotor.processEvents();
    mLimitSwitch.processEvents();

    mDispatcher->dispatchEvents();
}

//=======================================================================================
// HSM Callbacks
void SmartCurtains::onTransitionFailed(const SmartCurtainsHsmEvents event, const hsmcpp::VariantVector_t& args) {
    ERROR_ARGS("onTransitionFailed (event=%d)", SC2INT(event));
}

void SmartCurtains::onLoadConfiguration(const hsmcpp::VariantVector_t& args) {
    TRACE("onLoadConfiguration");

    if (true == mConfiguration.loadConfiguration()) {
        transition(SmartCurtainsHsmEvents::CONFIGURED);
    } else {
        transition(SmartCurtainsHsmEvents::CONFIG_NOT_AVAILABLE);
    }
}

void SmartCurtains::onStartWiFiAccessPoint(const hsmcpp::VariantVector_t& args) {
    TRACE("onStartWiFiAccessPoint");

    if (false == mNetworkManager.startWiFiAP(mConfiguration)) {
        transition(SmartCurtainsHsmEvents::CONFIGURATION_FAILED);
    }
}

void SmartCurtains::onStartConfigPortal(const hsmcpp::VariantVector_t& args) {
    TRACE("onStartConfigPortal");
    mWebFrontend.startFrontend(gPageSetup);
    transition(SmartCurtainsHsmEvents::CONFIG_PORTAL_STARTED);
}

void SmartCurtains::onFinalizeInitialConfiguration(const hsmcpp::VariantVector_t& args) {
    TRACE("onFinalizeInitialConfiguration");

    mWebFrontend.stopFrontend();
    mNetworkManager.stopWifiAP();
    transition(SmartCurtainsHsmEvents::CONFIGURED);
}

void SmartCurtains::onStopConfigPortal(const hsmcpp::VariantVector_t& args) {
    TRACE("onStopConfigPortal");
    mWebFrontend.stopFrontend();
}

void SmartCurtains::onPrepareDevice(const hsmcpp::VariantVector_t& args) {
    TRACE("onPrepareDevice");

    PRINT_FREE_HEAP();
    mMotor.setSpeed(mConfiguration.getMotorSpeed());
    mMotor.setRevolutionsLimit(mConfiguration.getMaxBlindsPosition());
    mNetworkManager.connectWiFi(mConfiguration);
    mLimitSwitch.initialize(this);
    mMotor.initialize(this);
    PRINT_FREE_HEAP();

    transition(SmartCurtainsHsmEvents::DEVICE_READY);
}

void SmartCurtains::onStartOtaManager(const hsmcpp::VariantVector_t& args) {
    TRACE("onStartOtaManager");

    PRINT_FREE_HEAP();
    mUpdateManager.start(mConfiguration);
    PRINT_FREE_HEAP();

    transition(SmartCurtainsHsmEvents::OTA_READY);
}

void SmartCurtains::onStartWebFrontend(const hsmcpp::VariantVector_t& args) {
    TRACE("onStartWebFrontend");
    PRINT_FREE_HEAP();
    mWebFrontend.startFrontend(gPageFrontend);
    PRINT_FREE_HEAP();

    mBuzzer.buzzLetter('R');
    transition(SmartCurtainsHsmEvents::FRONTEND_READY);
}

void SmartCurtains::onBeginHaRegistration(const hsmcpp::VariantVector_t& args) {
    TRACE("onBeginHaRegistration");
    PRINT_FREE_HEAP();

    if (false == mHaIntegration.start()) {
        transition(SmartCurtainsHsmEvents::ON_TIMER_HA_REGISTRATION);
    }
    PRINT_FREE_HEAP();
}

void SmartCurtains::onReconnectMqttConnection(const hsmcpp::VariantVector_t& args) {
    TRACE("onReconnectMqttConnection");
    mHaIntegration.reconnect();
}

void SmartCurtains::onStopNetworkConnections(const hsmcpp::VariantVector_t& args) {
    TRACE("onStopNetworkConnections");

    mWebFrontend.stopFrontend();
    mHaIntegration.disconnect();
    // NOTE: there is no API to stop UpdateManager, but OTA will correctly reinitialize after WiFi reconnection
}

void SmartCurtains::onResetDevice(const hsmcpp::VariantVector_t& args) {
    TRACE("onResetDevice");

    mConfiguration.resetConfiguration();
    transition(SmartCurtainsHsmEvents::RESET_DONE);
}

void SmartCurtains::onRebootDevice(const hsmcpp::VariantVector_t& args) {
    TRACE("onRebootDevice");

    // TODO: check if this is correct location
    mHaIntegration.updateAvailability(DeviceAvailability::OFFLINE);

    mBuzzer.buzzLetter('O');  // rebOOt
    delay(500);
    ESP.restart();
}

bool SmartCurtains::onDeviceAvailable(const hsmcpp::VariantVector_t& args) {
    TRACE("onDeviceAvailable");
    // NOTE: notify HA that device is ready to work
    mHaIntegration.updateCurrentPosition(mMotor.getCurrentPosition());
    mHaIntegration.updateAvailability(DeviceAvailability::ONLINE);
    return true;
}

void SmartCurtains::onDeviceUnavailable(const hsmcpp::VariantVector_t& args) {
    TRACE("onDeviceUnavailable");
    mBuzzer.buzzLetter('U');
    mHaIntegration.updateAvailability(DeviceAvailability::OFFLINE);
}

// TODO: need to wait for topic update or read it from local memory
void SmartCurtains::onLoadLastPosition(const hsmcpp::VariantVector_t& args) {
    int lastPos = mHaIntegration.getLastPosition();

    TRACE_ARGS("onLoadLastPosition: %d", lastPos);

    if (INVALID_LAST_POSITION != lastPos) {
        mMotor.overrideCurrentPosition(lastPos);
        transition(SmartCurtainsHsmEvents::POSITION_INITIALIZED);
    } else {
        // TODO: try reading from local memory
        transition(SmartCurtainsHsmEvents::POSITION_UNAVAILABLE);
    }
}

void SmartCurtains::onStartPositionDetection(const hsmcpp::VariantVector_t& args) {
    TRACE("onStartPositionDetection");

    mMotor.overrideCurrentPosition(POS_CLOSED);
    mMotor.moveToPosition(POS_OPEN);
}

void SmartCurtains::onRestoreInitialPosition(const hsmcpp::VariantVector_t& args) {
    TRACE("onRestoreInitialPosition");
    const uint32_t currentPos = mMotor.getCurrentPosition();

    mMotor.overrideCurrentPosition(POS_OPEN);
    mMotor.moveToPosition(POS_OPEN - currentPos);
}

void SmartCurtains::onResetStateBeforeCalibration(const hsmcpp::VariantVector_t& args) {
    TRACE("onResetStateBeforeCalibration");

    // keep opening until we reach limit switch
    mMotor.enablePositionLimit(false);
    mMotor.overrideCurrentPosition(100000);
    mMotor.moveToPosition(0);
}

void SmartCurtains::onPrepareCalibration(const hsmcpp::VariantVector_t& args) {
    TRACE("onPrepareCalibration");

    mMotor.overrideCurrentPosition(POS_OPEN);
}

void SmartCurtains::onCalibrationMove(const hsmcpp::VariantVector_t& args) {
    TRACE("onCalibrationMove");

    if (args.size() == 1) {
        bool res = false;
        const int64_t distance = args[0].toInt64();

        if (distance > 0) {
            res = mMotor.moveCW(distance);
        } else if (distance < 0) {
            res = mMotor.moveCCW(-distance);
        }

        if (false == res) {
            transition(SmartCurtainsHsmEvents::OPERATION_DONE);
        }
    }
}

void SmartCurtains::onCalibrationStopMovement(const hsmcpp::VariantVector_t& args) {
    TRACE("onCalibrationStopMovement");
    mMotor.stopMovement(true);
}

void SmartCurtains::onCancelCalibration(const hsmcpp::VariantVector_t& args) {
    TRACE("onCancelCalibration");
    onFinalizeCalibration(args);
}

void SmartCurtains::onFinalizeCalibration(const hsmcpp::VariantVector_t& args) {
    TRACE("onFinalizeCalibration");
    bool saveResults = (args.size() == 1 ? args[0].toBool() : false);
    const uint32_t oldMaxLimit = mConfiguration.getMaxBlindsPosition();

    mMotor.enablePositionLimit(true);

    if (true == saveResults) {
        mMotor.setRevolutionsLimit(mMotor.getCurrentRevolution());
        mConfiguration.setMaxBlindsPosition(mMotor.getCurrentRevolution());
        mConfiguration.setCurrentBlindsPosition(mMotor.getCurrentPosition());
        saveResults = mConfiguration.saveConfiguration();

        if (false == saveResults) {
            ERROR("failed to save config");
        }
    }

    if (false == saveResults) {
        if (mMotor.getCurrentRevolution() > oldMaxLimit) {
            // TODO: need to move back
        }

        mConfiguration.setMaxBlindsPosition(oldMaxLimit);
        mMotor.setRevolutionsLimit(oldMaxLimit);
    }
}

bool SmartCurtains::isInstallationMode(const hsmcpp::VariantVector_t& args) {
    const bool enabled = mConfiguration.isInstallationMode();
    TRACE_ARGS("isInstallationMode => %d", (int)enabled);
    return enabled;
}

void SmartCurtains::onInstallationSpinCW(const hsmcpp::VariantVector_t& args) {
    TRACE("onInstallationSpinCW");
    mMotor.startSpinningCW();
}

void SmartCurtains::onInstallationSpinCCW(const hsmcpp::VariantVector_t& args) {
    TRACE("onInstallationSpinCCW");
    mMotor.startSpinningCCW();
}

void SmartCurtains::onSpinningModeChanged(const hsmcpp::VariantVector_t& args) {
    mBuzzer.buzzLetter('M');
}

void SmartCurtains::onStateFullyOpenCurtains(const hsmcpp::VariantVector_t& args) {
    TRACE("onStateFullyOpenCurtains");

    mHaIntegration.updateDeviceState(CoverState::OPEN);
}

void SmartCurtains::onStateClosedCurtains(const hsmcpp::VariantVector_t& args) {
    TRACE("onStateClosedCurtains");
    mHaIntegration.updateDeviceState(CoverState::CLOSED);
}

void SmartCurtains::onStateOpeningCurtains(const hsmcpp::VariantVector_t& args) {
    mHaIntegration.updateDeviceState(CoverState::OPENING);

    if (args.size() == 1) {
        TRACE_ARGS("onStateOpeningCurtains, pos=%d", static_cast<int>(args[0].toUInt64()));

        mMotor.moveToPosition(args[0].toUInt64());
    }
    PRINT_FREE_HEAP();
}

void SmartCurtains::onStateClosingCurtains(const hsmcpp::VariantVector_t& args) {
    TRACE("onStateClosingCurtains");

    mHaIntegration.updateDeviceState(CoverState::CLOSING);

    if (args.size() == 1) {
        mMotor.moveToPosition(args[0].toUInt64());
    }
    PRINT_FREE_HEAP();
}

void SmartCurtains::onRequestStopMotor(const hsmcpp::VariantVector_t& args) {
    onRequestStopMotor();
}

bool SmartCurtains::onRequestStopMotor() {
    TRACE("onRequestStopMotor");
    mMotor.stopMovement(true);
    PRINT_FREE_HEAP();
    return true;
}

void SmartCurtains::onStateStoppedCurtains(const hsmcpp::VariantVector_t& args) {
    TRACE("onStateStoppedCurtains");

    mHaIntegration.updateDeviceState(CoverState::STOPPED);
    PRINT_FREE_HEAP();
}

void SmartCurtains::onCurtainsPositionChanged(const hsmcpp::VariantVector_t& args) {
    TRACE("onCurtainsPositionChanged");

    if (args.size() == 1) {
        // TODO: update only if not calibrating
        mHaIntegration.updateCurrentPosition(args[0].toUInt64());
    }
    PRINT_FREE_HEAP();
}

void SmartCurtains::onUpdateSystemTime(const hsmcpp::VariantVector_t& args) {
    TRACE("onUpdateSystemTime");

    if (false == hasCorrectSystemTime()) {
        const char* ntp1 = "time.windows.com";
        const char* ntp2 = "pool.ntp.org";

        configTime(mConfiguration.getTimezone() * 3600, 1, ntp1, ntp2);
        PRINT_FREE_HEAP();
    } else {
        transition(SmartCurtainsHsmEvents::TIME_UPDATED);
    }
}

void SmartCurtains::onValidateSystemTime(const hsmcpp::VariantVector_t& args) {
    TRACE("onValidateSystemTime");

    if (true == hasCorrectSystemTime()) {
        transition(SmartCurtainsHsmEvents::TIME_UPDATED);
    }
}

void SmartCurtains::onHomeAssistantUnavailable(const hsmcpp::VariantVector_t& args) {
    TRACE("onHomeAssistantUnavailable");
    // NOTE: do nothing
}

void SmartCurtains::onHomeAssistantAvailable(const hsmcpp::VariantVector_t& args) {
    TRACE("onHomeAssistantAvailable");
    mHaIntegration.updateAvailability(DeviceAvailability::ONLINE);
}

bool SmartCurtains::hasCorrectSystemTime() {
    return (time(nullptr) > (mConfiguration.getTimezone() * 3600));
}

bool SmartCurtains::isFullyClosed(const hsmcpp::VariantVector_t& args) {
    return mMotor.getCurrentPosition() == POS_CLOSED;
}

bool SmartCurtains::isFullyOpen(const hsmcpp::VariantVector_t& args) {
    return (mMotor.getCurrentPosition() == POS_OPEN);
}

bool SmartCurtains::isPartiallyOpen(const hsmcpp::VariantVector_t& args) {
    int pos = mMotor.getCurrentPosition();
    TRACE_ARGS("---- isPartiallyOpen: pos=%d, result=%d", pos, (int)((pos > 0) && (pos < 100)));
    return ((pos > 0) && (pos < 100));
}

//=======================================================================================
void SmartCurtains::onNetworkConnected() {
    TRACE("onNetworkConnected");
    transition(SmartCurtainsHsmEvents::WIFI_CONNECTED);
}

void SmartCurtains::onNetworkDisconnected(const WiFiDisconnectReason reason) {
    TRACE_ARGS("onNetworkDisconnected: reason=%d", static_cast<int>(reason));

    if ((WIFI_DISCONNECT_REASON_NO_AP_FOUND == reason) || (WIFI_DISCONNECT_REASON_AUTH_FAIL == reason)) {
        transition(SmartCurtainsHsmEvents::INVALID_WIFI_CREDENTIALS);
    } else {
        transition(SmartCurtainsHsmEvents::WIFI_DISCONNECTED);
    }
}

void SmartCurtains::onNetworkAccessPointStarted() {
    TRACE("onNetworkAccessPointStarted");
    transition(SmartCurtainsHsmEvents::AP_CONNECTED);
}

void SmartCurtains::onNetworkAccessPointStopped() {
    TRACE("onNetworkAccessPointStopped");
    transition(SmartCurtainsHsmEvents::AP_DISCONNECTED);
}

void SmartCurtains::onMqttConnectionLost() {
    TRACE("onMqttConnectionLost");
    // mBuzzer.buzzLetter('Q');
    transition(SmartCurtainsHsmEvents::LOST_MQTT_CONNECTION);
}

void SmartCurtains::onHaConnected() {
    TRACE("onHaConnected");
    transition(SmartCurtainsHsmEvents::HA_ONLINE);
}

void SmartCurtains::onHaDisconnected() {
    TRACE("onHaDisconnected");
    transition(SmartCurtainsHsmEvents::HA_OFFLINE);
}

void SmartCurtains::onHaRegistrationDone() {
    TRACE("onHaRegistrationDone");
    transition(SmartCurtainsHsmEvents::REGISTERED_WITH_HA);
}

void SmartCurtains::onHaRequestOpen() {
    TRACE("onHaRequestOpen");
    transition(SmartCurtainsHsmEvents::OPEN_CURTAINS, POS_OPEN);
}

void SmartCurtains::onHaRequestClose() {
    TRACE("onHaRequestClose");
    transition(SmartCurtainsHsmEvents::CLOSE_CURTAINS, POS_CLOSED);
}

void SmartCurtains::onHaRequestStop() {
    TRACE("onHaRequestStop");
    transition(SmartCurtainsHsmEvents::STOP_CURTAINS);
}

void SmartCurtains::onHaRequestSetPosition(const uint32_t newPosition) {
    if (mMotor.getCurrentPosition() > newPosition) {
        transition(SmartCurtainsHsmEvents::OPEN_CURTAINS, newPosition);
    } else if (mMotor.getCurrentPosition() < newPosition) {
        transition(SmartCurtainsHsmEvents::CLOSE_CURTAINS, newPosition);
    }
}

void SmartCurtains::onHaLastPositionLoaded() {
    TRACE("onHaLastPositionLoaded");
    transition(SmartCurtainsHsmEvents::POSITION_RECEIVED);
}

int SmartCurtains::onFrontendRequest(const String& command) {
    TRACE_ARGS("onFrontendRequest: <%s>, args=%d", command.c_str(), mWebFrontend.getArgsCount());
    int statusCode = HTTP_RESULT_OK;

    if (command == "/open") {
        transition(SmartCurtainsHsmEvents::OPEN_CURTAINS, POS_OPEN);
    } else if (command == "/close") {
        transition(SmartCurtainsHsmEvents::CLOSE_CURTAINS, POS_CLOSED);
    } else if (command == "/stop") {
        transition(SmartCurtainsHsmEvents::STOP_CURTAINS);
    } else if (command == "/set_pos") {
        if (1 == mWebFrontend.getArgsCount()) {
            onHaRequestSetPosition(mWebFrontend.getArgValue(0).toInt());
        } else {
            statusCode = HTTP_RESULT_BAD_REQUEST;
        }
    } else if (command == "/move") {
        if (1 == mWebFrontend.getArgsCount()) {
            int dist = mWebFrontend.getArgValue(0).toInt();
            uint32_t newPos = mMotor.getCurrentPosition() + dist;

            onHaRequestSetPosition(newPos);
        } else {
            statusCode = HTTP_RESULT_BAD_REQUEST;
        }
    } else if (command == "/enable_installation") {
        mConfiguration.setInstallationMode(true);

        if (true == mConfiguration.saveConfiguration()) {
            transition(SmartCurtainsHsmEvents::REBOOT_DEVICE);
            statusCode = HTTP_RESULT_OK;
        } else {
            statusCode = HTTP_RESULT_SERVER_ERROR;
        }
    } else if (command == "/finish_installation") {
        mConfiguration.setInstallationMode(false);

        if (true == mConfiguration.saveConfiguration()) {
            transition(SmartCurtainsHsmEvents::FINISHED_INSTALLATION);
            statusCode = HTTP_RESULT_OK;
        } else {
            statusCode = HTTP_RESULT_SERVER_ERROR;
        }
    } else if (command == "/calibration_begin") {
        transition(SmartCurtainsHsmEvents::START_CALIBRATION);
    } else if (command == "/calibration_finish") {
        if (1 == mWebFrontend.getArgsCount()) {
            transition(SmartCurtainsHsmEvents::FINISH_CALIBRATION, (mWebFrontend.getArgValue(0).toInt() != 0));
        } else {
            statusCode = HTTP_RESULT_BAD_REQUEST;
        }
    } else if (command == "/calibration_move") {
        if (1 == mWebFrontend.getArgsCount()) {
            if (0 != mWebFrontend.getArgValue(0).toInt()) {
                transition(SmartCurtainsHsmEvents::CALIBRATION_MOVE, static_cast<int32_t>(mWebFrontend.getArgValue(0).toInt()));
            } else {
                transition(SmartCurtainsHsmEvents::STOP_CURTAINS);  // TODO: validate
            }
        } else {
            statusCode = HTTP_RESULT_BAD_REQUEST;
        }
    } else if (command == "/reset") {
        transition(SmartCurtainsHsmEvents::RESET_DEVICE);
    } else if (command == "/reboot") {
        transition(SmartCurtainsHsmEvents::REBOOT_DEVICE);
    } else if (command == "/configure") {
        TRACE_ARGS("args count: <%d>", mWebFrontend.getArgsCount());

        if (8 == mWebFrontend.getArgsCount()) {
            mConfiguration.setWiFiSSID(mWebFrontend.getArgValue("wifi_ssid").c_str());
            mConfiguration.setWiFiPassword(mWebFrontend.getArgValue("wifi_pwd").c_str());
            mConfiguration.setOtaPort(mWebFrontend.getArgValue("ota_port").toInt());
            mConfiguration.setOtaPassword(mWebFrontend.getArgValue("ota_pwd").c_str());
            mConfiguration.setMqttServerHost(mWebFrontend.getArgValue("mqtt_host").c_str());
            mConfiguration.setMqttServerPort(mWebFrontend.getArgValue("mqtt_port").toInt());
            mConfiguration.setMqttUser(mWebFrontend.getArgValue("mqtt_user").c_str());
            mConfiguration.setMqttPassword(mWebFrontend.getArgValue("mqtt_pwd").c_str());

            if (true == mConfiguration.validateConfiguration()) {
                if (true == mConfiguration.saveConfiguration()) {
                    TRACE("configured");
                } else {
                    ERROR("failed to save config");
                    statusCode = HTTP_RESULT_SERVER_ERROR;
                }
            } else {
                TRACE("validation FAILED");
                statusCode = HTTP_RESULT_BAD_REQUEST;
            }
        } else {
            ERROR_ARGS("unexpected amount of arguments: %d", mWebFrontend.getArgsCount());
            statusCode = HTTP_RESULT_SERVER_ERROR;
        }
    } else {
        statusCode = HTTP_RESULT_NOT_FOUND;
    }

    return statusCode;
}

#include <ArduinoJson.h>

int SmartCurtains::onFrontendRequestData(const String& command, String& outData) {
    TRACE_ARGS("onFrontendRequestData: <%s>, args=%d", command.c_str(), mWebFrontend.getArgsCount());
    int statusCode = HTTP_RESULT_OK;

    if (command == "/get_status") {
        // v - Version
        // im - Installation Mode
        // {"v":"xx.yy","im":false}
        StaticJsonDocument<40> deviceStatus;

        deviceStatus[F("v")] = mConfiguration.getFirmwareVersion();
        deviceStatus[F("im")] = mConfiguration.isInstallationMode();

        statusCode = HTTP_RESULT_OK;
    } else {
        statusCode = HTTP_RESULT_NOT_FOUND;
    }

    return statusCode;
}

void SmartCurtains::onFrontendResponseSent(const String& command, const int statusCode) {
    TRACE_ARGS("onFrontendResponseSent: command=<%s>, statusCode=%d", command.c_str(), statusCode);

    if (HTTP_RESULT_OK == statusCode) {
        if (command == "/configure") {
            transition(SmartCurtainsHsmEvents::CONFIGURATION_RECEIVED);
        }
    }
    //  else {
    //     transition(SmartCurtainsHsmEvents::INVALID_CONFIG_REQUEST);
    // }
}

void SmartCurtains::onFileUploaded(const String& id) {
    TRACE_ARGS("onFileUploaded: id=<%s>", id.c_str());

    transition(SmartCurtainsHsmEvents::CONFIGURATION_RECEIVED);
}

const char* SmartCurtains::onFileUploadRequest(const String& id, const String& file) {
    TRACE_ARGS("onFileUploadRequest: id=<%s>, file=<%s>", id.c_str(), file.c_str());
    const char* uploadPath = nullptr;

    if (id == "mqtt_cert_ca") {
        uploadPath = mConfiguration.getMqttCaCertPath();
    } else if (id == "mqtt_cert_client") {
        uploadPath = mConfiguration.getMqttClientCertPath();
    } else if (id == "mqtt_client_key") {
        uploadPath = mConfiguration.getMqttClientKeyPath();
    }

    return uploadPath;
}

void SmartCurtains::onMotorPositionChanged(const uint32_t pos, const uint32_t currentRevolution) {
    TRACE_ARGS("onMotorPositionChanged: pos=%" PRIu32 ", rev=%" PRIu32, pos, currentRevolution);
    transition(SmartCurtainsHsmEvents::POS_CHANGED, pos);
}

void SmartCurtains::onMotorOperationFinished() {
    TRACE("onMotorOperationFinished");
    transition(SmartCurtainsHsmEvents::OPERATION_DONE);
}

void SmartCurtains::onLimitSwitchPressed() {
    TRACE("onLimitSwitchPressed");
    transition(SmartCurtainsHsmEvents::OPEN_LIMIT_PRESSED);
}

void SmartCurtains::onLimitSwitchReleased() {
    transition(SmartCurtainsHsmEvents::OPEN_LIMIT_RELEASED);
}

void SmartCurtains::onUpdateStarted() {
    TRACE("onUpdateStarted");
    transition(SmartCurtainsHsmEvents::UPDATE_STARTED);
}

void SmartCurtains::onUpdateReadyToReboot() {
    TRACE("onUpdateReadyToReboot");
    transition(SmartCurtainsHsmEvents::UPDATE_FINISHED);
}

void SmartCurtains::onUpdateFailed() {
    TRACE("onUpdateFailed");
    transition(SmartCurtainsHsmEvents::UPDATE_FAILED);
}