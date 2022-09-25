/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#include "HomeAssistantIntegration.hpp"
#include <ArduinoJson.h>
#include "LoggingHelper.hpp"

#define HA_DISCOVERY_PREFIX     "homeassistant"
#define TOPIC_STATE             "state"
#define TOPIC_AVAILABILITY      "available"
#define TOPIC_COMMANDS          "ctrl"
#define TOPIC_POSITION          "pos"
#define TOPIC_SET_POSITION      "pos/set"
#define TOPIC_HA_STATUS         F("homeassistant/status")

void HomeAssistantIntegration::initialize(IHomeAssistantListener *listener)
{
    mListener = listener;
}

void HomeAssistantIntegration::start(const ConfigurationManager &config)
{
    // TODO: check result
    std::list<String> defaultSubscriptions;
    
    defaultSubscriptions.push_back(getTopicPath(TOPIC_COMMANDS));
    defaultSubscriptions.push_back(getTopicPath(TOPIC_SET_POSITION));
    defaultSubscriptions.push_back(getTopicPath(TOPIC_POSITION));
    defaultSubscriptions.push_back(TOPIC_HA_STATUS);
    mClient.initialize(this, config.getMqttServerHost(), config.getMqttServerPort(), defaultSubscriptions);
}

void HomeAssistantIntegration::processEvents()
{
    mClient.processEvents();
}

void HomeAssistantIntegration::updateAvailability(const DeviceAvailability availability) {
    mClient.publishTopic(getTopicPath(TOPIC_AVAILABILITY), (DeviceAvailability::ONLINE == availability ? F("on") : F("off")), false);
}

void HomeAssistantIntegration::updateDeviceState(const CoverState state)
{
    String value;

    switch(state)
    {
        case CoverState::OPEN:
            value = F("open");
            break;
        case CoverState::CLOSED:
            value = F("closed");
            break;
        case CoverState::OPENING:
            value = F("opening");
            break;
        case CoverState::CLOSING:
            value = F("closing");
            break;
        case CoverState::STOPPED:
        default:
            value = F("stopped");
            break;
    }

    mClient.publishTopic(getTopicPath(TOPIC_STATE), value, false);
}

void HomeAssistantIntegration::updateCurrentPosition(const int pos)
{
    if ((pos >= 0) && (pos <= 100)){
        mLastPosition = pos;
        mClient.publishTopic(getTopicPath(TOPIC_POSITION), String(pos), true);
    } else {
        ERROR_ARGS("invalid pos=%d (only 0~100 allowed)", pos);
    }
}

int HomeAssistantIntegration::getLastPosition() const
{
    return mLastPosition;
}

void HomeAssistantIntegration::onMqttConnected()
{
    // TODO: impl
    sendDiscoveryPayload();
}

void HomeAssistantIntegration::onMqttDisconnected()
{
    // TODO: impl
}

void HomeAssistantIntegration::onTopicUpdated(char *topic, char *payload, unsigned int length)
{
    TRACE_ARGS("HomeAssistantIntegration: topic<%s>, len=%u", topic, length);

    if ((nullptr != payload) && (length > 0)) {
        if (true == getTopicPath(TOPIC_COMMANDS).equals(topic)) {
            if (true == comparePayload("open", payload, length)) {
                mListener->onHaRequestOpen();
            } else if (true == comparePayload("close", payload, length)) {
                mListener->onHaRequestClose();
            } else if (true == comparePayload("stop", payload, length)) {
                mListener->onHaRequestStop();
            } else {
                ERROR("unknown command");
                for (int i = 0; i < length; ++i) {
                    Log.error("%c", payload[i]);
                }
                Log.error("\n");
            }
        } else if (true == getTopicPath(TOPIC_SET_POSITION).equals(topic)) {
            mListener->onHaRequestSetPosition(atoi(payload));
        } else if (true == getTopicPath(TOPIC_POSITION).equals(topic)) {
            mLastPosition = atoi(payload);
        } else if (true == getTopicPath(TOPIC_HA_STATUS).equals(topic)) {
            if (true == comparePayload("online", payload, length)) {
                mListener->onHaConnected();
            } else {
                mListener->onHaDisconnected();
            }
        } else {
            ERROR("unexpected topic");
        }
    } else {
        ERROR("empty payload");
    }
}

bool HomeAssistantIntegration::comparePayload(const char* str, char* payload, unsigned int payloadSize) const
{
    bool isEqual = false;

    for (int i = 0 ; (i < payloadSize) && (0 != str[i]) ; ++i)
    {
        isEqual = (str[i] == payload[i]);

        if (false == isEqual) {
            break;
        }
    }

    return isEqual;
}

String HomeAssistantIntegration::getTopicPrefix() const {
    return cTopicPrefix + "/" + String(ESP.getChipId());
}

String HomeAssistantIntegration::getTopicPath(const String &suffix) const
{
    return getTopicPrefix() + "/" + suffix;
}

void HomeAssistantIntegration::sendDiscoveryPayload()
{
    // TODO: remove isDiscoverySent
    static boolean isDiscoverySent = false;

    if (false == isDiscoverySent) {
        NOTICE("Sending HA Discovery payload");

        isDiscoverySent = true;
        String objectId = String(ESP.getChipId());

        StaticJsonDocument<1024> payload;

        payload[F("~")] = getTopicPrefix();
        payload[F("name")] = F("SmartCurtains");
        payload[F("uniq_id")] = objectId;
        payload[F("dev_cla")] = F("curtain");
        payload[F("avty_t")] = "~/" TOPIC_AVAILABILITY;// availability_topic
        payload[F("cmd_t")] = "~/" TOPIC_COMMANDS;// command_topic
        payload[F("state_topic")] = "~/" TOPIC_STATE;
        payload[F("pos_t")] = "~/" TOPIC_POSITION;// position_topic
        payload[F("set_pos_t")] = "~/" TOPIC_SET_POSITION;// set_position_topic

        payload[F("set_pos_tpl")] = F("{{ position }}");
        payload[F("pos_tpl")] = F("{{ value }}");// position_template

        payload[F("value_template")] = F("{{ value }}"); // TODO: Defines a template that can be used to extract the payload for the state_topic topic.
        payload[F("state_open")] = F("open");
        payload[F("state_opening")] = F("opening");
        payload[F("state_closed")] = F("closed");
        payload[F("state_closing")] = F("closing");

        payload[F("avty_tpl")] = F("{{ value }}");
        payload[F("pl_avail")] = F("on");
        payload[F("pl_not_avail")] = F("off");

        payload[F("pl_open")] = F("open");// payload_open
        payload[F("pl_cls")] = F("close");// payload_close
        payload[F("pl_stop")] = F("stop");// payload_stop
        payload[F("pos_open")] = 100;
        payload[F("pos_clsd")] = 0;
        payload[F("opt")] = false;// optimistic

        JsonObject dev  = payload.createNestedObject("dev");// device
        dev["name"] = F("Wemos SmartCurtains");
        JsonArray ids = dev.createNestedArray(F("ids"));
        ids.add(objectId);
        dev["mf"] = F("Igor Krechetov");
        dev["sw"] = F("0.1");
        dev["mdl"] = F("P1");

        String haConfig;

        serializeJson(payload, haConfig);
        
        if (true == mClient.publishTopic(String(HA_DISCOVERY_PREFIX) + "/cover/" + objectId + "/config", haConfig, true)) {
            // NOTE: initially mark our device as unavailable
            updateAvailability(DeviceAvailability::OFFLINE);
            mListener->onHaRegistrationDone();
        }
    }
}