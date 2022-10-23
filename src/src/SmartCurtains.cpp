/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#include "SmartCurtains.hpp"
#include "LoggingHelper.hpp"

#define POS_CLOSED          (0)
#define POS_OPEN            (100)

SmartCurtains::SmartCurtains()
    : mUpdateManager(this) {
}

void SmartCurtains::initialize()
{
    mDispatcher = std::make_shared<hsmcpp::HsmEventDispatcherArduino>();
    SmartCurtainsHsmBase::initialize(mDispatcher);

    mConfiguration.initialize(this);
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

void SmartCurtains::onSetupConfigPortal(const hsmcpp::VariantVector_t &args)
{
    TRACE("onSetupConfigPortal");
    mConfiguration.startConfigurationPortal();
}

void SmartCurtains::onPrepareDevice(const hsmcpp::VariantVector_t &args)
{
    TRACE("onPrepareDevice");

    PRINT_FREE_HEAP();
    mMotor.setSpeed(mConfiguration.getMotorSpeed());
    mMotor.setLimit(mConfiguration.getMaxBlindsPosition());
    mNetworkManager.startWiFi(mConfiguration);
    mLimitSwitch.initialize(this);
    PRINT_FREE_HEAP();

    transition(SmartCurtainsHsmEvents::DEVICE_READY);
}

void SmartCurtains::onStartOtaManager(const hsmcpp::VariantVector_t& args) {
    TRACE("onStartOtaManager");

    PRINT_FREE_HEAP();
    mUpdateManager.start(mConfiguration);
    PRINT_FREE_HEAP();
}

void SmartCurtains::onStartWebFrontend(const hsmcpp::VariantVector_t &args)
{
    TRACE("onStartWebFrontend");
    PRINT_FREE_HEAP();
    mWebFrontend.startFrontend();
    PRINT_FREE_HEAP();
    transition(SmartCurtainsHsmEvents::FRONTEND_READY);
}

void SmartCurtains::onBeginHaRegistration(const hsmcpp::VariantVector_t &args)
{
    TRACE("onBeginHaRegistration");
    PRINT_FREE_HEAP();
    mHaIntegration.start(mConfiguration);
    PRINT_FREE_HEAP();
}

bool SmartCurtains::onStopWebFrontend()
{
    TRACE("onStopWebFrontend");
    // TODO: impl
    return true;
}

void SmartCurtains::onResetDevice(const hsmcpp::VariantVector_t &args)
{
    TRACE("onResetDevice");
    // TODO: impl
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
    // TODO: impl
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
    // TODO: impl
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

void SmartCurtains::onHomeAssistantUnavailable(const hsmcpp::VariantVector_t &args)
{
    TRACE("onHomeAssistantUnavailable");
    // TODO: impl
}

void SmartCurtains::onHomeAssistantAvailable(const hsmcpp::VariantVector_t &args)
{
    TRACE("onHomeAssistantAvailable");
    // TODO: impl
    mHaIntegration.updateAvailability(DeviceAvailability::ONLINE);
}

bool SmartCurtains::isFullyClosed(const hsmcpp::VariantVector_t &args)
{
    TRACE_ARGS("---- isFullyClosed: pos=%d, closed=%d", (int)mMotor.getCurrentPosition(), (int)(mMotor.getCurrentPosition() == POS_CLOSED));
    return mMotor.getCurrentPosition() == POS_CLOSED;
}

bool SmartCurtains::isFullyOpen(const hsmcpp::VariantVector_t &args)
{
    TRACE_ARGS("---- isFullyOpen: pos=%d, closed=%d", (int)mMotor.getCurrentPosition(), (int)(mMotor.getCurrentPosition() == POS_OPEN));
    return mMotor.getCurrentPosition() == POS_OPEN;
}

bool SmartCurtains::isPartiallyOpen(const hsmcpp::VariantVector_t& args)
{
    int pos = mMotor.getCurrentPosition();
    return ((pos > 0) && (pos < 100));
}

//=======================================================================================
void SmartCurtains::onConfigured()
{
    TRACE("onConfigured");
    mConfiguration.stopConfigurationPortal();
    transition(SmartCurtainsHsmEvents::CONFIGURED);
}

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

void SmartCurtains::onFrontendRequest(const String &command, const String &args)
{
    TRACE_ARGS("onFrontendRequest: <%s>", command.c_str());

    // TODO: impl
    if (command == "open")
    {
        transition(SmartCurtainsHsmEvents::OPEN_CURTAINS, static_cast<int32_t>(args.toInt()));
    }
    else if (command == "close")
    {
        transition(SmartCurtainsHsmEvents::CLOSE_CURTAINS, static_cast<int32_t>(args.toInt()));
    }
    else if (command == "reset")
    {
        transition(SmartCurtainsHsmEvents::RESET_DEVICE);
    }
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