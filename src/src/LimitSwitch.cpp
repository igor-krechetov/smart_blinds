/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#include "LimitSwitch.hpp"

#define PIN_SWITCH_C        (14)
#define PIN_SWITCH_NO       (16)

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
