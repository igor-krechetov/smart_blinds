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

#include "HomeAssistantIntegration.hpp"

#include <ArduinoJson.h>

#include "LoggingHelper.hpp"

#define TOPIC_PREFIX "SmartCurtains/"
#define HA_DISCOVERY_PREFIX "homeassistant"
#define TOPIC_STATE "/state"
#define TOPIC_AVAILABILITY "/available"
#define TOPIC_COMMANDS "/ctrl"
#define TOPIC_POSITION "/pos"
#define TOPIC_SET_POSITION "/pos/set"
#define TOPIC_HA_STATUS F("homeassistant/status")

// NOTE: make sure to sync with TOPIC_AVAILABILITY and other macroses
const char gDiscoveryJsonTemplate[] PROGMEM = R"===(
{"~":"","name":"SmartCurtains","uniq_id":"","dev_cla":"curtain","avty_t":"~/available","cmd_t":"~/ctrl","stat_t":"~/state","pos_t":"~/pos","set_pos_t":"~/pos/set","set_pos_tpl":"{{ position }}","pos_tpl":"{{ value }}","val_tpl":"{{ value }}","stat_open":"open","stat_opening":"opening","stat_clsd":"closed","stat_closing":"closing","avty_tpl":"{{ value }}","pl_avail":"on","pl_not_avail":"off","payload_open":"open","payload_close":"close","payload_stop":"stop","pos_open":100,"pos_clsd":0,"opt":false,"dev":{"name":"Wemos SmartCurtains","ids":[],"mf":"Igor Krechetov","sw":"","mdl":"R1"}}
)===";

// TODO: move to common location
#define FV(_progStr) reinterpret_cast<const __FlashStringHelper*>(_progStr)

/*
{"~":"SmartCurtains/6404654","name":"SmartCurtains","uniq_id":"6404654","dev_cla":"curtain","avty_t":"~/available","cmd_t":"~/ctrl","stat_t":"~/state","pos_t":"~/pos","set_pos_t":"~/pos/set","set_pos_tpl":"{{
position }}","pos_tpl":"{{ value }}","val_tpl":"{{ value
}}","stat_open":"open","stat_opening":"opening","stat_clsd":"closed","stat_closing":"closing","avty_tpl":"{{ value
}}","pl_avail":"on","pl_not_avail":"off","payload_open":"open","payload_close":"close","payload_stop":"stop","pos_open":100,"pos_clsd":0,"opt":false,"dev":{"name":"Wemos
SmartCurtains","ids":["6404654"],"mf":"Igor Krechetov","sw":"0.1","mdl":"P1"}}
*/

HomeAssistantIntegration::HomeAssistantIntegration(ConfigurationManager& config) : mConfig(config) {}

bool HomeAssistantIntegration::initialize(IHomeAssistantListener* listener) {
    bool res = false;

    mListener = listener;

    if (nullptr != mListener) {
        snprintf(mObjectID, sizeof(mObjectID), "%" PRIu32, ESP.getChipId());
        snprintf(mTopicPrefix, sizeof(mTopicPrefix), TOPIC_PREFIX "%s", mObjectID);
        snprintf(mTopicConfig, sizeof(mTopicConfig), HA_DISCOVERY_PREFIX "/cover/%s/config", mObjectID);
        res = true;
    }

    return res;
}

bool HomeAssistantIntegration::start() {
    bool res = false;

    if (nullptr != mListener) {
        PRINT_FREE_HEAP();
        // TODO: replace String with char*
        std::list<String> defaultSubscriptions;

        defaultSubscriptions.push_back(getTopicPath(F(TOPIC_COMMANDS)));
        defaultSubscriptions.push_back(getTopicPath(F(TOPIC_SET_POSITION)));
        defaultSubscriptions.push_back(getTopicPath(F(TOPIC_POSITION)));
        defaultSubscriptions.push_back(TOPIC_HA_STATUS);
        PRINT_FREE_HEAP();

        res = mClient.initialize(mConfig, this, defaultSubscriptions);
        PRINT_FREE_HEAP();
    }

    return res;
}

void HomeAssistantIntegration::reconnect() {
    mClient.disconnect();

    if (false == mClient.connect()) {
        mListener->onMqttConnectionLost();
    }
}

void HomeAssistantIntegration::disconnect() {
    mClient.disconnect();
}

bool HomeAssistantIntegration::isConnected() {
    return mClient.isConnected();
}

void HomeAssistantIntegration::processEvents() {
    mClient.processEvents();
}

void HomeAssistantIntegration::updateAvailability(const DeviceAvailability availability) {
    mClient.publishTopic(getTopicPath(F(TOPIC_AVAILABILITY)),
                         (DeviceAvailability::ONLINE == availability ? F("on") : F("off")),
                         false);
    processEvents();  // process events to force send data
}

void HomeAssistantIntegration::updateDeviceState(const CoverState state) {
    String value;

    switch (state) {
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

    mClient.publishTopic(getTopicPath(F(TOPIC_STATE)), value, false);
}

void HomeAssistantIntegration::updateCurrentPosition(const int pos) {
    if ((pos >= 0) && (pos <= 100)) {
        // reverse position. HA expects 0 as closed and 100 as open, but we use opposite
        mLastPosition = 100 - pos;
        mClient.publishTopic(getTopicPath(F(TOPIC_POSITION)), String(mLastPosition), true);
    } else {
        ERROR_ARGS("invalid pos=%d (only 0~100 allowed)", pos);
    }
}

int HomeAssistantIntegration::getLastPosition() const {
    return (100 - mLastPosition);
}

void HomeAssistantIntegration::onMqttConnected() {
    sendDiscoveryPayload();
}

void HomeAssistantIntegration::onMqttDisconnected() {
    mListener->onMqttConnectionLost();
}

void HomeAssistantIntegration::onTopicUpdated(char* topic, char* payload, unsigned int length) {
    TRACE_ARGS("HomeAssistantIntegration: topic<%s>, len=%u", topic, length);

    if ((nullptr != payload) && (length > 0)) {
        if (true == getTopicPath(F(TOPIC_COMMANDS)).equals(topic)) {
            if (true == comparePayload("open", payload, length)) {
                mListener->onHaRequestOpen();
            } else if (true == comparePayload("close", payload, length)) {
                mListener->onHaRequestClose();
            } else if (true == comparePayload("stop", payload, length)) {
                mListener->onHaRequestStop();
            } else {
                ERROR("unknown command");
                for (unsigned int i = 0; i < length; ++i) {
                    ERROR_ARGS("%c", payload[i]);
                }
            }
        } else if (true == getTopicPath(F(TOPIC_SET_POSITION)).equals(topic)) {
            mListener->onHaRequestSetPosition(100 - atoi(payload));
        } else if (true == getTopicPath(F(TOPIC_POSITION)).equals(topic)) {
            const int prevPos = mLastPosition;

            mLastPosition = atoi(payload);  // TODO: add validation?

            if (INVALID_LAST_POSITION == prevPos) {
                mListener->onHaLastPositionLoaded();
            }
        } else if (true == getTopicPath(TOPIC_HA_STATUS).equals(topic)) {
            if (true == comparePayload("online", payload, length)) {
                mListener->onHaConnected();
                // TODO: need sendDiscoveryPayload() ??
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

bool HomeAssistantIntegration::comparePayload(const char* str, char* payload, unsigned int payloadSize) const {
    bool isEqual = false;

    for (unsigned int i = 0; (i < payloadSize) && (0 != str[i]); ++i) {
        isEqual = (str[i] == payload[i]);

        if (false == isEqual) {
            break;
        }
    }

    return isEqual;
}

String HomeAssistantIntegration::getTopicPath(const __FlashStringHelper* suffix) const {
    return String(mTopicPrefix) + suffix;
}

void HomeAssistantIntegration::sendDiscoveryPayload() {
    NOTICE("Sending HA Discovery payload");
    // NOTE: CRITICAL: make sure to check memory size if values will signifficantly change
    DynamicJsonDocument payload(1000);

    // NOTE: using PROGMEM to reduce stack usage
    DeserializationError err = deserializeJson(payload, FV(gDiscoveryJsonTemplate));
    if (DeserializationError::Ok == err) {
        payload["~"] = mTopicPrefix;
        payload["uniq_id"] = mObjectID;
        payload["dev"]["ids"].add(mObjectID);
        payload["dev"]["sw"] = mConfig.getFirmwareVersion();

        std::vector<char> haConfig(650, 0);
        const int haConfigSize = serializeJson(payload, haConfig.data(), haConfig.size());

        if (true == mClient.publishTopic(mTopicConfig, haConfig.data(), haConfigSize, true)) {
            // NOTE: initially mark our device as unavailable
            updateAvailability(DeviceAvailability::OFFLINE);
            mListener->onHaRegistrationDone();
        }
    } else {
        ERROR_ARGS("fatal: can't parse gDiscoveryJsonTemplate <%s>", err.c_str());
    }
}