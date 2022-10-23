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
    UpdatingFirmware,
    RebootingDevice,
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
    OpenLimitReached,
    History_1,
};

enum class SmartCurtainsHsmEvents
{
    HA_OFFLINE,
    OPEN_LIMIT_REACHED,
    HA_ONLINE,
    UPDATE_FAILED,
    WIFI_CONNECTED,
    DEVICE_READY,
    WIFI_DISCONNECTED,
    OPEN_CURTAINS,
    RESET_DONE,
    STOP_CURTAINS,
    OPERATION_DONE,
    UPDATE_FINISHED,
    CONFIG_NOT_AVAILABLE,
    UPDATE_STARTED,
    REGISTERED_WITH_HA,
    FRONTEND_READY,
    POS_CHANGED,
    RESET_DEVICE,
    CONFIGURED,
    CLOSE_CURTAINS,
    BOOTING_FINISHED,
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
    virtual void onLoadConfiguration(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onResetDevice(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onPrepareDevice(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStateStoppedCurtains(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStateClosedCurtains(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStateFullyOpenCurtains(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onLoadLastPosition(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStateOpeningCurtains(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onRebootDevice(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onSetupConfigPortal(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStateClosingCurtains(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStartOtaManager(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onRequestStopMotor(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStartWebFrontend(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onBeginHaRegistration(const hsmcpp::VariantVector_t& args) = 0;

// HSM state entering callbacks
protected:

// HSM state exiting callbacks
protected:
    virtual bool onStopWebFrontend() = 0;
    virtual bool onRequestStopMotor() = 0;

// HSM transition callbacks
protected:
    // NOTE: override this method in child class if needed
    virtual void onTransitionFailed(const SmartCurtainsHsmEvents event, const hsmcpp::VariantVector_t& args);

    virtual void onCurtainsPositionChanged(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onHomeAssistantAvailable(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onHomeAssistantUnavailable(const hsmcpp::VariantVector_t& args) = 0;

// HSM transition condition callbacks
protected:
    virtual bool isPartiallyOpen(const hsmcpp::VariantVector_t& args) = 0;
    virtual bool isFullyOpen(const hsmcpp::VariantVector_t& args) = 0;
    virtual bool isFullyClosed(const hsmcpp::VariantVector_t& args) = 0;

protected:
    std::string getStateName(const SmartCurtainsHsmStates state) override;
    std::string getEventName(const SmartCurtainsHsmEvents event) override;
};

#endif // __GEN_HSM_SMARTCURTAINSHSMBASE__