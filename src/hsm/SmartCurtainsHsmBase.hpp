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
    RebootingDevice,
    Final_3,
    InitializingFeatures,
    Operable,
    UpdatingFirmware,
    UpdatingSystemTime,
    Final_2,
    StartWiFIAccessPoint,
    FinalizingInitialConfiguration,
    WaitingForConfigRequest,
    StartConfigPortal,
    ReceivingConfigParts,
    Final_6,
    PrepareFrontend,
    HomeAssistantIntegration,
    PreparingOTA,
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
    ReceivedConfigParts2,
    ReceivedConfigParts3,
    ReceivedConfigParts1,
    Final_5,
};

enum class SmartCurtainsHsmEvents
{
    INVALID = INVALID_ID,
    
    CLOSE_CURTAINS,
    REBOOT_DEVICE,
    CONFIGURATION_FAILED,
    AP_DISCONNECTED,
    TIME_UPDATED,
    CONFIG_NOT_AVAILABLE,
    DEVICE_READY,
    OTA_READY,
    DONE,
    POS_CHANGED,
    STOP_CURTAINS,
    OPEN_CURTAINS,
    HA_OFFLINE,
    BOOTING_FINISHED,
    FRONTEND_READY,
    ON_TIMER_CHECK_SYSTEM_TIME,
    UPDATE_STARTED,
    REGISTERED_WITH_HA,
    RESET_DONE,
    RESET_DEVICE,
    OPERATION_DONE,
    HA_ONLINE,
    WIFI_DISCONNECTED,
    UPDATE_FAILED,
    OPEN_LIMIT_REACHED,
    UPDATE_FINISHED,
    CONFIGURED,
    CONFIGURATION_RECEIVED,
    TIMER_CHECK_SYSTEM_TIME,
    WIFI_CONNECTED,
    INVALID_CONFIG_REQUEST,
    CONFIG_PORTAL_STARTED,
    AP_CONNECTED,
};

enum class SmartCurtainsHsmTimers
{
    CHECK_SYSTEM_TIME,
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
    virtual void onBeginHaRegistration(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onFinalizeInitialConfiguration(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onLoadLastPosition(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStateClosedCurtains(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStateClosingCurtains(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onRequestStopMotor(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStartConfigPortal(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStartWebFrontend(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStateOpeningCurtains(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStateFullyOpenCurtains(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStartWiFiAccessPoint(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStartOtaManager(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onRebootDevice(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onUpdateSystemTime(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStateStoppedCurtains(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onPrepareDevice(const hsmcpp::VariantVector_t& args) = 0;

// HSM state entering callbacks
protected:

// HSM state exiting callbacks
protected:
    virtual bool onRequestStopMotor() = 0;

// HSM transition callbacks
protected:
    // NOTE: override this method in child class if needed
    virtual void onTransitionFailed(const SmartCurtainsHsmEvents event, const hsmcpp::VariantVector_t& args);

    virtual void onStopWebFrontend(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onStopConfigPortal(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onCurtainsPositionChanged(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onHomeAssistantUnavailable(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onHomeAssistantAvailable(const hsmcpp::VariantVector_t& args) = 0;
    virtual void onValidateSystemTime(const hsmcpp::VariantVector_t& args) = 0;

// HSM transition condition callbacks
protected:
    virtual bool isFullyClosed(const hsmcpp::VariantVector_t& args) = 0;
    virtual bool isPartiallyOpen(const hsmcpp::VariantVector_t& args) = 0;
    virtual bool isFullyOpen(const hsmcpp::VariantVector_t& args) = 0;
    virtual bool hasCorrectSystemTime(const hsmcpp::VariantVector_t& args) = 0;

protected:
    std::string getStateName(const SmartCurtainsHsmStates state) override;
    std::string getEventName(const SmartCurtainsHsmEvents event) override;
};

#endif // __GEN_HSM_SMARTCURTAINSHSMBASE__