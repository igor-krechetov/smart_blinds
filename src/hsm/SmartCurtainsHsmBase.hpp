// Content of this file was generated

#ifndef __GEN_HSM_SMARTCURTAINSHSMBASE__
#define __GEN_HSM_SMARTCURTAINSHSMBASE__

#include <hsmcpp/hsm.hpp>

enum class SmartCurtainsHsmStates
{
    LoadConfiguration,
    ConnectingToWiFi,
    NetworkAvailable,
    Booting,
    WaitingInitialConfig,
    PrepareDevice,
    ResettingDevice,
    PreparingUI,
    Operable,
    PrepareFrontend,
    HomeAssistantIntegration,
    Final_1,
    LoadingLastPosition,
    FullyOpen,
    FullyClosed,
    Opening,
    Closing,
    Stopped,
    Stopping,
};

enum class SmartCurtainsHsmEvents
{
    HA_OFFLINE,
    DEVICE_READY,
    CONFIGURED,
    OPEN_CURTAINS,
    CLOSE_CURTAINS,
    WIFI_CONNECTED,
    HA_ONLINE,
    FRONTEND_READY,
    BOOTING_FINISHED,
    WIFI_DISCONNECTED,
    REGISTERED_WITH_HA,
    RESET_DEVICE,
    RESET_DONE,
    CONFIG_NOT_AVAILABLE,
    POS_CHANGED,
    STOP_CURTAINS,
    OPERATION_DONE,
};

enum class SmartCurtainsHsmTimers
{
};

class SmartCurtainsHsmBase: public hsmcpp::HierarchicalStateMachine<SmartCurtainsHsmStates, SmartCurtainsHsmEvents>
{
    using SmartCurtainsHsmBaseTransitionCallbackPtr_t           = void (SmartCurtainsHsmBase::*)(const hsmcpp::VariantVector_t&);
    using SmartCurtainsHsmBaseTransitionConditionCallbackPtr_t  = bool (SmartCurtainsHsmBase::*)(const hsmcpp::VariantVector_t&);
    using SmartCurtainsHsmBaseStateChangedCallbackPtr_t         = void (SmartCurtainsHsmBase::*)(const hsmcpp::VariantVector_t&);
    using SmartCurtainsHsmBaseStateEnterCallbackPtr_t           = bool (SmartCurtainsHsmBase::*)(const hsmcpp::VariantVector_t&);
    using SmartCurtainsHsmBaseStateExitCallbackPtr_t            = bool (SmartCurtainsHsmBase::*)();
    using SmartCurtainsHsmBaseTransitionFailedCallbackPtr_t     = void (SmartCurtainsHsmBase::*)(const SmartCurtainsHsmEvents, const hsmcpp::VariantVector_t&);

public:
    SmartCurtainsHsmBase();
    virtual ~SmartCurtainsHsmBase();

protected:
    void configureHsm();

// HSM state changed callbacks
protected:
    virtual void onStateClosingCurtains(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onLoadLastPosition(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onPrepareDevice(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onResetDevice(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onSetupConfigPortal(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onLoadConfiguration(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onBeginHaRegistration(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStateClosedCurtains(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStateStoppedCurtains(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStateOpeningCurtains(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStateStoppingCurtains(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStartWebFrontend(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStateOpenCurtains(const hsmcpp::VariantVector_t& args) = 0;

// HSM state entering callbacks
protected:

// HSM state exiting callbacks
protected:
    virtual bool onStopWebFrontend() = 0;

// HSM transition callbacks
protected:
    // NOTE: override this method in child class if needed
    virtual void onTransitionFailed(const SmartCurtainsHsmEvents event, const hsmcpp::VariantVector_t& args);

    virtual void onCurtainsPositionChanged(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onHomeAssistantUnavailable(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onHomeAssistantAvailable(const hsmcpp::VariantVector_t& args) = 0;

// HSM transition condition callbacks
protected:
    virtual bool isFullyClosed(const hsmcpp::VariantVector_t& args) = 0;
    virtual bool isPartiallyOpen(const hsmcpp::VariantVector_t& args) = 0;
    virtual bool isFullyOpen(const hsmcpp::VariantVector_t& args) = 0;

protected:
    std::string getStateName(const SmartCurtainsHsmStates state) override;
    std::string getEventName(const SmartCurtainsHsmEvents event) override;
};

#endif // __GEN_HSM_SMARTCURTAINSHSMBASE__