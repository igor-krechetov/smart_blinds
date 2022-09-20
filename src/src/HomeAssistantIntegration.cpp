/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#include "HomeAssistantIntegration.hpp"
#include <ArduinoJson.h>
#include <ArduinoLog.h>

#define HA_DISCOVERY_PREFIX     "homeassistant"
#define TOPIC_STATE             "state"
#define TOPIC_COMMANDS          "ctrl"
#define TOPIC_POSITION          "pos"
#define TOPIC_SET_POSITION      TOPIC_POSITION "/set"

void HomeAssistantIntegration::initialize(IHomeAssistantListener *listener)
{
    mListener = listener;
}

void HomeAssistantIntegration::start(const ConfigurationManager &config)
{
    // TODO: check result
    std::list<String> defaultSubscriptions;
    
    defaultSubscriptions.push_back(getTopicPath(TOPIC_COMMANDS));
    mClient.initialize(this, config.getMqttServerHost(), config.getMqttServerPort(), defaultSubscriptions);
}

void HomeAssistantIntegration::processEvents()
{
    mClient.processEvents();
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

void HomeAssistantIntegration::onTopicUpdated(char *topic, byte *payload, unsigned int length)
{
    Log.traceln("HomeAssistantIntegration: topic<%s>", topic);

    if (true == getTopicPath(TOPIC_COMMANDS).equals(topic)) {
        // TODO: impl
    } else if (true == getTopicPath(TOPIC_SET_POSITION).equals(topic)) {
        // TODO: impl
    } else {
        // TODO: impl
    }
}

String HomeAssistantIntegration::getTopicPrefix() const {
    return cTopicPrefix + "/" + String(ESP.getChipId());
}

String HomeAssistantIntegration::getTopicPath(const String &suffix) const
{
    return getTopicPrefix() + "/" + suffix;
}

void HomeAssistantIntegration::sendAvailabilityMessage() {
    mClient.publishTopic(getTopicPath("available"), "online", false);
}

void HomeAssistantIntegration::sendDiscoveryPayload()
{
    static boolean isDiscoverySent = false;

    if (false == isDiscoverySent) {
        Log.noticeln("Sending HA Discovery payload");

        isDiscoverySent = true;
        String chipId = String(ESP.getChipId());

        StaticJsonDocument<1024> payload;
        // StaticJsonDocument<100> payloadCmd;
        // String tempJson;

        payload["~"] = getTopicPrefix();
        payload["name"] = "SmartBlinds";
        payload["uniq_id"] = chipId;
        payload["dev_cla"] = "blind";
        payload["avty_t"] = "~/available";// availability_topic
        payload["cmd_t"] = "~/" TOPIC_COMMANDS;// command_topic
        payload["state_topic"] = "~/" TOPIC_STATE;
        payload["pos_t"] = "~/" TOPIC_POSITION;// position_topic
        payload["set_pos_t"] = "~/" TOPIC_SET_POSITION;// set_position_topic

        // payloadCmd["cmd"] = "set_pos";
        // payloadCmd["value"] = "{{ position }}";
        // serializeJson(payloadCmd, tempJson);
        // payloadCmd.clear();
        // payload["set_pos_tpl"] = tempJson;// 
        payload["set_pos_tpl"] = "{{ position }}";
        payload["pos_tpl"] = "{{ value_json.position }}";// position_template // TODO: validate
        payload["value_template"] = "????"; // TODO: Defines a template that can be used to extract the payload for the state_topic topic.

        payload["state_open"] = "open";
        payload["state_opening"] = "opening";
        payload["state_closed"] = "closed";
        payload["state_closing"] = "closing";

        payload["pl_open"] = "open";// payload_open
        payload["pl_stop"] = "stop";// payload_stop
        payload["pos_open"] = 0;
        payload["pos_clsd"] = 100;
        payload["opt"] = false;// optimistic

        JsonObject dev  = payload.createNestedObject("dev");// device
        dev["name"] = "Wemos SmartBlinds";
        dev["mf"] = "http://0.0.0.0";
        dev["sw"] = "0.1";
        dev["cu"] = "http://" + WiFi.localIP().toString();
        dev["mdl"] = "DIY";

        String haConfig;

        serializeJson(payload, haConfig);
        mClient.publishTopic(String(HA_DISCOVERY_PREFIX) + "/cover/" + chipId + "/config", haConfig, true);
    }

    //  = "{\"~\":\"" + mqttHelper.prefix + "/" + chipId + "\","
    //                    "\"name\":\"SmartBlinds\",\n"
    //                    "\"uniq_id\":\"" +
    //            String(chipId) + "_" + String(num) + "\","
    //                                                 "\"dev_cla\":\"blind\","
    //                                                 "\"avty_t\":\"~/available\","
    //                                                 "\"cmd_t\":\"~/in\","
    //                                                 "\"set_pos_t\":\"~/in\","
    //                                                 "\"set_pos_tpl\":\"{\\\"num\\\": " +
    //            String(num) + ", \\\"action\\\": \\\"auto\\\", \\\"value\\\": {{ 100 - position }} }\",\n"
    //                          "\"pos_t\":\"~/out\",\n"
    //                          "\"pos_tpl\":\"{{ value_json.position" +
    //            String(num) + " }}\",\n"
    //                          "\"pl_open\":\"{\\\"num\\\": " +
    //            String(num) + ", \\\"action\\\": \\\"auto\\\", \\\"value\\\": 0}\",\n"
    //                          "\"pl_cls\":\"{\\\"num\\\": " +
    //            String(num) + ", \\\"action\\\": \\\"auto\\\", \\\"value\\\": 100}\",\n"
    //                          "\"pl_stop\":\"{\\\"num\\\": " +
    //            String(num) + ", \\\"action\\\": \\\"stop\\\", \\\"value\\\": 0}\",\n"
    //                          "\"pos_open\":0,\n"
    //                          "\"pos_clsd\":100,\n"
    //                          "\"opt\":false,"
    //                          "\"dev\":{\"ids\":\"" +
    //            String(chipId) + "\",\"name\":\"Wemos SmartBlinds\",\"mf\":\"https://github.com/eg321/esp32-motorized-roller-blinds\",\"sw\":\"" + version + "\",\"cu\":\"http://" + WiFi.localIP().toString() + "\",\"mdl\":\"DIY\"} "
                                                                                                                                                                                                                        //  "}";
}