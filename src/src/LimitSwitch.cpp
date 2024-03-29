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

#include "LimitSwitch.hpp"

#define PIN_SWITCH_C (14)
#define PIN_SWITCH_NO (16)

void LimitSwitch::initialize(ILimitSwitchListener* listener) {
    mListener = listener;

    pinMode(PIN_SWITCH_C, OUTPUT);
    pinMode(PIN_SWITCH_NO, INPUT_PULLDOWN_16);
    digitalWrite(PIN_SWITCH_C, HIGH);
}

void LimitSwitch::processEvents() {
    if (nullptr != mListener) {
        const int switchState = digitalRead(PIN_SWITCH_NO);

        if (switchState != mPrevSwitchState) {
            mPrevSwitchState = switchState;

            if (switchState == HIGH) {
                mListener->onLimitSwitchPressed();
            } else {
                mListener->onLimitSwitchReleased();
            }
        }
    }
}
