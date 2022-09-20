/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */
#ifndef SMARTBLINDS_HPP
#define SMARTBLINDS_HPP

#include <Arduino.h>
#include <memory>
#include <hsmcpp/HsmEventDispatcherArduino.hpp>
#include "hsm/SmartBlindsHsmBase.hpp"

#include "ConfigurationManager.hpp"
#include "HomeAssistantIntegration.hpp"
#include "NetworkManager.hpp"
#include "StepperMotor.hpp"
#include "UpdateManager.hpp"
#include "WebFrontend.hpp"

// TODO: rename to SmartCurtains

class SmartBlinds: public SmartBlindsHsmBase,
                   public IConfigurationListener,
                   public INetworkListener,
                   public IHomeAssistantListener,
                   public IWebFrontendListener,
                   public IStepperMotorListener {
public:
    virtual ~SmartBlinds() = default;

    void initialize();
    void processEvents();

private:
    void onLoadConfiguration(const hsmcpp::VariantVector_t& args) override;
    void onSetupConfigPortal(const hsmcpp::VariantVector_t& args) override;
    void onPrepareDevice(const hsmcpp::VariantVector_t& args) override;
    void onStartWebFrontend(const hsmcpp::VariantVector_t& args) override;
    void onBeginHaRegistration(const hsmcpp::VariantVector_t& args) override;
    void onOpenCurtains(const hsmcpp::VariantVector_t& args) override;
    void onCloseCurtains(const hsmcpp::VariantVector_t& args) override;

private:
    void onConfigured() override;

private:
    void onNetworkConnected() override;
    void onNetworkDisconnected() override;


// IHomeAssistantListener
// IWebFrontendListener
private:
    void onFrontendRequest(const String& command, const String& args) override;

// IStepperMotorListener
private:
    void onMotorOperationFinished() override;

private:
    std::shared_ptr<hsmcpp::HsmEventDispatcherArduino> mDispatcher;

    ConfigurationManager mConfiguration;
    HomeAssistantIntegration mHaIntegration;
    NetworkManager mNetworkManager;
    StepperMotor mMotor;
    UpdateManager mUpdateManager;
    WebFrontend mWebFrontend;
};

#endif // SMARTBLINDS_HPP