/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#include "SmartCurtains.hpp"
#include "LoggingHelper.hpp"
#include "resources/setup.html.mem"

#define POS_CLOSED          (0)
#define POS_OPEN            (100)

SmartCurtains::SmartCurtains()
    : mUpdateManager(this) {
}

void SmartCurtains::initialize()
{
    mDispatcher = std::make_shared<hsmcpp::HsmEventDispatcherArduino>();
    SmartCurtainsHsmBase::initialize(mDispatcher);

    mConfiguration.initialize();
    mNetworkManager.initialize(this);
    mHaIntegration.initialize(this);
    mWebFrontend.initialize(this);
    mMotor.initialize(this);

    transition(SmartCurtainsHsmEvents::BOOTING_FINISHED);
}

void SmartCurtains::processEvents()
{
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
void SmartCurtains::onTransitionFailed(const SmartCurtainsHsmEvents event, const hsmcpp::VariantVector_t& args)
{
    ERROR_ARGS("onTransitionFailed (event=%d)", SC2INT(event));
}

void SmartCurtains::onLoadConfiguration(const hsmcpp::VariantVector_t &args)
{
    TRACE("onLoadConfiguration");

    if (true == mConfiguration.loadConfiguration())
    {
        transition(SmartCurtainsHsmEvents::CONFIGURED);
    }
    else
    {
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

void SmartCurtains::onPrepareDevice(const hsmcpp::VariantVector_t &args)
{
    TRACE("onPrepareDevice");

    PRINT_FREE_HEAP();
    mMotor.setSpeed(mConfiguration.getMotorSpeed());
    mMotor.setLimit(mConfiguration.getMaxBlindsPosition());
    mNetworkManager.connectWiFi(mConfiguration);
    mLimitSwitch.initialize(this);
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

void SmartCurtains::onStartWebFrontend(const hsmcpp::VariantVector_t &args)
{
    TRACE("onStartWebFrontend");
    PRINT_FREE_HEAP();
    // TODO: impl onStartWebFrontend
    // mWebFrontend.startFrontend();
    PRINT_FREE_HEAP();
    transition(SmartCurtainsHsmEvents::FRONTEND_READY);
}

void SmartCurtains::onBeginHaRegistration(const hsmcpp::VariantVector_t &args)
{
    TRACE("onBeginHaRegistration");
    PRINT_FREE_HEAP();
    // TODO: check mHaIntegration.start() result and recover
    mHaIntegration.start(mConfiguration);
    PRINT_FREE_HEAP();
}

void SmartCurtains::onStopWebFrontend(const hsmcpp::VariantVector_t& args)
{
    TRACE("onStopWebFrontend");
    
    mWebFrontend.stopFrontend();
}

void SmartCurtains::onResetDevice(const hsmcpp::VariantVector_t &args)
{
    TRACE("onResetDevice");
    
    mConfiguration.resetConfiguration();
    transition(SmartCurtainsHsmEvents::RESET_DONE);
}

void SmartCurtains::onRebootDevice(const hsmcpp::VariantVector_t& args) {
    TRACE("onRebootDevice");

    delay(500);
    ESP.restart();
}

// TODO: need to wait for topic update or read it from local memory
void SmartCurtains::onLoadLastPosition(const hsmcpp::VariantVector_t &args)
{
    int lastPos = mHaIntegration.getLastPosition();

    TRACE_ARGS("onLoadLastPosition: %d", lastPos);

    if ((-1) != lastPos) {
        mMotor.overrideCurrentPosition(lastPos);
    } else {
        // TODO: need to detect current position 
        // NOTE: for now just assume we are fully closed
        mMotor.overrideCurrentPosition(POS_CLOSED);
        mHaIntegration.updateCurrentPosition(POS_CLOSED);
    }

    transition(SmartCurtainsHsmEvents::OPERATION_DONE);
    // NOTE: notify HA that device is ready to work
    mHaIntegration.updateAvailability(DeviceAvailability::ONLINE);
}

void SmartCurtains::onStateFullyOpenCurtains(const hsmcpp::VariantVector_t &args)
{
    TRACE("onStateFullyOpenCurtains");

    mMotor.overrideCurrentPosition(POS_OPEN);
    mHaIntegration.updateCurrentPosition(POS_OPEN);
    mHaIntegration.updateDeviceState(CoverState::OPEN);
}

void SmartCurtains::onStateClosedCurtains(const hsmcpp::VariantVector_t &args)
{
    TRACE("onStateClosedCurtains");
    mHaIntegration.updateDeviceState(CoverState::CLOSED);
}

void SmartCurtains::onStateOpeningCurtains(const hsmcpp::VariantVector_t &args)
{
    TRACE("onStateOpeningCurtains");

    mHaIntegration.updateDeviceState(CoverState::OPENING);

    if (args.size() == 1)
    {
        mMotor.moveToPosition(args[0].toUInt64());
    }
    PRINT_FREE_HEAP();
}

void SmartCurtains::onStateClosingCurtains(const hsmcpp::VariantVector_t &args)
{
    TRACE("onStateClosingCurtains");

    mHaIntegration.updateDeviceState(CoverState::CLOSING);

    if (args.size() == 1)
    {
        mMotor.moveToPosition(args[0].toUInt64());
    }
    PRINT_FREE_HEAP();
}

void SmartCurtains::onRequestStopMotor(const hsmcpp::VariantVector_t& args)
{
    onRequestStopMotor();
    PRINT_FREE_HEAP();
}

bool SmartCurtains::onRequestStopMotor() {
    TRACE("onRequestStopMotor");
    mMotor.stopMovement();
    return true;
}

void SmartCurtains::onStateStoppedCurtains(const hsmcpp::VariantVector_t &args)
{
    TRACE("onStateStoppedCurtains");

    mHaIntegration.updateDeviceState(CoverState::STOPPED);
    PRINT_FREE_HEAP();
}

void SmartCurtains::onCurtainsPositionChanged(const hsmcpp::VariantVector_t& args)
{
    TRACE("onCurtainsPositionChanged");

    if (args.size() == 1)
    {
        mHaIntegration.updateCurrentPosition(args[0].toUInt64());
    }
    PRINT_FREE_HEAP();
}

void SmartCurtains::onUpdateSystemTime(const hsmcpp::VariantVector_t& args)
{
    TRACE("onUpdateSystemTime");
    const char *ntp1 = "time.windows.com";
    const char *ntp2 = "pool.ntp.org";
    
    configTime(2 * 3600, 1, ntp1, ntp2);
    PRINT_FREE_HEAP();
}

void SmartCurtains::onValidateSystemTime(const hsmcpp::VariantVector_t& args)
{
    TRACE("onValidateSystemTime");

    if (true == hasCorrectSystemTime(args))
    {
        transition(SmartCurtainsHsmEvents::TIME_UPDATED);
    }
}

void SmartCurtains::onHomeAssistantUnavailable(const hsmcpp::VariantVector_t &args)
{
    TRACE("onHomeAssistantUnavailable");
    // NOTE: do nothing
}

void SmartCurtains::onHomeAssistantAvailable(const hsmcpp::VariantVector_t &args)
{
    TRACE("onHomeAssistantAvailable");
    mHaIntegration.updateAvailability(DeviceAvailability::ONLINE);
}

bool SmartCurtains::hasCorrectSystemTime(const hsmcpp::VariantVector_t& args)
{
    time_t now = time(nullptr);

    TRACE_ARGS("---- hasCorrectSystemTime: %d", (int)(now < 2 * 3600));// TODO: remove before commit
    return (now < 2 * 3600);
}

bool SmartCurtains::isFullyClosed(const hsmcpp::VariantVector_t &args)
{
    // TODO: remove trace before commit
    TRACE_ARGS("---- isFullyClosed: pos=%d, closed=%d", (int)mMotor.getCurrentPosition(), (int)(mMotor.getCurrentPosition() == POS_CLOSED));
    return mMotor.getCurrentPosition() == POS_CLOSED;
}

bool SmartCurtains::isFullyOpen(const hsmcpp::VariantVector_t &args)
{
    // TODO: remove trace before commit
    TRACE_ARGS("---- isFullyOpen: pos=%d, closed=%d", (int)mMotor.getCurrentPosition(), (int)(mMotor.getCurrentPosition() == POS_OPEN));
    return mMotor.getCurrentPosition() == POS_OPEN;
}

bool SmartCurtains::isPartiallyOpen(const hsmcpp::VariantVector_t& args)
{
    int pos = mMotor.getCurrentPosition();
    return ((pos > 0) && (pos < 100));
}

//=======================================================================================
void SmartCurtains::onNetworkConnected()
{
    TRACE("onNetworkConnected");
    transition(SmartCurtainsHsmEvents::WIFI_CONNECTED);
}

void SmartCurtains::onNetworkDisconnected()
{
    TRACE("onNetworkDisconnected");
    transition(SmartCurtainsHsmEvents::WIFI_DISCONNECTED);
}

void SmartCurtains::onNetworkAccessPointStarted() {
    TRACE("onNetworkAccessPointStarted");
    transition(SmartCurtainsHsmEvents::AP_CONNECTED);
}

void SmartCurtains::onNetworkAccessPointStopped() {
    TRACE("onNetworkAccessPointStopped");
    transition(SmartCurtainsHsmEvents::AP_DISCONNECTED);
}

void SmartCurtains::onHaConnected()
{
    TRACE("onHaConnected");
    transition(SmartCurtainsHsmEvents::HA_ONLINE);
}

void SmartCurtains::onHaDisconnected()
{
    TRACE("onHaDisconnected");
    transition(SmartCurtainsHsmEvents::HA_OFFLINE);
}

void SmartCurtains::onHaRegistrationDone()
{
    TRACE("onHaRegistrationDone");
    transition(SmartCurtainsHsmEvents::REGISTERED_WITH_HA);
}

void SmartCurtains::onHaRequestOpen()
{
    TRACE("onHaRequestOpen");
    transition(SmartCurtainsHsmEvents::OPEN_CURTAINS, POS_OPEN);
}

void SmartCurtains::onHaRequestClose()
{
    TRACE("onHaRequestClose");
    transition(SmartCurtainsHsmEvents::CLOSE_CURTAINS, POS_CLOSED);
}

void SmartCurtains::onHaRequestStop()
{
    TRACE("onHaRequestStop");
    transition(SmartCurtainsHsmEvents::STOP_CURTAINS);
}

void SmartCurtains::onHaRequestSetPosition(const uint32_t newPosition)
{
    if (mMotor.getCurrentPosition() < newPosition)
    {
        transition(SmartCurtainsHsmEvents::OPEN_CURTAINS, newPosition);
    }
    else if (mMotor.getCurrentPosition() > newPosition)
    {
        transition(SmartCurtainsHsmEvents::CLOSE_CURTAINS, newPosition);
    }
}

int SmartCurtains::onFrontendRequest(const String &command)
{
    TRACE_ARGS("onFrontendRequest: <%s>", command.c_str());
    int statusCode = HTTP_RESULT_OK;

    if (command == "open")
    {
        // TODO: check if arg is needed
        if (1 == mWebFrontend.getArgsCount()) {
            transition(SmartCurtainsHsmEvents::OPEN_CURTAINS, static_cast<int32_t>(mWebFrontend.getArgValue(0).toInt()));
        }
    }
    else if (command == "close")
    {
        // TODO: check if arg is needed
        if (1 == mWebFrontend.getArgsCount()) {
            transition(SmartCurtainsHsmEvents::CLOSE_CURTAINS, static_cast<int32_t>(mWebFrontend.getArgValue(0).toInt()));
        }
    }
    else if (command == "reset")
    {
        transition(SmartCurtainsHsmEvents::RESET_DEVICE);
    }
    else if (command == "/configure")
    {
        TRACE_ARGS("args count: <%d>", mWebFrontend.getArgsCount());

        if (6 == mWebFrontend.getArgsCount())
        {
            // TODO: remove before commit. for debug
            // TRACE_ARGS("wifi_ssid: <%s>", mWebFrontend.getArgValue("wifi_ssid").c_str());
            // TRACE_ARGS("wifi_pwd: <%s>", mWebFrontend.getArgValue("wifi_pwd").c_str());
            // TRACE_ARGS("ota_port: <%s>", mWebFrontend.getArgValue("ota_port").c_str());
            // TRACE_ARGS("ota_pwd: <%s>", mWebFrontend.getArgValue("ota_pwd").c_str());
            // TRACE_ARGS("mqtt_host: <%s>", mWebFrontend.getArgValue("mqtt_host").c_str());
            // TRACE_ARGS("mqtt_port: <%s>", mWebFrontend.getArgValue("mqtt_port").c_str());

            mConfiguration.setWiFiSSID(mWebFrontend.getArgValue("wifi_ssid").c_str());
            mConfiguration.setWiFiPassword(mWebFrontend.getArgValue("wifi_pwd").c_str());
            mConfiguration.setOtaPort(mWebFrontend.getArgValue("ota_port").toInt());
            mConfiguration.setOtaPassword(mWebFrontend.getArgValue("ota_pwd").c_str());
            mConfiguration.setMqttServerHost(mWebFrontend.getArgValue("mqtt_host").c_str());
            mConfiguration.setMqttServerPort(mWebFrontend.getArgValue("mqtt_port").toInt());

            if (true == mConfiguration.validateConfiguration()) {
                if (true == mConfiguration.saveConfiguration()) {
                    TRACE("configured");
                } else {
                    ERROR("failed to save config");
                    statusCode =  HTTP_RESULT_SERVER_ERROR;
                }
            } else {
                TRACE("validation FAILED");
                statusCode =  HTTP_RESULT_BAD_REQUEST;
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

void SmartCurtains::onFrontendResponseSent(const int statusCode) {
    TRACE_ARGS("onFrontendResponseSent: statusCode=%d", statusCode);

    if (HTTP_RESULT_OK == statusCode) {
        transition(SmartCurtainsHsmEvents::CONFIGURATION_RECEIVED);
    } else {
        transition(SmartCurtainsHsmEvents::INVALID_CONFIG_REQUEST);
    }
}

void SmartCurtains::onFileUploaded(const String& id) {
    TRACE_ARGS("onFileUploaded: id=<%s>", id.c_str());

    transition(SmartCurtainsHsmEvents::CONFIGURATION_RECEIVED);
}

const char* SmartCurtains::onFileUploadRequest(const String& id, const String& file)
{
    TRACE_ARGS("onFileUploadRequest: id=<%s>, file=<%s>", id.c_str(), file.c_str());
    const char* uploadPath = nullptr;

    if (id == "mqtt_cert_ca"){
        uploadPath = mConfiguration.getMqttCaCertPath();
    }
    else if (id == "mqtt_cert_client"){
        uploadPath = mConfiguration.getMqttClientCertPath();
    }
    else if (id == "mqtt_client_key"){
        uploadPath = mConfiguration.getMqttClientKeyPath();
    }

    return uploadPath;
}

void SmartCurtains::onMotorPositionChanged(const uint32_t pos)
{
    TRACE_ARGS("onMotorPositionChanged: pos=%" PRIu32, pos);
    transition(SmartCurtainsHsmEvents::POS_CHANGED, pos);
}

void SmartCurtains::onMotorOperationFinished()
{
    TRACE("onMotorOperationFinished");
    transition(SmartCurtainsHsmEvents::OPERATION_DONE);
}

void SmartCurtains::onLimitSwitchPressed() {
    TRACE("onLimitSwitchPressed");
    transition(SmartCurtainsHsmEvents::OPEN_LIMIT_REACHED);
}

void SmartCurtains::onLimitSwitchReleased() {
    // NOTE: do nothing
}

void SmartCurtains::onUpdateStarted() {
    delay(5000);
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