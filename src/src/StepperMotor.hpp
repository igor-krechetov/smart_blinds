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
#ifndef STEPPERMOTOR_HPP
#define STEPPERMOTOR_HPP

#include <AccelStepper.h>
#include <Arduino.h>

class IStepperMotorListener {
public:
    virtual void onMotorPositionChanged(const uint32_t pos, const uint32_t currentRevolution) = 0;
    virtual void onMotorOperationFinished() = 0;
};

class StepperMotor {
public:
    StepperMotor();

    void initialize(IStepperMotorListener* listener);
    void processEvents();

    void setEnabled(const bool enabled);

    void overrideCurrentPosition(const uint32_t newPos);           // 0 ~ max
    void overrideCurrentRevolution(const uint32_t newRevolution);  // 0 ~ ...
    uint32_t getCurrentPosition();                                 // 0 ~ 100
    uint32_t getCurrentRevolution();

    void enablePositionLimit(const bool enable);
    void setMaxPosition(const uint32_t maxPos);
    void setRevolutionsLimit(const uint32_t revolutions);
    void setSpeed(const uint32_t level);  // 0 ~ 10

    void stopMovement(const bool alignPosition);
    void moveToPosition(const uint32_t pos);

    bool moveCW(const uint32_t revolutions);
    bool moveCCW(const uint32_t revolutions);

    void startSpinningCW();
    void startSpinningCCW();

private:
    void moveStepperTo(const uint32_t revolutions);
    void forceStop();

private:
    // 200 - Nema 17 speps per revolution
    // 12.4 - planetary gear coefficient
    const double cStepsPerRevolution = 200.0 * 12.4;
    const uint32_t cMaxRPM = 600;  // Nema 17

    uint32_t mSpeedLevel = 0;  // 0 ~ 10

    bool mPositionLimitEnabled = true;
    uint32_t mMaxPosition = 100;
    uint32_t mLimitRevolutions = 0;  // amount of revolutions to be considered as position=mMaxPosition
    uint32_t mPrevPosition = 0;
    double mRevolutionsPerPosition = 0.0;  // revolutions per position * 100

    bool mEnabled = false;
    IStepperMotorListener* mListener = nullptr;
    AccelStepper mStepper;

    bool mIsSpinning = false;
};

#endif  // STEPPERMOTOR_HPP