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
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::NetworkAvailable, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onStartOtaManager), nullptr, static_cast<SmartCurtainsHsmBaseStateExitCallbackPtr_t>(&SmartCurtainsHsmBase::onStopWebFrontend));
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Booting);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::WaitingInitialConfig, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onSetupConfigPortal), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::PrepareDevice, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onPrepareDevice), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::ResettingDevice, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onResetDevice), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::PreparingUI);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Operable, this, nullptr, nullptr, static_cast<SmartCurtainsHsmBaseStateExitCallbackPtr_t>(&SmartCurtainsHsmBase::onRequestStopMotor));
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::UpdatingFirmware);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::RebootingDevice, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onRebootDevice), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::PrepareFrontend, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onStartWebFrontend), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::HomeAssistantIntegration, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onBeginHaRegistration), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Final_1);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::LoadingLastPosition, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onLoadLastPosition), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::FullyOpen, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onStateFullyOpenCurtains), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::FullyClosed, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onStateClosedCurtains), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Opening, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onStateOpeningCurtains), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Closing, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onStateClosingCurtains), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Stopped, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onStateStoppedCurtains), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Stopping, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onRequestStopMotor), nullptr, nullptr);
    registerState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::OpenLimitReached, this, static_cast<SmartCurtainsHsmBaseStateChangedCallbackPtr_t>(&SmartCurtainsHsmBase::onRequestStopMotor), nullptr, nullptr);

    registerSubstateEntryPoint(SmartCurtainsHsmStates::NetworkAvailable, SmartCurtainsHsmStates::PreparingUI);
    registerSubstate(SmartCurtainsHsmStates::NetworkAvailable, SmartCurtainsHsmStates::Operable);
    registerSubstate(SmartCurtainsHsmStates::NetworkAvailable, SmartCurtainsHsmStates::UpdatingFirmware);
    registerSubstate(SmartCurtainsHsmStates::NetworkAvailable, SmartCurtainsHsmStates::RebootingDevice);
    registerSubstateEntryPoint(SmartCurtainsHsmStates::PreparingUI, SmartCurtainsHsmStates::PrepareFrontend);
    registerSubstate(SmartCurtainsHsmStates::PreparingUI, SmartCurtainsHsmStates::HomeAssistantIntegration);
    registerSubstate(SmartCurtainsHsmStates::PreparingUI, SmartCurtainsHsmStates::Final_1);
    registerSubstateEntryPoint(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::LoadingLastPosition);
    registerSubstate(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::FullyOpen);
    registerSubstate(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::FullyClosed);
    registerSubstate(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::Opening);
    registerSubstate(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::Closing);
    registerSubstate(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::Stopped);
    registerSubstate(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::Stopping);
    registerSubstate(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::OpenLimitReached);
    registerHistory(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::History_1, HistoryType::SHALLOW);

    registerFinalState<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Final_1, SmartCurtainsHsmEvents::REGISTERED_WITH_HA);

    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::LoadConfiguration, SmartCurtainsHsmStates::PrepareDevice, SmartCurtainsHsmEvents::CONFIGURED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::LoadConfiguration, SmartCurtainsHsmStates::WaitingInitialConfig, SmartCurtainsHsmEvents::CONFIG_NOT_AVAILABLE);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::ConnectingToWiFi, SmartCurtainsHsmStates::NetworkAvailable, SmartCurtainsHsmEvents::WIFI_CONNECTED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::NetworkAvailable, SmartCurtainsHsmStates::ResettingDevice, SmartCurtainsHsmEvents::RESET_DEVICE);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::NetworkAvailable, SmartCurtainsHsmStates::ConnectingToWiFi, SmartCurtainsHsmEvents::WIFI_DISCONNECTED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Booting, SmartCurtainsHsmStates::LoadConfiguration, SmartCurtainsHsmEvents::BOOTING_FINISHED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::WaitingInitialConfig, SmartCurtainsHsmStates::PrepareDevice, SmartCurtainsHsmEvents::CONFIGURED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::PrepareDevice, SmartCurtainsHsmStates::ConnectingToWiFi, SmartCurtainsHsmEvents::DEVICE_READY);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::ResettingDevice, SmartCurtainsHsmStates::LoadConfiguration, SmartCurtainsHsmEvents::RESET_DONE);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::PreparingUI, SmartCurtainsHsmStates::Operable, SmartCurtainsHsmEvents::REGISTERED_WITH_HA);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::Operable, SmartCurtainsHsmEvents::HA_ONLINE, this, &SmartCurtainsHsmBase::onHomeAssistantAvailable);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::Operable, SmartCurtainsHsmEvents::HA_OFFLINE, this, &SmartCurtainsHsmBase::onHomeAssistantUnavailable);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::Operable, SmartCurtainsHsmEvents::POS_CHANGED, this, &SmartCurtainsHsmBase::onCurtainsPositionChanged);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::Operable, SmartCurtainsHsmStates::UpdatingFirmware, SmartCurtainsHsmEvents::UPDATE_STARTED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::UpdatingFirmware, SmartCurtainsHsmStates::RebootingDevice, SmartCurtainsHsmEvents::UPDATE_FINISHED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::UpdatingFirmware, SmartCurtainsHsmStates::History_1, SmartCurtainsHsmEvents::UPDATE_FAILED);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::PrepareFrontend, SmartCurtainsHsmStates::HomeAssistantIntegration, SmartCurtainsHsmEvents::FRONTEND_READY);
    registerTransition<SmartCurtainsHsmBase>(SmartCurtainsHsmStates::HomeAssistantIntegration, SmartCurtainsHsmStates::Final_1, SmartCurtainsHsmEvents::REGISTERED_WITH_HA);
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


}

void SmartCurtainsHsmBase::onTransitionFailed(const SmartCurtainsHsmEvents event, const hsmcpp::VariantVector_t& args)
{
    // do nothing
}

std::string SmartCurtainsHsmBase::getStateName(const SmartCurtainsHsmStates state)
{
    std::string stateName;

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
        case SmartCurtainsHsmStates::PreparingUI:
            stateName = "PreparingUI";
            break;
        case SmartCurtainsHsmStates::Operable:
            stateName = "Operable";
            break;
        case SmartCurtainsHsmStates::UpdatingFirmware:
            stateName = "UpdatingFirmware";
            break;
        case SmartCurtainsHsmStates::RebootingDevice:
            stateName = "RebootingDevice";
            break;
        case SmartCurtainsHsmStates::PrepareFrontend:
            stateName = "PrepareFrontend";
            break;
        case SmartCurtainsHsmStates::HomeAssistantIntegration:
            stateName = "HomeAssistantIntegration";
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
        default:
            stateName = hsmcpp::HierarchicalStateMachine<SmartCurtainsHsmStates, SmartCurtainsHsmEvents>::getStateName(state);
            break;
    }

    return stateName;
}

std::string SmartCurtainsHsmBase::getEventName(const SmartCurtainsHsmEvents event)
{
    std::string eventName;

    switch(event)
    {
        case SmartCurtainsHsmEvents::HA_OFFLINE:
            eventName = "HA_OFFLINE";
            break;
        case SmartCurtainsHsmEvents::OPEN_LIMIT_REACHED:
            eventName = "OPEN_LIMIT_REACHED";
            break;
        case SmartCurtainsHsmEvents::HA_ONLINE:
            eventName = "HA_ONLINE";
            break;
        case SmartCurtainsHsmEvents::UPDATE_FAILED:
            eventName = "UPDATE_FAILED";
            break;
        case SmartCurtainsHsmEvents::WIFI_CONNECTED:
            eventName = "WIFI_CONNECTED";
            break;
        case SmartCurtainsHsmEvents::DEVICE_READY:
            eventName = "DEVICE_READY";
            break;
        case SmartCurtainsHsmEvents::WIFI_DISCONNECTED:
            eventName = "WIFI_DISCONNECTED";
            break;
        case SmartCurtainsHsmEvents::OPEN_CURTAINS:
            eventName = "OPEN_CURTAINS";
            break;
        case SmartCurtainsHsmEvents::RESET_DONE:
            eventName = "RESET_DONE";
            break;
        case SmartCurtainsHsmEvents::STOP_CURTAINS:
            eventName = "STOP_CURTAINS";
            break;
        case SmartCurtainsHsmEvents::OPERATION_DONE:
            eventName = "OPERATION_DONE";
            break;
        case SmartCurtainsHsmEvents::UPDATE_FINISHED:
            eventName = "UPDATE_FINISHED";
            break;
        case SmartCurtainsHsmEvents::CONFIG_NOT_AVAILABLE:
            eventName = "CONFIG_NOT_AVAILABLE";
            break;
        case SmartCurtainsHsmEvents::UPDATE_STARTED:
            eventName = "UPDATE_STARTED";
            break;
        case SmartCurtainsHsmEvents::REGISTERED_WITH_HA:
            eventName = "REGISTERED_WITH_HA";
            break;
        case SmartCurtainsHsmEvents::FRONTEND_READY:
            eventName = "FRONTEND_READY";
            break;
        case SmartCurtainsHsmEvents::POS_CHANGED:
            eventName = "POS_CHANGED";
            break;
        case SmartCurtainsHsmEvents::RESET_DEVICE:
            eventName = "RESET_DEVICE";
            break;
        case SmartCurtainsHsmEvents::CONFIGURED:
            eventName = "CONFIGURED";
            break;
        case SmartCurtainsHsmEvents::CLOSE_CURTAINS:
            eventName = "CLOSE_CURTAINS";
            break;
        case SmartCurtainsHsmEvents::BOOTING_FINISHED:
            eventName = "BOOTING_FINISHED";
            break;
        default:
            eventName = hsmcpp::HierarchicalStateMachine<SmartCurtainsHsmStates, SmartCurtainsHsmEvents>::getEventName(event);
            break;
    }

    return eventName;
}