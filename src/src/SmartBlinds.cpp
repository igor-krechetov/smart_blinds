/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#include "SmartBlinds.hpp"
#include <ArduinoLog.h>

void SmartBlinds::initialize()
{
    mDispatcher = std::make_shared<hsmcpp::HsmEventDispatcherArduino>();
    SmartBlindsHsmBase::initialize(mDispatcher);

    mConfiguration.initialize(this);
    mNetworkManager.initialize(this);
    mUpdateManager.initialize();
    mHaIntegration.initialize(this);
    mWebFrontend.initialize(this);
    mMotor.initialize(this);

    transition(SmartBlindsHsmEvents::BOOTING_FINISHED);
}

void SmartBlinds::processEvents()
{
    mConfiguration.processEvents();
    mNetworkManager.processEvents();
    mUpdateManager.processEvents();
    mWebFrontend.processEvents();
    mHaIntegration.processEvents();
    mMotor.processEvents();

    mDispatcher->dispatchEvents();
}

//=======================================================================================
// HSM Callbacks
void SmartBlinds::onLoadConfiguration(const hsmcpp::VariantVector_t &args)
{
    Log.traceln("onLoadConfiguration");

    if (true == mConfiguration.loadConfiguration())
    {
        transition(SmartBlindsHsmEvents::CONFIGURED);
    }
    else
    {
        transition(SmartBlindsHsmEvents::CONFIG_NOT_AVAILABLE);
    }
}

void SmartBlinds::onSetupConfigPortal(const hsmcpp::VariantVector_t &args)
{
    Log.traceln("onSetupConfigPortal");
    mConfiguration.startConfigurationPortal();
}

void SmartBlinds::onPrepareDevice(const hsmcpp::VariantVector_t &args)
{
    Log.traceln("onPrepareDevice");
    mMotor.setSpeed(mConfiguration.getMotorSpeed());
    mNetworkManager.startWiFi(mConfiguration);
    mHaIntegration.start(mConfiguration);

    transition(SmartBlindsHsmEvents::DEVICE_READY);
}

void SmartBlinds::onStartWebFrontend(const hsmcpp::VariantVector_t &args)
{
    Log.traceln("onStartWebFrontend");
    mWebFrontend.startFrontend();
    transition(SmartBlindsHsmEvents::FRONTEND_READY);
}

void SmartBlinds::onBeginHaRegistration(const hsmcpp::VariantVector_t &args)
{
    Log.traceln("onBeginHaRegistration");
    // TODO: impl
}

// TODO: consider moving arg validation to onFrontendRequest
void SmartBlinds::onOpenCurtains(const hsmcpp::VariantVector_t &args)
{
    Log.traceln("onOpenCurtains");
    int distance;

    if (args.size() == 1) {
        distance = args[0].toUInt64();
    }
    if (0 == distance) {
        distance = 1;
    }

    mMotor.moveCW(distance);
}

void SmartBlinds::onCloseCurtains(const hsmcpp::VariantVector_t &args)
{
    Log.traceln("onCloseCurtains");
    int distance;

    if (args.size() == 1) {
        distance = args[0].toUInt64();
    }
    if (0 == distance) {
        distance = 1;
    }

    mMotor.moveCCW(distance);
}

//=======================================================================================
void SmartBlinds::onConfigured()
{
    Log.traceln("onConfigured");
    mConfiguration.stopConfigurationPortal();
    transition(SmartBlindsHsmEvents::CONFIGURED);
}

void SmartBlinds::onNetworkConnected()
{
    Log.traceln("onNetworkConnected");
    transition(SmartBlindsHsmEvents::WIFI_CONNECTED);
}

void SmartBlinds::onNetworkDisconnected()
{
    Log.traceln("onNetworkDisconnected");
    transition(SmartBlindsHsmEvents::WIFI_DISCONNECTED);
}

void SmartBlinds::onFrontendRequest(const String& command, const String& args)
{
    Log.traceln("onFrontendRequest: <%s>", command.c_str());
    
    // TODO: impl
    if (command == "open") {
        transition(SmartBlindsHsmEvents::OPEN_CURTAINS, static_cast<int32_t>(args.toInt()));
    } else if (command == "close") {
        transition(SmartBlindsHsmEvents::CLOSE_CURTAINS, static_cast<int32_t>(args.toInt()));
    } else if (command == "reset") {
        transition(SmartBlindsHsmEvents::RESET_DEVICE);
    }
}

void SmartBlinds::onMotorOperationFinished() {
    Log.traceln("onMotorOperationFinished");
    transition(SmartBlindsHsmEvents::OPERATION_DONE);
}