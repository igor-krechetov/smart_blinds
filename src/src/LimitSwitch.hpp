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

#endif  // LIMITSWITCH_HPP