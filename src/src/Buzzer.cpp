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

#include "Buzzer.hpp"

#define PIN_BUZZER_CTRL (15)
#define PIN_BUZZER_GND (13)

#define DIT_DURATION (300)
#define GAP_DURATION (DIT_DURATION)

void Buzzer::initialize() {
    pinMode(PIN_BUZZER_GND, OUTPUT);
    pinMode(PIN_BUZZER_CTRL, OUTPUT);

    digitalWrite(PIN_BUZZER_GND, LOW);
    digitalWrite(PIN_BUZZER_CTRL, LOW);
}

void Buzzer::buzz(const int durationMs) {
    digitalWrite(PIN_BUZZER_CTRL, HIGH);
    delay(durationMs);
    digitalWrite(PIN_BUZZER_CTRL, LOW);
}

void Buzzer::buzzLetter(const char c) {
    if ('W' == c) {
        buzzShort();
        waitGap();
        buzzLong();
        waitGap();
        buzzLong();
    } else if ('R' == c) {
        buzzShort();
        waitGap();
        buzzLong();
        waitGap();
        buzzShort();
    } else if ('I' == c) {
        buzzShort();
        waitGap();
        buzzShort();
    } else if ('E' == c) {
        buzzShort();
    } else if ('K' == c) {
        buzzLong();
        waitGap();
        buzzShort();
        waitGap();
        buzzLong();
    } else if ('M' == c) {
        buzzLong();
        waitGap();
        buzzLong();
    } else if ('U' == c) {
        buzzShort();
        waitGap();
        buzzShort();
        waitGap();
        buzzLong();
    } else if ('O' == c) {
        buzzLong();
        waitGap();
        buzzLong();
        waitGap();
        buzzLong();
    } else if ('Q' == c) {
        buzzLong();
        waitGap();
        buzzLong();
        waitGap();
        buzzShort();
        waitGap();
        buzzLong();
    }
}

void Buzzer::buzzLong() {
    buzz(DIT_DURATION * 3);
}

void Buzzer::buzzShort() {
    buzz(DIT_DURATION);
}

void Buzzer::waitGap() {
    delay(GAP_DURATION);
}
