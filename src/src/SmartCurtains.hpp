/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */
#ifndef SMARTCURTAINS_HPP
#define SMARTCURTAINS_HPP

#include <Arduino.h>
#include <memory>
#include <hsmcpp/HsmEventDispatcherArduino.hpp>
#include "hsm/SmartCurtainsHsmBase.hpp"

#include "ConfigurationManager.hpp"
#include "HomeAssistantIntegration.hpp"
#include "NetworkManager.hpp"
#include "StepperMotor.hpp"
#include "UpdateManager.hpp"
#include "WebFrontend.hpp"

// TODO: rename to SmartCurtains

class SmartCurtains: public SmartCurtainsHsmBase,
                   public IConfigurationListener,
                   public INetworkListener,
                   public IHomeAssistantListener,
                   public IWebFrontendListener,
                   public IStepperMotorListener {
public:
    virtual ~SmartCurtains() = default;

    void initialize();
    void processEvents();

private:
    void onLoadConfiguration(const hsmcpp::VariantVector_t& args) override;
    void onSetupConfigPortal(const hsmcpp::VariantVector_t& args) override;
    void onPrepareDevice(const hsmcpp::VariantVector_t& args) override;
    void onStartWebFrontend(const hsmcpp::VariantVector_t& args) override;
    void onBeginHaRegistration(const hsmcpp::VariantVector_t& args) override;

    bool onStopWebFrontend() override;
    void onResetDevice(const hsmcpp::VariantVector_t& args) override;

    void onLoadLastPosition(const hsmcpp::VariantVector_t& args) override;
    void onStateOpenCurtains(const hsmcpp::VariantVector_t& args) override;
    void onStateClosedCurtains(const hsmcpp::VariantVector_t& args) override;
    void onStateOpeningCurtains(const hsmcpp::VariantVector_t& args) override;
    void onStateClosingCurtains(const hsmcpp::VariantVector_t& args) override;
    void onStateStoppingCurtains(const hsmcpp::VariantVector_t& args) override;
    void onStateStoppedCurtains(const hsmcpp::VariantVector_t& args) override;

    void onCurtainsPositionChanged(const hsmcpp::VariantVector_t& args) override;

    void onHomeAssistantUnavailable(const hsmcpp::VariantVector_t& args) override;
    void onHomeAssistantAvailable(const hsmcpp::VariantVector_t& args) override;

    bool isFullyClosed(const hsmcpp::VariantVector_t& args) override;
    bool isFullyOpen(const hsmcpp::VariantVector_t& args) override;
    bool isPartiallyOpen(const hsmcpp::VariantVector_t& args) override;

private:
    void onConfigured() override;

private:
    void onNetworkConnected() override;
    void onNetworkDisconnected() override;

// IHomeAssistantListener
private:
    void onHaConnected() override;
    void onHaDisconnected() override;
    void onHaRegistrationDone() override;

    void onHaRequestOpen() override;
    void onHaRequestClose() override;
    void onHaRequestStop() override;
    void onHaRequestSetPosition(const uint32_t newPosition) override;

// IWebFrontendListener
private:
    void onFrontendRequest(const String& command, const String& args) override;

// IStepperMotorListener
private:
    void onMotorPositionChanged(const uint32_t pos) override;
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

#endif // SMARTCURTAINS_HPP