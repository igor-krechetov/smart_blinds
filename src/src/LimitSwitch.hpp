/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#ifndef LIMITSWITCH_HPP
#define LIMITSWITCH_HPP

#include <Arduino.h>

class ILimitSwitchListener {
public:
    virtual void onLimitSwitchPressed() = 0;
    virtual void onLimitSwitchReleased() = 0;
};

class LimitSwitch {
public:
    void initialize(ILimitSwitchListener* listener);
    void processEvents();

private:
    ILimitSwitchListener* mListener = nullptr;
    int mPrevSwitchState = LOW;
};

#endif // LIMITSWITCH_HPP