/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#include <Arduino.h>
#include <ArduinoLog.h>
#include "src/SmartCurtains.hpp"
#include "src/LoggingHelper.hpp"

SmartCurtains gFirmware;

// MAX HEAP: 52208 bytes

void setup(void) {
    Serial.begin(115200);
    while (!Serial && !Serial.available()) {
        delay(10);
    }

    delay(5000);
    Log.begin(LOG_LEVEL_VERBOSE, &Serial);

    Log.traceln("FIRMWARE VERSION: 0.1");
    PRINT_FREE_HEAP();
    gFirmware.initialize();
    PRINT_FREE_HEAP();

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
}