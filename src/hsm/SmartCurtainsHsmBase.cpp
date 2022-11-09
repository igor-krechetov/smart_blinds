// Content of this file was generated

#include "SmartCurtainsHsmBase.hpp"

SmartCurtainsHsmBase::SmartCurtainsHsmBase()
    : hsmcpp::HierarchicalStateMachine<SmartCurtainsHsmStates, SmartCurtainsHsmEvents>(SmartCurtainsHsmStates::Booting)
{
    configureHsm();
}

SmartCurtainsHsmBase::~SmartCurtainsHsmBase()
{}

void SmartCurtainsHsmBase::configureHsm()
{
    registerFailedTransitionCallback<SmartCurtainsHsmBase>(this, &SmartCurtainsHsmBase::onTransitionFailed);

    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::LoadConfiguration, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onLoadConfiguration), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::ConnectingToWiFi);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::NetworkAvailable);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Booting);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::WaitingInitialConfig);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::PrepareDevice, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onPrepareDevice), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::ResettingDevice, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onResetDevice), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::RebootingDevice, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onRebootDevice), nullptr, nullptr);
    registerFinalState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Final_3, SmartCurtainsHsmEvents::INVALID);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::InitializingFeatures);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Operable, this, nullptr, nullptr, static_cast<SmartCurtainsHsmBaseStateExitCallbackPtr_t>(&SmartCurtainsHsmBase::onRequestStopMotor));
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::UpdatingFirmware);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::UpdatingSystemTime, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onUpdateSystemTime), nullptr, nullptr);
    registerFinalState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Final_2, SmartCurtainsHsmEvents::INVALID);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::StartWiFIAccessPoint, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onStartWiFiAccessPoint), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::FinalizingInitialConfiguration, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onFinalizeInitialConfiguration), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::WaitingForConfigRequest);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::StartConfigPortal, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onStartConfigPortal), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::ReceivingConfigParts);
    registerFinalState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Final_6, SmartCurtainsHsmEvents::INVALID);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::PrepareFrontend, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onStartWebFrontend), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::HomeAssistantIntegration, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onBeginHaRegistration), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::PreparingOTA, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onStartOtaManager), nullptr, nullptr);
    registerFinalState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Final_1, SmartCurtainsHsmEvents::INVALID);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::LoadingLastPosition, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onLoadLastPosition), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::FullyOpen, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onStateFullyOpenCurtains), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::FullyClosed, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onStateClosedCurtains), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Opening, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onStateOpeningCurtains), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Closing, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onStateClosingCurtains), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Stopped, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onStateStoppedCurtains), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Stopping, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onRequestStopMotor), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::OpenLimitReached, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onRequestStopMotor), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::ReceivedConfigParts2);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::ReceivedConfigParts3);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::ReceivedConfigParts1);
    registerFinalState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Final_5, SmartCurtainsHsmEvents::INVALID);

    registerSubstateEntryPoint(SmartCurtainsHsmStates::NetworkAvailable, SmartCurtainsHsmStates::InitializingFeatures, SmartCurtainsHsmEvents::WIFI_CONNECTED, this, static_cast<SmartCurtainsHsmBaseTransitionConditionCallbackPtr_t>(&SmartCurtainsHsmBase::hasCorrectSystemTime), true);
    registerSubstateEntryPoint(SmartCurtainsHsmStates::NetworkAvailable, SmartCurtainsHsmStates::UpdatingSystemTime, SmartCurtainsHsmEvents::WIFI_CONNECTED, this, static_cast<SmartCurtainsHsmBaseTransitionConditionCallbackPtr_t>(&SmartCurtainsHsmBase::hasCorrectSystemTime), false);
    registerSubstate(SmartCurtainsHsmStates::NetworkAvailable, SmartCurtainsHsmStates::Operable);
    registerSubstate(SmartCurtainsHsmStates::NetworkAvailable, SmartCurtainsHsmStates::UpdatingFirmware);
    registerSubstate(SmartCurtainsHsmStates::NetworkAvailable, SmartCurtainsHsmStates::Final_2);
    registerSubstateEntryPoint(SmartCurtainsHsmStates::WaitingInitialConfig, SmartCurtainsHsmStates::StartWiFIAccessPoint);
    registerSubstate(SmartCurtainsHsmStates::WaitingInitialConfig, SmartCurtainsHsmStates::FinalizingInitialConfiguration);
    registerSubstate(SmartCurtainsHsmStates::WaitingInitialConfig, SmartCurtainsHsmStates::WaitingForConfigRequest);
    registerSubstate(SmartCurtainsHsmStates::WaitingInitialConfig, SmartCurtainsHsmStates::StartConfigPortal);
    registerSubstate(SmartCurtainsHsmStates::WaitingInitialConfig, SmartCurtainsHsmStates::ReceivingConfigParts);
    registerSubstate(SmartCurtainsHsmStates::WaitingInitialConfig, SmartCurtainsHsmStates::Final_6);
    registerSubstateEntryPoint(SmartCurtainsHsmStates::InitializingFeatures, SmartCurtainsHsmStates::PreparingOTA);
    registerSubstate(SmartCurtainsHsmStates::InitializingFeatures, SmartCurtainsHsmStates::PrepareFrontend);
    registerSubstate(SmartCurtainsHsmStates::InitializingFeatures, SmartCurtainsHsmStates::HomeAssistantIntegration);
    registerSubstate(SmartCurtainsHsmStates::InitializingFeatures, SmartCurtainsHsmStates::Final_1);
    registerSubstateEntryPoint(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::LoadingLastPosition);
    registerSubstate(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::FullyOpen);
    registerSubstate(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::FullyClosed);
    registerSubstate(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::Opening);
    registerSubstate(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::Closing);
    registerSubstate(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::Stopped);
    registerSubstate(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::Stopping);
    registerSubstate(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::OpenLimitReached);
    registerHistory(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::History_1, HistoryType::SHALLOW);
    registerSubstateEntryPoint(SmartCurtainsHsmStates::ReceivingConfigParts, SmartCurtainsHsmStates::ReceivedConfigParts1);
    registerSubstate(SmartCurtainsHsmStates::ReceivingConfigParts, SmartCurtainsHsmStates::ReceivedConfigParts2);
    registerSubstate(SmartCurtainsHsmStates::ReceivingConfigParts, SmartCurtainsHsmStates::ReceivedConfigParts3);
    registerSubstate(SmartCurtainsHsmStates::ReceivingConfigParts, SmartCurtainsHsmStates::Final_5);

    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::LoadConfiguration, SmartCurtainsHsmStates::PrepareDevice, SmartCurtainsHsmEvents::CONFIGURED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::LoadConfiguration, SmartCurtainsHsmStates::WaitingInitialConfig, SmartCurtainsHsmEvents::CONFIG_NOT_AVAILABLE);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::ConnectingToWiFi, SmartCurtainsHsmStates::NetworkAvailable, SmartCurtainsHsmEvents::WIFI_CONNECTED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::NetworkAvailable, SmartCurtainsHsmStates::ResettingDevice, SmartCurtainsHsmEvents::RESET_DEVICE);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::NetworkAvailable, SmartCurtainsHsmStates::ConnectingToWiFi, SmartCurtainsHsmEvents::WIFI_DISCONNECTED, this, &SmartCurtainsHsmBase::onStopWebFrontend);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::NetworkAvailable, SmartCurtainsHsmStates::RebootingDevice, SmartCurtainsHsmEvents::REBOOT_DEVICE);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Booting, SmartCurtainsHsmStates::LoadConfiguration, SmartCurtainsHsmEvents::BOOTING_FINISHED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::WaitingInitialConfig, SmartCurtainsHsmStates::PrepareDevice, SmartCurtainsHsmEvents::CONFIGURED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::WaitingInitialConfig, SmartCurtainsHsmStates::RebootingDevice, SmartCurtainsHsmEvents::CONFIGURATION_FAILED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::PrepareDevice, SmartCurtainsHsmStates::ConnectingToWiFi, SmartCurtainsHsmEvents::DEVICE_READY);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::ResettingDevice, SmartCurtainsHsmStates::LoadConfiguration, SmartCurtainsHsmEvents::RESET_DONE);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::RebootingDevice, SmartCurtainsHsmStates::Final_3, SmartCurtainsHsmEvents::DONE);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::InitializingFeatures, SmartCurtainsHsmStates::Operable, SmartCurtainsHsmEvents::REGISTERED_WITH_HA);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::Operable, SmartCurtainsHsmEvents::HA_ONLINE, this, &SmartCurtainsHsmBase::onHomeAssistantAvailable);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::Operable, SmartCurtainsHsmEvents::HA_OFFLINE, this, &SmartCurtainsHsmBase::onHomeAssistantUnavailable);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::Operable, SmartCurtainsHsmEvents::POS_CHANGED, this, &SmartCurtainsHsmBase::onCurtainsPositionChanged);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::UpdatingFirmware, SmartCurtainsHsmEvents::UPDATE_STARTED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::UpdatingFirmware, SmartCurtainsHsmStates::Final_2, SmartCurtainsHsmEvents::UPDATE_FINISHED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::UpdatingFirmware, SmartCurtainsHsmStates::History_1, SmartCurtainsHsmEvents::UPDATE_FAILED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::UpdatingSystemTime, SmartCurtainsHsmStates::InitializingFeatures, SmartCurtainsHsmEvents::TIME_UPDATED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::UpdatingSystemTime, SmartCurtainsHsmStates::UpdatingSystemTime, SmartCurtainsHsmEvents::TIMER_CHECK_SYSTEM_TIME, this, &SmartCurtainsHsmBase::onValidateSystemTime);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::StartWiFIAccessPoint, SmartCurtainsHsmStates::StartConfigPortal, SmartCurtainsHsmEvents::AP_CONNECTED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::FinalizingInitialConfiguration, SmartCurtainsHsmStates::Final_6, SmartCurtainsHsmEvents::CONFIGURED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::WaitingForConfigRequest, SmartCurtainsHsmStates::ReceivingConfigParts, SmartCurtainsHsmEvents::CONFIGURATION_RECEIVED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::StartConfigPortal, SmartCurtainsHsmStates::StartWiFIAccessPoint, SmartCurtainsHsmEvents::AP_DISCONNECTED, this, &SmartCurtainsHsmBase::onStopConfigPortal);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::StartConfigPortal, SmartCurtainsHsmStates::WaitingForConfigRequest, SmartCurtainsHsmEvents::CONFIG_PORTAL_STARTED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::ReceivingConfigParts, SmartCurtainsHsmStates::WaitingForConfigRequest, SmartCurtainsHsmEvents::INVALID_CONFIG_REQUEST);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::ReceivingConfigParts, SmartCurtainsHsmStates::FinalizingInitialConfiguration, SmartCurtainsHsmEvents::CONFIGURATION_RECEIVED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::PrepareFrontend, SmartCurtainsHsmStates::HomeAssistantIntegration, SmartCurtainsHsmEvents::FRONTEND_READY);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::HomeAssistantIntegration, SmartCurtainsHsmStates::Final_1, SmartCurtainsHsmEvents::REGISTERED_WITH_HA);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::PreparingOTA, SmartCurtainsHsmStates::PrepareFrontend, SmartCurtainsHsmEvents::OTA_READY);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::LoadingLastPosition, SmartCurtainsHsmStates::FullyOpen, SmartCurtainsHsmEvents::OPERATION_DONE, this, nullptr, static_cast<SmartCurtainsHsmBaseTransitionConditionCallbackPtr_t>(&SmartCurtainsHsmBase::isFullyOpen), true);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::LoadingLastPosition, SmartCurtainsHsmStates::FullyClosed, SmartCurtainsHsmEvents::OPERATION_DONE, this, nullptr, static_cast<SmartCurtainsHsmBaseTransitionConditionCallbackPtr_t>(&SmartCurtainsHsmBase::isFullyClosed), true);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::LoadingLastPosition, SmartCurtainsHsmStates::Stopped, SmartCurtainsHsmEvents::OPERATION_DONE, this, nullptr, static_cast<SmartCurtainsHsmBaseTransitionConditionCallbackPtr_t>(&SmartCurtainsHsmBase::isPartiallyOpen), true);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::FullyOpen, SmartCurtainsHsmStates::Closing, SmartCurtainsHsmEvents::CLOSE_CURTAINS);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::FullyClosed, SmartCurtainsHsmStates::Opening, SmartCurtainsHsmEvents::OPEN_CURTAINS);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Opening, SmartCurtainsHsmStates::FullyOpen, SmartCurtainsHsmEvents::OPERATION_DONE, this, nullptr, static_cast<SmartCurtainsHsmBaseTransitionConditionCallbackPtr_t>(&SmartCurtainsHsmBase::isFullyOpen), true);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Opening, SmartCurtainsHsmStates::Closing, SmartCurtainsHsmEvents::CLOSE_CURTAINS);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Opening, SmartCurtainsHsmStates::Stopped, SmartCurtainsHsmEvents::OPERATION_DONE, this, nullptr, static_cast<SmartCurtainsHsmBaseTransitionConditionCallbackPtr_t>(&SmartCurtainsHsmBase::isFullyOpen), false);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Opening, SmartCurtainsHsmStates::Stopping, SmartCurtainsHsmEvents::STOP_CURTAINS);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Opening, SmartCurtainsHsmStates::OpenLimitReached, SmartCurtainsHsmEvents::OPEN_LIMIT_REACHED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Closing, SmartCurtainsHsmStates::FullyClosed, SmartCurtainsHsmEvents::OPERATION_DONE, this, nullptr, static_cast<SmartCurtainsHsmBaseTransitionConditionCallbackPtr_t>(&SmartCurtainsHsmBase::isFullyClosed), true);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Closing, SmartCurtainsHsmStates::Opening, SmartCurtainsHsmEvents::OPEN_CURTAINS);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Closing, SmartCurtainsHsmStates::Stopped, SmartCurtainsHsmEvents::OPERATION_DONE, this, nullptr, static_cast<SmartCurtainsHsmBaseTransitionConditionCallbackPtr_t>(&SmartCurtainsHsmBase::isFullyClosed), false);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Closing, SmartCurtainsHsmStates::Stopping, SmartCurtainsHsmEvents::STOP_CURTAINS);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Stopped, SmartCurtainsHsmStates::Opening, SmartCurtainsHsmEvents::OPEN_CURTAINS);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Stopped, SmartCurtainsHsmStates::Closing, SmartCurtainsHsmEvents::CLOSE_CURTAINS);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Stopping, SmartCurtainsHsmStates::Stopped, SmartCurtainsHsmEvents::OPERATION_DONE);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::OpenLimitReached, SmartCurtainsHsmStates::FullyOpen, SmartCurtainsHsmEvents::OPERATION_DONE);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::ReceivedConfigParts2, SmartCurtainsHsmStates::ReceivedConfigParts3, SmartCurtainsHsmEvents::CONFIGURATION_RECEIVED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::ReceivedConfigParts3, SmartCurtainsHsmStates::Final_5, SmartCurtainsHsmEvents::CONFIGURATION_RECEIVED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::ReceivedConfigParts1, SmartCurtainsHsmStates::ReceivedConfigParts2, SmartCurtainsHsmEvents::CONFIGURATION_RECEIVED);

    registerTimer(static_cast<hsmcpp::TimerID_t>(SmartCurtainsHsmTimers::CHECK_SYSTEM_TIME), SmartCurtainsHsmEvents::ON_TIMER_CHECK_SYSTEM_TIME);

    registerStateAction(SmartCurtainsHsmStates::UpdatingSystemTime, StateActionTrigger::ON_STATE_ENTRY, StateAction::START_TIMER, static_cast<hsmcpp::TimerID_t>(SmartCurtainsHsmTimers::CHECK_SYSTEM_TIME),  500,  false);
    registerStateAction(SmartCurtainsHsmStates::UpdatingSystemTime, StateActionTrigger::ON_STATE_EXIT, StateAction::STOP_TIMER, static_cast<hsmcpp::TimerID_t>(SmartCurtainsHsmTimers::CHECK_SYSTEM_TIME));
}

void SmartCurtainsHsmBase::onTransitionFailed(const SmartCurtainsHsmEvents event, const hsmcpp::VariantVector_t& args)
{
    // do nothing
}

std::string SmartCurtainsHsmBase::getStateName(const SmartCurtainsHsmStates state)
{
    std::string stateName;

#ifndef HSM_DISABLE_TRACES
    switch(state)
    {
        case SmartCurtainsHsmStates::LoadConfiguration:
            stateName = "LoadConfiguration";
            break;
        case SmartCurtainsHsmStates::ConnectingToWiFi:
            stateName = "ConnectingToWiFi";
            break;
        case SmartCurtainsHsmStates::NetworkAvailable:
            stateName = "NetworkAvailable";
            break;
        case SmartCurtainsHsmStates::Booting:
            stateName = "Booting";
            break;
        case SmartCurtainsHsmStates::WaitingInitialConfig:
            stateName = "WaitingInitialConfig";
            break;
        case SmartCurtainsHsmStates::PrepareDevice:
            stateName = "PrepareDevice";
            break;
        case SmartCurtainsHsmStates::ResettingDevice:
            stateName = "ResettingDevice";
            break;
        case SmartCurtainsHsmStates::RebootingDevice:
            stateName = "RebootingDevice";
            break;
        case SmartCurtainsHsmStates::Final_3:
            stateName = "Final_3";
            break;
        case SmartCurtainsHsmStates::InitializingFeatures:
            stateName = "InitializingFeatures";
            break;
        case SmartCurtainsHsmStates::Operable:
            stateName = "Operable";
            break;
        case SmartCurtainsHsmStates::UpdatingFirmware:
            stateName = "UpdatingFirmware";
            break;
        case SmartCurtainsHsmStates::UpdatingSystemTime:
            stateName = "UpdatingSystemTime";
            break;
        case SmartCurtainsHsmStates::Final_2:
            stateName = "Final_2";
            break;
        case SmartCurtainsHsmStates::StartWiFIAccessPoint:
            stateName = "StartWiFIAccessPoint";
            break;
        case SmartCurtainsHsmStates::FinalizingInitialConfiguration:
            stateName = "FinalizingInitialConfiguration";
            break;
        case SmartCurtainsHsmStates::WaitingForConfigRequest:
            stateName = "WaitingForConfigRequest";
            break;
        case SmartCurtainsHsmStates::StartConfigPortal:
            stateName = "StartConfigPortal";
            break;
        case SmartCurtainsHsmStates::ReceivingConfigParts:
            stateName = "ReceivingConfigParts";
            break;
        case SmartCurtainsHsmStates::Final_6:
            stateName = "Final_6";
            break;
        case SmartCurtainsHsmStates::PrepareFrontend:
            stateName = "PrepareFrontend";
            break;
        case SmartCurtainsHsmStates::HomeAssistantIntegration:
            stateName = "HomeAssistantIntegration";
            break;
        case SmartCurtainsHsmStates::PreparingOTA:
            stateName = "PreparingOTA";
            break;
        case SmartCurtainsHsmStates::Final_1:
            stateName = "Final_1";
            break;
        case SmartCurtainsHsmStates::LoadingLastPosition:
            stateName = "LoadingLastPosition";
            break;
        case SmartCurtainsHsmStates::FullyOpen:
            stateName = "FullyOpen";
            break;
        case SmartCurtainsHsmStates::FullyClosed:
            stateName = "FullyClosed";
            break;
        case SmartCurtainsHsmStates::Opening:
            stateName = "Opening";
            break;
        case SmartCurtainsHsmStates::Closing:
            stateName = "Closing";
            break;
        case SmartCurtainsHsmStates::Stopped:
            stateName = "Stopped";
            break;
        case SmartCurtainsHsmStates::Stopping:
            stateName = "Stopping";
            break;
        case SmartCurtainsHsmStates::OpenLimitReached:
            stateName = "OpenLimitReached";
            break;
        case SmartCurtainsHsmStates::History_1:
            stateName = "History_1";
            break;
        case SmartCurtainsHsmStates::ReceivedConfigParts2:
            stateName = "ReceivedConfigParts2";
            break;
        case SmartCurtainsHsmStates::ReceivedConfigParts3:
            stateName = "ReceivedConfigParts3";
            break;
        case SmartCurtainsHsmStates::ReceivedConfigParts1:
            stateName = "ReceivedConfigParts1";
            break;
        case SmartCurtainsHsmStates::Final_5:
            stateName = "Final_5";
            break;
        default:
            stateName = hsmcpp::HierarchicalStateMachine<SmartCurtainsHsmStates, SmartCurtainsHsmEvents>::getStateName(state);
            break;
    }
#endif // HSM_DISABLE_TRACES

    return stateName;
}

std::string SmartCurtainsHsmBase::getEventName(const SmartCurtainsHsmEvents event)
{
    std::string eventName;
    
#ifndef HSM_DISABLE_TRACES
    switch(event)
    {
        case SmartCurtainsHsmEvents::CLOSE_CURTAINS:
            eventName = "CLOSE_CURTAINS";
            break;
        case SmartCurtainsHsmEvents::REBOOT_DEVICE:
            eventName = "REBOOT_DEVICE";
            break;
        case SmartCurtainsHsmEvents::CONFIGURATION_FAILED:
            eventName = "CONFIGURATION_FAILED";
            break;
        case SmartCurtainsHsmEvents::AP_DISCONNECTED:
            eventName = "AP_DISCONNECTED";
            break;
        case SmartCurtainsHsmEvents::TIME_UPDATED:
            eventName = "TIME_UPDATED";
            break;
        case SmartCurtainsHsmEvents::CONFIG_NOT_AVAILABLE:
            eventName = "CONFIG_NOT_AVAILABLE";
            break;
        case SmartCurtainsHsmEvents::DEVICE_READY:
            eventName = "DEVICE_READY";
            break;
        case SmartCurtainsHsmEvents::OTA_READY:
            eventName = "OTA_READY";
            break;
        case SmartCurtainsHsmEvents::DONE:
            eventName = "DONE";
            break;
        case SmartCurtainsHsmEvents::POS_CHANGED:
            eventName = "POS_CHANGED";
            break;
        case SmartCurtainsHsmEvents::STOP_CURTAINS:
            eventName = "STOP_CURTAINS";
            break;
        case SmartCurtainsHsmEvents::OPEN_CURTAINS:
            eventName = "OPEN_CURTAINS";
            break;
        case SmartCurtainsHsmEvents::HA_OFFLINE:
            eventName = "HA_OFFLINE";
            break;
        case SmartCurtainsHsmEvents::BOOTING_FINISHED:
            eventName = "BOOTING_FINISHED";
            break;
        case SmartCurtainsHsmEvents::FRONTEND_READY:
            eventName = "FRONTEND_READY";
            break;
        case SmartCurtainsHsmEvents::ON_TIMER_CHECK_SYSTEM_TIME:
            eventName = "ON_TIMER_CHECK_SYSTEM_TIME";
            break;
        case SmartCurtainsHsmEvents::UPDATE_STARTED:
            eventName = "UPDATE_STARTED";
            break;
        case SmartCurtainsHsmEvents::REGISTERED_WITH_HA:
            eventName = "REGISTERED_WITH_HA";
            break;
        case SmartCurtainsHsmEvents::RESET_DONE:
            eventName = "RESET_DONE";
            break;
        case SmartCurtainsHsmEvents::RESET_DEVICE:
            eventName = "RESET_DEVICE";
            break;
        case SmartCurtainsHsmEvents::OPERATION_DONE:
            eventName = "OPERATION_DONE";
            break;
        case SmartCurtainsHsmEvents::HA_ONLINE:
            eventName = "HA_ONLINE";
            break;
        case SmartCurtainsHsmEvents::WIFI_DISCONNECTED:
            eventName = "WIFI_DISCONNECTED";
            break;
        case SmartCurtainsHsmEvents::UPDATE_FAILED:
            eventName = "UPDATE_FAILED";
            break;
        case SmartCurtainsHsmEvents::OPEN_LIMIT_REACHED:
            eventName = "OPEN_LIMIT_REACHED";
            break;
        case SmartCurtainsHsmEvents::UPDATE_FINISHED:
            eventName = "UPDATE_FINISHED";
            break;
        case SmartCurtainsHsmEvents::CONFIGURED:
            eventName = "CONFIGURED";
            break;
        case SmartCurtainsHsmEvents::CONFIGURATION_RECEIVED:
            eventName = "CONFIGURATION_RECEIVED";
            break;
        case SmartCurtainsHsmEvents::TIMER_CHECK_SYSTEM_TIME:
            eventName = "TIMER_CHECK_SYSTEM_TIME";
            break;
        case SmartCurtainsHsmEvents::WIFI_CONNECTED:
            eventName = "WIFI_CONNECTED";
            break;
        case SmartCurtainsHsmEvents::INVALID_CONFIG_REQUEST:
            eventName = "INVALID_CONFIG_REQUEST";
            break;
        case SmartCurtainsHsmEvents::CONFIG_PORTAL_STARTED:
            eventName = "CONFIG_PORTAL_STARTED";
            break;
        case SmartCurtainsHsmEvents::AP_CONNECTED:
            eventName = "AP_CONNECTED";
            break;
        default:
            eventName = hsmcpp::HierarchicalStateMachine<SmartCurtainsHsmStates, SmartCurtainsHsmEvents>::getEventName(event);
            break;
    }
#endif // HSM_DISABLE_TRACES

    return eventName;
}