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
    virtual void onMotorOperationFinished() = 0;
};

class StepperMotor {
public:
    StepperMotor();

    void initialize(IStepperMotorListener* listener);
    void processEvents();

    void setEnabled(const bool enabled);

    void setSpeed(const int rpm);
    void moveCW(const int revolutions);
    void moveCCW(const int revolutions);

private:
    const int cStepsPerRevolution = 200;// Nema 17
    const int cMaxRPM = 600;// Nema 17

    IStepperMotorListener *mListener = nullptr;
    AccelStepper mStepper;
    bool mEnabled = false;
};

#endif // STEPPERMOTOR_HPP