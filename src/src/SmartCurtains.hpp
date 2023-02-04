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
#ifndef SMARTCURTAINS_HPP
#define SMARTCURTAINS_HPP

#include <Arduino.h>

#include <hsmcpp/HsmEventDispatcherArduino.hpp>
#include <memory>

#include "Buzzer.hpp"
#include "ConfigurationManager.hpp"
#include "HomeAssistantIntegration.hpp"
#include "LimitSwitch.hpp"
#include "NetworkManager.hpp"
#include "StepperMotor.hpp"
#include "UpdateManager.hpp"
#include "WebFrontend.hpp"
#include "hsm/SmartCurtainsHsmBase.hpp"

class SmartCurtains : public SmartCurtainsHsmBase,
                      public INetworkListener,
                      public IHomeAssistantListener,
                      public IWebFrontendListener,
                      public IStepperMotorListener,
                      public ILimitSwitchListener,
                      public IUpdateListener {
public:
    SmartCurtains();
    virtual ~SmartCurtains() = default;

    void initialize();
    void processEvents();

private:
    void onTransitionFailed(const SmartCurtainsHsmEvents event, const hsmcpp::VariantVector_t& args) override;

    void onLoadConfiguration(const hsmcpp::VariantVector_t& args) override;
    void onStartWiFiAccessPoint(const hsmcpp::VariantVector_t& args) override;
    void onStartConfigPortal(const hsmcpp::VariantVector_t& args) override;
    void onFinalizeInitialConfiguration(const hsmcpp::VariantVector_t& args) override;
    void onStopConfigPortal(const hsmcpp::VariantVector_t& args) override;

    void onPrepareDevice(const hsmcpp::VariantVector_t& args) override;
    void onStartOtaManager(const hsmcpp::VariantVector_t& args) override;
    void onStartWebFrontend(const hsmcpp::VariantVector_t& args) override;

    void onBeginHaRegistration(const hsmcpp::VariantVector_t& args) override;
    void onReconnectMqttConnection(const hsmcpp::VariantVector_t& args) override;

    void onStopNetworkConnections(const hsmcpp::VariantVector_t& args) override;
    void onResetDevice(const hsmcpp::VariantVector_t& args) override;
    void onRebootDevice(const hsmcpp::VariantVector_t& args) override;

    bool onDeviceAvailable(const hsmcpp::VariantVector_t& args) override;
    void onDeviceUnavailable(const hsmcpp::VariantVector_t& args) override;

    void onLoadLastPosition(const hsmcpp::VariantVector_t& args) override;
    void onStartPositionDetection(const hsmcpp::VariantVector_t& args) override;
    void onRestoreInitialPosition(const hsmcpp::VariantVector_t& args) override;

    void onResetStateBeforeCalibration(const hsmcpp::VariantVector_t& args) override;
    void onPrepareCalibration(const hsmcpp::VariantVector_t& args) override;
    void onCalibrationMove(const hsmcpp::VariantVector_t& args) override;
    void onCalibrationStopMovement(const hsmcpp::VariantVector_t& args) override;
    // void onCancelCalibration(const hsmcpp::VariantVector_t& args) override;
    // TODO: add this action to Calibrating::onExit after integrating new hsmcpp
    void onCancelCalibration(const hsmcpp::VariantVector_t& args);
    void onFinalizeCalibration(const hsmcpp::VariantVector_t& args) override;

    bool isInstallationMode(const hsmcpp::VariantVector_t& args) override;
    void onInstallationSpinCW(const hsmcpp::VariantVector_t& args) override;
    void onInstallationSpinCCW(const hsmcpp::VariantVector_t& args) override;
    void onSpinningModeChanged(const hsmcpp::VariantVector_t& args) override;

    void onStateFullyOpenCurtains(const hsmcpp::VariantVector_t& args) override;
    void onStateClosedCurtains(const hsmcpp::VariantVector_t& args) override;
    void onStateOpeningCurtains(const hsmcpp::VariantVector_t& args) override;
    void onStateClosingCurtains(const hsmcpp::VariantVector_t& args) override;
    void onRequestStopMotor(const hsmcpp::VariantVector_t& args) override;
    bool onRequestStopMotor() override;
    void onStateStoppedCurtains(const hsmcpp::VariantVector_t& args) override;

    void onCurtainsPositionChanged(const hsmcpp::VariantVector_t& args) override;

    void onUpdateSystemTime(const hsmcpp::VariantVector_t& args) override;
    void onValidateSystemTime(const hsmcpp::VariantVector_t& args) override;

    void onHomeAssistantUnavailable(const hsmcpp::VariantVector_t& args) override;
    void onHomeAssistantAvailable(const hsmcpp::VariantVector_t& args) override;

    bool hasCorrectSystemTime();
    bool isFullyClosed(const hsmcpp::VariantVector_t& args) override;
    bool isFullyOpen(const hsmcpp::VariantVector_t& args) override;
    bool isPartiallyOpen(const hsmcpp::VariantVector_t& args) override;

private:
    void onNetworkConnected() override;
    void onNetworkDisconnected(const WiFiDisconnectReason reason) override;
    void onNetworkAccessPointStarted() override;
    void onNetworkAccessPointStopped() override;

    // IHomeAssistantListener
private:
    void onMqttConnectionLost() override;
    void onHaConnected() override;
    void onHaDisconnected() override;
    void onHaRegistrationDone() override;

    void onHaRequestOpen() override;
    void onHaRequestClose() override;
    void onHaRequestStop() override;
    void onHaRequestSetPosition(const uint32_t newPosition) override;
    void onHaLastPositionLoaded() override;

    // IWebFrontendListener
private:
    int onFrontendRequest(const String& command) override;
    int onFrontendRequestData(const String& command, String& outData) override;
    void onFrontendResponseSent(const String& command, const int statusCode) override;
    void onFileUploaded(const String& id) override;
    const char* onFileUploadRequest(const String& id, const String& file) override;

    // IStepperMotorListener
private:
    void onMotorPositionChanged(const uint32_t pos, const uint32_t currentRevolution) override;
    void onMotorOperationFinished() override;

    // ILimitSwitchListener
private:
    void onLimitSwitchPressed() override;
    void onLimitSwitchReleased() override;

    // IUpdateListener
private:
    void onUpdateStarted() override;
    void onUpdateReadyToReboot() override;
    void onUpdateFailed() override;

private:
    std::shared_ptr<hsmcpp::HsmEventDispatcherArduino> mDispatcher;

    ConfigurationManager mConfiguration;
    HomeAssistantIntegration mHaIntegration;
    NetworkManager mNetworkManager;
    StepperMotor mMotor;
    UpdateManager mUpdateManager;
    WebFrontend mWebFrontend;
    LimitSwitch mLimitSwitch;
    Buzzer mBuzzer;
};

#endif  // SMARTCURTAINS_HPP