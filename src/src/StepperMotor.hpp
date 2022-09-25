/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */
#ifndef STEPPERMOTOR_HPP
#define STEPPERMOTOR_HPP

#include <Arduino.h>
#include <AccelStepper.h>

class IStepperMotorListener {
public:
    virtual void onMotorPositionChanged(const uint32_t pos) = 0;
    virtual void onMotorOperationFinished() = 0;
};

class StepperMotor {
public:
    StepperMotor();

    void initialize(IStepperMotorListener* listener);
    void processEvents();

    void setEnabled(const bool enabled);

    void overrideCurrentPosition(const uint32_t newPos);// 0 ~ 100
    uint32_t getCurrentPosition();// 0 ~ 100
    uint32_t getCurrentRevolution();

    void setLimit(const uint32_t revolutions);
    void setSpeed(const uint32_t rpm);

    void stopMovement();
    void moveToPosition(const uint32_t pos);

    void moveCW(const uint32_t revolutions);
    void moveCCW(const uint32_t revolutions);

private:
    void refreshCurrentPosition();

private:
    const double cStepsPerRevolution = 200.0;// Nema 17
    const uint32_t cMaxRPM = 600;// Nema 17

    uint32_t mLimitRevolutions = 10;// revolutions
    uint32_t mPrevPosition = 0;
    uint32_t mRevolutionsPerPosition = 10;// revolutions per position * 100 

    bool mEnabled = false;
    IStepperMotorListener *mListener = nullptr;
    AccelStepper mStepper;
};

#endif // STEPPERMOTOR_HPP