// Content of this file was generated

#ifndef __GEN_HSM_SMARTBLINDSHSMBASE__
#define __GEN_HSM_SMARTBLINDSHSMBASE__

#include <hsmcpp/hsm.hpp>

enum class SmartBlindsHsmStates
{
    LoadConfiguration,
    ConnectingToWiFi,
    Operable,
    Booting,
    WaitingInitialConfig,
    PrepareDevice,
    WaitingForCommands,
    Opening,
    Closing,
    PrepareFrontend,
    HomeAssistantIntegration,
};

enum class SmartBlindsHsmEvents
{
    WIFI_CONNECTED,
    CLOSE_CURTAINS,
    CONFIG_NOT_AVAILABLE,
    REGISTERED_WITH_HA,
    CONFIGURED,
    CHECK_WIFI_CONNECTION,
    RESET_DEVICE,
    OPEN_CURTAINS,
    WIFI_DISCONNECTED,
    BOOTING_FINISHED,
    FRONTEND_READY,
    DEVICE_READY,
    OPERATION_DONE,
};

enum class SmartBlindsHsmTimers
{
};

class SmartBlindsHsmBase: public hsmcpp::HierarchicalStateMachine<SmartBlindsHsmStates, SmartBlindsHsmEvents>
{
    using SmartBlindsHsmBaseTransitionCallbackPtr_t           = void (SmartBlindsHsmBase::*)(const hsmcpp::VariantVector_t&);
    using SmartBlindsHsmBaseTransitionConditionCallbackPtr_t  = bool (SmartBlindsHsmBase::*)(const hsmcpp::VariantVector_t&);
    using SmartBlindsHsmBaseStateChangedCallbackPtr_t         = void (SmartBlindsHsmBase::*)(const hsmcpp::VariantVector_t&);
    using SmartBlindsHsmBaseStateEnterCallbackPtr_t           = bool (SmartBlindsHsmBase::*)(const hsmcpp::VariantVector_t&);
    using SmartBlindsHsmBaseStateExitCallbackPtr_t            = bool (SmartBlindsHsmBase::*)();
    using SmartBlindsHsmBaseTransitionFailedCallbackPtr_t     = void (SmartBlindsHsmBase::*)(const SmartBlindsHsmEvents, const hsmcpp::VariantVector_t&);

public:
    SmartBlindsHsmBase();
    virtual ~SmartBlindsHsmBase();

protected:
    void configureHsm();

// HSM state changed callbacks
protected:
    virtual void onOpenCurtains(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onCloseCurtains(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onLoadConfiguration(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onBeginHaRegistration(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onSetupConfigPortal(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStartWebFrontend(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onPrepareDevice(const hsmcpp::VariantVector_t& args) = 0;

// HSM state entering callbacks
protected:

// HSM state exiting callbacks
protected:

// HSM transition callbacks
protected:
    // NOTE: override this method in child class if needed
    virtual void onTransitionFailed(const SmartBlindsHsmEvents event, const hsmcpp::VariantVector_t& args);


// HSM transition condition callbacks
protected:

protected:
    std::string getStateName(const SmartBlindsHsmStates state) override;
    std::string getEventName(const SmartBlindsHsmEvents event) override;
};

#endif // __GEN_HSM_SMARTBLINDSHSMBASE__