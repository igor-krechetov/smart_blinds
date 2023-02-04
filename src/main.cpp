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

#include <Arduino.h>

#include "src/LoggingHelper.hpp"
#include "src/SmartCurtains.hpp"

SmartCurtains *gFirmware = nullptr;

unsigned long gStackAddressBegin = 0;

unsigned long getLatestStackAdress() {
    byte stackVar;
    return (unsigned long)&stackVar;
}

// RAM END
// MAX HEAP: 52208 bytes
// MAX STACK: ~3616
// gStackAddressBegin=3fffffa0

// TODO: hard reset device on critical errors (not available WiFi, etc.)

void setup(void) {
    gStackAddressBegin = getLatestStackAdress() + 64;

    Serial.begin(115200);
    while (!Serial && !Serial.available()) {
        delay(10);
    }

#if !defined(DISABLE_APP_LOGGING)
    Log.begin(LOG_LEVEL_VERBOSE, &Serial);
#endif

    PRINT_FREE_HEAP();
    gFirmware = new SmartCurtains();
    PRINT_FREE_HEAP();
    gFirmware->initialize();
    PRINT_FREE_HEAP();
}

void loop(void) {
    gFirmware->processEvents();
}
