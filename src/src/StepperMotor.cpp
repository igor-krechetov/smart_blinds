/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */

#include "StepperMotor.hpp"
#include "LoggingHelper.hpp"

#define PIN_STEP            (4)
#define PIN_DIR             (0)
#define PIN_ENABLE          (5)

StepperMotor::StepperMotor() : mStepper(AccelStepper::DRIVER, PIN_STEP, PIN_DIR) {

}

void StepperMotor::initialize(IStepperMotorListener* listener){
    mListener = listener;

    pinMode(PIN_ENABLE, OUTPUT);
    setEnabled(false);

    overrideCurrentPosition(0);
    setLimit(10);
    setSpeed(60);

    // TODO: test this
    // stepper.setEnablePin(5);
    // stepper.enableOutputs();
}

void StepperMotor::processEvents() {
    if (true == mEnabled) {
        const bool isRunning = mStepper.run();

        // refreshCurrentPosition();
        if (getCurrentPosition() != mPrevPosition) {
            mPrevPosition = getCurrentPosition();
            mListener->onMotorPositionChanged(mPrevPosition);
        }

        if (false == isRunning)
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

void StepperMotor::overrideCurrentPosition(const uint32_t newPos)
{
    if (false == mStepper.isRunning()) {
        TRACE_ARGS("overrideCurrentPosition: newPos=%" PRIu32, newPos);
        mPrevPosition = newPos;
        mStepper.setCurrentPosition(newPos * mRevolutionsPerPosition * cStepsPerRevolution / 100);
    } else {
        ERROR("can't change position of a running motor");
    }
}

uint32_t StepperMotor::getCurrentPosition()
{
    // TODO: debug
    // TRACE_ARGS("getCurrentPosition: steps=%d, revolutions=%d, pos=%d",
    //             (int)mStepper.currentPosition(),
    //             (int)mStepper.currentPosition() * 100 / mRevolutionsPerPosition,
    //             (int)mStepper.currentPosition() * 100 / mRevolutionsPerPosition / cStepsPerRevolution);
    return mStepper.currentPosition() * 100 / mRevolutionsPerPosition / cStepsPerRevolution;
}

uint32_t StepperMotor::getCurrentRevolution() {
    return mStepper.currentPosition() / cStepsPerRevolution;
}

void StepperMotor::setLimit(const uint32_t revolutions) {
    mLimitRevolutions = revolutions;
    mRevolutionsPerPosition = mLimitRevolutions;
}

void StepperMotor::setSpeed(const uint32_t rpm) {
    uint32_t speedStepsPerSec = rpm * cStepsPerRevolution / 60;

    if (rpm <= 0) {
        speedStepsPerSec = cStepsPerRevolution / 60;
    } else if (rpm > cMaxRPM) {
        speedStepsPerSec = 10 * cStepsPerRevolution;
    }

    mStepper.setMaxSpeed(speedStepsPerSec);
    mStepper.setAcceleration(3000);
}

void StepperMotor::stopMovement() {
    if (true == mStepper.isRunning()) {
        mStepper.stop();
        uint32_t extraSteps = mStepper.currentPosition() % static_cast<uint32_t>(mRevolutionsPerPosition * cStepsPerRevolution / 100);

        if (0 != extraSteps)
        {
            setEnabled(true);

            if (extraSteps > (cStepsPerRevolution / 2))
            {
                mStepper.move(cStepsPerRevolution - extraSteps);
            } else {
                mStepper.move(-extraSteps);
            }
        } else {
            setEnabled(false);
        }
    } else {
        WARNING("IGNORED. Stepper was not running");
    }
}

void StepperMotor::moveToPosition(const uint32_t pos) {
    TRACE_ARGS("StepperMotor::moveToPosition: %" PRIu32, pos);
    mStepper.stop();

    if ((pos != getCurrentPosition()) && (pos <= 100)){
        setEnabled(true);
        TRACE_ARGS("moveTo: %d", (int)pos * mRevolutionsPerPosition * cStepsPerRevolution / 100);
        mStepper.moveTo(pos * mRevolutionsPerPosition * cStepsPerRevolution / 100);
    } else {
        setEnabled(false);
    }
}

void StepperMotor::moveCW(const uint32_t revolutions) {
    TRACE_ARGS("StepperMotor::moveCW: %" PRIu32, revolutions);
    mStepper.stop();
    
    if ((getCurrentRevolution() + revolutions) <= mLimitRevolutions) {
        setEnabled(true);
        TRACE_ARGS("moveTo: %d", (int)(getCurrentRevolution() + revolutions) * cStepsPerRevolution);
        mStepper.moveTo((getCurrentRevolution() + revolutions) * cStepsPerRevolution);
    } else {
        setEnabled(false);
    }
}

void StepperMotor::moveCCW(const uint32_t revolutions) {
    TRACE_ARGS("StepperMotor::moveCCW: %" PRIu32, revolutions);
    mStepper.stop();
    
    if ((getCurrentRevolution() - revolutions) >= 0) {
        setEnabled(true);
        TRACE_ARGS("moveTo: %d", (int)(getCurrentRevolution() - revolutions) * cStepsPerRevolution);
        mStepper.moveTo((getCurrentRevolution() - revolutions) * cStepsPerRevolution);
    } else {
        setEnabled(false);
    }
}
