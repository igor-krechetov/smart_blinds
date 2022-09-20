/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#include "StepperMotor.hpp"

#define PIN_STEP            (4)
#define PIN_DIR             (0)
#define PIN_ENABLE          (5)

StepperMotor::StepperMotor() : mStepper(AccelStepper::DRIVER, PIN_STEP, PIN_DIR) {

}

void StepperMotor::initialize(IStepperMotorListener* listener){
    mListener = listener;

    pinMode(PIN_ENABLE, OUTPUT);
    setEnabled(false);
    setSpeed(60);

    // TODO: test this
    // stepper.setEnablePin(5);
    // stepper.enableOutputs();
}

void StepperMotor::processEvents() {
    if (true == mEnabled) {
        if (false == mStepper.run())
        {
            setEnabled(false);
            mListener->onMotorOperationFinished();
        }
    }
}

void StepperMotor::setEnabled(const bool enabled) {
    mEnabled = enabled;
    digitalWrite(PIN_ENABLE, (true == mEnabled ? LOW : HIGH));
}

void StepperMotor::setSpeed(const int rpm) {
    int speedStepsPerSec = rpm * cStepsPerRevolution / 60;

    if (rpm <= 0) {
        speedStepsPerSec = cStepsPerRevolution / 60;
    } else if (rpm > cMaxRPM) {
        speedStepsPerSec = 10 * cStepsPerRevolution;
    }

    mStepper.setSpeed(speedStepsPerSec);
    mStepper.setMaxSpeed(speedStepsPerSec);
    mStepper.setAcceleration(3000);
}

void StepperMotor::moveCW(const int revolutions) {
    setEnabled(true);
    mStepper.stop();
    mStepper.move(revolutions * cStepsPerRevolution);
}

void StepperMotor::moveCCW(const int revolutions) {
    setEnabled(true);
    mStepper.stop();
    mStepper.move(-revolutions * cStepsPerRevolution);
}
