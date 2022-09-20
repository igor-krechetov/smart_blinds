/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#include <Arduino.h>
#include <ArduinoLog.h>
#include "src/SmartBlinds.hpp"

SmartBlinds gFirmware;

void setup(void) {
    Serial.begin(115200);
    while (!Serial && !Serial.available()) {
        delay(10);
    }

    delay(5000);
    Log.begin(LOG_LEVEL_VERBOSE, &Serial);

    Log.traceln("Configuring...");
    gFirmware.initialize();

// #ifdef ESP32
//     esp_task_wdt_init(10, true);
// #else
//     ESP.wdtDisable();
// #endif
}

void loop(void) {
#ifdef ESP32
    esp_task_wdt_reset();
#else //ESP8266
    ESP.wdtFeed();
#endif
    
    gFirmware.processEvents();

    // /**
    //   Storing positioning data and turns off the power to the coils
    // */
    // if (saveItNow) {
    //     saveConfig();
    //     saveItNow = false;

    //     stopPowerToCoils();
    // }

    // /**
    //   Manage actions. Steering of the blind
    // */
    // uint8_t num = 0;
    // bool isTimeToSendUpdate = false;
    // for (StepperHelper &stepperHelper : stepperHelpers) {
    //     num++;
    //     if (!stepperHelper.isConnected()) {
    //         continue;
    //     }
    //     if (stepperHelper.action == "auto") {
    //         stepperHelper.getStepper()->run();
    //         stepperHelper.currentPosition = stepperHelper.targetPosition - stepperHelper.getStepper()->getStepsLeft();
    //         if (stepperHelper.currentPosition == stepperHelper.targetPosition) {
    //             stepperHelper.route = 0;
    //             stepperHelper.action = "";
    //             stepperHelper.set = (stepperHelper.targetPosition * 100) / stepperHelper.maxPosition;
    //             stepperHelper.pos = (stepperHelper.currentPosition * 100) / stepperHelper.maxPosition;
    //             sendUpdate();
    //             Serial.printf("Stepper %i has reached target position.\r\n", num);
    //             saveItNow = true;
    //             isTimeToSendUpdate = true;
    //         }
    //     } else if (stepperHelper.action == "manual" && stepperHelper.route != 0) {
    //         stepperHelper.getStepper()->move(stepperHelper.route > 0, abs(stepperHelper.route));
    //         stepperHelper.currentPosition += stepperHelper.route;
    //     }

    //     if (stepperHelper.action != "") {
    //         isTimeToSendUpdate = true;
    //     }
    // }

    // if (isTimeToSendUpdate) {
    //     long now = millis();
    //     if (now - lastPublish > 3000) { // Update state
    //         lastPublish = now;
    //         sendUpdate();
    //     }
    // }

    /*
       After running setup() the motor might still have
       power on some of the coils. This is making sure that
       power is off the first time loop() has been executed
       to avoid heating the stepper motor draining
       unnecessary current
    */
    // if (initLoop) {
    //     initLoop = false;
    //     stopPowerToCoils();
    // }
}