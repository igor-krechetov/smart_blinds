// Content of this file was generated

#include "SmartBlindsHsmBase.hpp"

SmartBlindsHsmBase::SmartBlindsHsmBase()
    : hsmcpp::HierarchicalStateMachine<SmartBlindsHsmStates, SmartBlindsHsmEvents>(SmartBlindsHsmStates::Booting)
{
    configureHsm();
}

SmartBlindsHsmBase::~SmartBlindsHsmBase()
{}

void SmartBlindsHsmBase::configureHsm()
{
    registerFailedTransitionCallback<SmartBlindsHsmBase>(this, &SmartBlindsHsmBase::onTransitionFailed);

    registerState<SmartBlindsHsmBase>(SmartBlindsHsmStates::LoadConfiguration, this, static_cast<SmartBlindsHsmBaseStateChangedCallbackPtr_t>(&SmartBlindsHsmBase::onLoadConfiguration), nullptr, nullptr);
    registerState<SmartBlindsHsmBase>(SmartBlindsHsmStates::ConnectingToWiFi);
    registerState<SmartBlindsHsmBase>(SmartBlindsHsmStates::Operable);
    registerState<SmartBlindsHsmBase>(SmartBlindsHsmStates::Booting);
    registerState<SmartBlindsHsmBase>(SmartBlindsHsmStates::WaitingInitialConfig, this, static_cast<SmartBlindsHsmBaseStateChangedCallbackPtr_t>(&SmartBlindsHsmBase::onSetupConfigPortal), nullptr, nullptr);
    registerState<SmartBlindsHsmBase>(SmartBlindsHsmStates::PrepareDevice, this, static_cast<SmartBlindsHsmBaseStateChangedCallbackPtr_t>(&SmartBlindsHsmBase::onPrepareDevice), nullptr, nullptr);
    registerState<SmartBlindsHsmBase>(SmartBlindsHsmStates::WaitingForCommands);
    registerState<SmartBlindsHsmBase>(SmartBlindsHsmStates::Opening, this, static_cast<SmartBlindsHsmBaseStateChangedCallbackPtr_t>(&SmartBlindsHsmBase::onOpenCurtains), nullptr, nullptr);
    registerState<SmartBlindsHsmBase>(SmartBlindsHsmStates::Closing, this, static_cast<SmartBlindsHsmBaseStateChangedCallbackPtr_t>(&SmartBlindsHsmBase::onCloseCurtains), nullptr, nullptr);
    registerState<SmartBlindsHsmBase>(SmartBlindsHsmStates::PrepareFrontend, this, static_cast<SmartBlindsHsmBaseStateChangedCallbackPtr_t>(&SmartBlindsHsmBase::onStartWebFrontend), nullptr, nullptr);
    registerState<SmartBlindsHsmBase>(SmartBlindsHsmStates::HomeAssistantIntegration, this, static_cast<SmartBlindsHsmBaseStateChangedCallbackPtr_t>(&SmartBlindsHsmBase::onBeginHaRegistration), nullptr, nullptr);

    registerSubstateEntryPoint(SmartBlindsHsmStates::Operable, SmartBlindsHsmStates::PrepareFrontend);
    registerSubstate(SmartBlindsHsmStates::Operable, SmartBlindsHsmStates::WaitingForCommands);
    registerSubstate(SmartBlindsHsmStates::Operable, SmartBlindsHsmStates::Opening);
    registerSubstate(SmartBlindsHsmStates::Operable, SmartBlindsHsmStates::Closing);
    registerSubstate(SmartBlindsHsmStates::Operable, SmartBlindsHsmStates::HomeAssistantIntegration);

    registerTransition<SmartBlindsHsmBase>(SmartBlindsHsmStates::LoadConfiguration, SmartBlindsHsmStates::PrepareDevice, SmartBlindsHsmEvents::CONFIGURED);
    registerTransition<SmartBlindsHsmBase>(SmartBlindsHsmStates::LoadConfiguration, SmartBlindsHsmStates::WaitingInitialConfig, SmartBlindsHsmEvents::CONFIG_NOT_AVAILABLE);
    registerTransition<SmartBlindsHsmBase>(SmartBlindsHsmStates::ConnectingToWiFi, SmartBlindsHsmStates::Operable, SmartBlindsHsmEvents::WIFI_CONNECTED);
    registerTransition<SmartBlindsHsmBase>(SmartBlindsHsmStates::ConnectingToWiFi, SmartBlindsHsmStates::ConnectingToWiFi, SmartBlindsHsmEvents::CHECK_WIFI_CONNECTION);
    registerTransition<SmartBlindsHsmBase>(SmartBlindsHsmStates::Operable, SmartBlindsHsmStates::LoadConfiguration, SmartBlindsHsmEvents::RESET_DEVICE);
    registerTransition<SmartBlindsHsmBase>(SmartBlindsHsmStates::Operable, SmartBlindsHsmStates::ConnectingToWiFi, SmartBlindsHsmEvents::WIFI_DISCONNECTED);
    registerTransition<SmartBlindsHsmBase>(SmartBlindsHsmStates::Booting, SmartBlindsHsmStates::LoadConfiguration, SmartBlindsHsmEvents::BOOTING_FINISHED);
    registerTransition<SmartBlindsHsmBase>(SmartBlindsHsmStates::WaitingInitialConfig, SmartBlindsHsmStates::PrepareDevice, SmartBlindsHsmEvents::CONFIGURED);
    registerTransition<SmartBlindsHsmBase>(SmartBlindsHsmStates::PrepareDevice, SmartBlindsHsmStates::ConnectingToWiFi, SmartBlindsHsmEvents::DEVICE_READY);
    registerTransition<SmartBlindsHsmBase>(SmartBlindsHsmStates::WaitingForCommands, SmartBlindsHsmStates::Opening, SmartBlindsHsmEvents::OPEN_CURTAINS);
    registerTransition<SmartBlindsHsmBase>(SmartBlindsHsmStates::WaitingForCommands, SmartBlindsHsmStates::Closing, SmartBlindsHsmEvents::CLOSE_CURTAINS);
    registerTransition<SmartBlindsHsmBase>(SmartBlindsHsmStates::Opening, SmartBlindsHsmStates::Closing, SmartBlindsHsmEvents::CLOSE_CURTAINS);
    registerTransition<SmartBlindsHsmBase>(SmartBlindsHsmStates::Opening, SmartBlindsHsmStates::WaitingForCommands, SmartBlindsHsmEvents::OPERATION_DONE);
    registerTransition<SmartBlindsHsmBase>(SmartBlindsHsmStates::Closing, SmartBlindsHsmStates::WaitingForCommands, SmartBlindsHsmEvents::OPERATION_DONE);
    registerTransition<SmartBlindsHsmBase>(SmartBlindsHsmStates::Closing, SmartBlindsHsmStates::Opening, SmartBlindsHsmEvents::OPEN_CURTAINS);
    registerTransition<SmartBlindsHsmBase>(SmartBlindsHsmStates::PrepareFrontend, SmartBlindsHsmStates::HomeAssistantIntegration, SmartBlindsHsmEvents::FRONTEND_READY);
    registerTransition<SmartBlindsHsmBase>(SmartBlindsHsmStates::PrepareFrontend, SmartBlindsHsmStates::WaitingForCommands, SmartBlindsHsmEvents::FRONTEND_READY);
    registerTransition<SmartBlindsHsmBase>(SmartBlindsHsmStates::HomeAssistantIntegration, SmartBlindsHsmStates::WaitingForCommands, SmartBlindsHsmEvents::REGISTERED_WITH_HA);


}

void SmartBlindsHsmBase::onTransitionFailed(const SmartBlindsHsmEvents event, const hsmcpp::VariantVector_t& args)
{
    // do nothing
}

std::string SmartBlindsHsmBase::getStateName(const SmartBlindsHsmStates state)
{
    std::string stateName;

    switch(state)
    {
        case SmartBlindsHsmStates::LoadConfiguration:
            stateName = "LoadConfiguration";
            break;
        case SmartBlindsHsmStates::ConnectingToWiFi:
            stateName = "ConnectingToWiFi";
            break;
        case SmartBlindsHsmStates::Operable:
            stateName = "Operable";
            break;
        case SmartBlindsHsmStates::Booting:
            stateName = "Booting";
            break;
        case SmartBlindsHsmStates::WaitingInitialConfig:
            stateName = "WaitingInitialConfig";
            break;
        case SmartBlindsHsmStates::PrepareDevice:
            stateName = "PrepareDevice";
            break;
        case SmartBlindsHsmStates::WaitingForCommands:
            stateName = "WaitingForCommands";
            break;
        case SmartBlindsHsmStates::Opening:
            stateName = "Opening";
            break;
        case SmartBlindsHsmStates::Closing:
            stateName = "Closing";
            break;
        case SmartBlindsHsmStates::PrepareFrontend:
            stateName = "PrepareFrontend";
            break;
        case SmartBlindsHsmStates::HomeAssistantIntegration:
            stateName = "HomeAssistantIntegration";
            break;
        default:
            stateName = hsmcpp::HierarchicalStateMachine<SmartBlindsHsmStates, SmartBlindsHsmEvents>::getStateName(state);
            break;
    }

    return stateName;
}

std::string SmartBlindsHsmBase::getEventName(const SmartBlindsHsmEvents event)
{
    std::string eventName;

    switch(event)
    {
        case SmartBlindsHsmEvents::WIFI_CONNECTED:
            eventName = "WIFI_CONNECTED";
            break;
        case SmartBlindsHsmEvents::CLOSE_CURTAINS:
            eventName = "CLOSE_CURTAINS";
            break;
        case SmartBlindsHsmEvents::CONFIG_NOT_AVAILABLE:
            eventName = "CONFIG_NOT_AVAILABLE";
            break;
        case SmartBlindsHsmEvents::REGISTERED_WITH_HA:
            eventName = "REGISTERED_WITH_HA";
            break;
        case SmartBlindsHsmEvents::CONFIGURED:
            eventName = "CONFIGURED";
            break;
        case SmartBlindsHsmEvents::CHECK_WIFI_CONNECTION:
            eventName = "CHECK_WIFI_CONNECTION";
            break;
        case SmartBlindsHsmEvents::RESET_DEVICE:
            eventName = "RESET_DEVICE";
            break;
        case SmartBlindsHsmEvents::OPEN_CURTAINS:
            eventName = "OPEN_CURTAINS";
            break;
        case SmartBlindsHsmEvents::WIFI_DISCONNECTED:
            eventName = "WIFI_DISCONNECTED";
            break;
        case SmartBlindsHsmEvents::BOOTING_FINISHED:
            eventName = "BOOTING_FINISHED";
            break;
        case SmartBlindsHsmEvents::FRONTEND_READY:
            eventName = "FRONTEND_READY";
            break;
        case SmartBlindsHsmEvents::DEVICE_READY:
            eventName = "DEVICE_READY";
            break;
        case SmartBlindsHsmEvents::OPERATION_DONE:
            eventName = "OPERATION_DONE";
            break;
        default:
            eventName = hsmcpp::HierarchicalStateMachine<SmartBlindsHsmStates, SmartBlindsHsmEvents>::getEventName(event);
            break;
    }

    return eventName;
}