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

#include "StepperMotor.hpp"

#include "LoggingHelper.hpp"

#define PIN_STEP (4)
#define PIN_DIR (0)
#define PIN_ENABLE (5)

StepperMotor::StepperMotor() : mStepper(AccelStepper::DRIVER, PIN_STEP, PIN_DIR) {}

void StepperMotor::initialize(IStepperMotorListener* listener) {
    mListener = listener;

    pinMode(PIN_ENABLE, OUTPUT);
    setEnabled(false);

    mStepper.setMaxSpeed(1000);
    mStepper.setSpeed(0);
    mStepper.setCurrentPosition(0);
    setMaxPosition(100);
    setRevolutionsLimit(10);

    // TODO: test this
    // stepper.setEnablePin(5);
    // stepper.enableOutputs();
}

void StepperMotor::processEvents() {
    if (true == mEnabled) {
        if (true == mIsSpinning) {
            mStepper.runSpeed();
        } else {
            mStepper.runSpeedToPosition();

            if (getCurrentPosition() != mPrevPosition) {
                mPrevPosition = getCurrentPosition();
                mListener->onMotorPositionChanged(mPrevPosition, getCurrentRevolution());
            }

            if (mStepper.targetPosition() == mStepper.currentPosition()) {
                forceStop();
                TRACE_ARGS("curRevolution=%" PRIu32, getCurrentRevolution());
                mListener->onMotorOperationFinished();
            }
        }
    }
}

void StepperMotor::setEnabled(const bool enabled) {
    mEnabled = enabled;
    digitalWrite(PIN_ENABLE, (true == mEnabled ? LOW : HIGH));
}

void StepperMotor::overrideCurrentPosition(const uint32_t newPos) {
    if (false == mStepper.isRunning()) {
        TRACE_ARGS("overrideCurrentPosition: newPos=%" PRIu32, newPos);
        mPrevPosition = newPos;

        if ((mPositionLimitEnabled == true) && (mPrevPosition > mMaxPosition)) {
            mPrevPosition = mMaxPosition;
        }

        mStepper.setCurrentPosition(mPrevPosition * mRevolutionsPerPosition * cStepsPerRevolution);
    } else {
        ERROR("can't change position of a running motor");
    }
}

void StepperMotor::overrideCurrentRevolution(const uint32_t newRevolution) {
    if (false == mStepper.isRunning()) {
        TRACE_ARGS("overrideCurrentRevolution: newRevolution=%" PRIu32, newRevolution);
        mStepper.setCurrentPosition(newRevolution * cStepsPerRevolution);
        mPrevPosition = getCurrentPosition();
    } else {
        ERROR("can't change position of a running motor");
    }
}

uint32_t StepperMotor::getCurrentPosition() {
    // NOTE: mRevolutionsPerPosition should come first to make sure expression is calculated as double
    return mStepper.currentPosition() / mRevolutionsPerPosition / cStepsPerRevolution;
}

uint32_t StepperMotor::getCurrentRevolution() {
    return mStepper.currentPosition() / cStepsPerRevolution;
}

void StepperMotor::enablePositionLimit(const bool enable) {
    mPositionLimitEnabled = enable;
}

void StepperMotor::setMaxPosition(const uint32_t maxPos) {
    mMaxPosition = maxPos;
    mRevolutionsPerPosition = static_cast<double>(mLimitRevolutions) / mMaxPosition;
}

void StepperMotor::setRevolutionsLimit(const uint32_t revolutions) {
    TRACE_ARGS("StepperMotor::setRevolutionsLimit: revolutions=%" PRIu32, revolutions);

    mLimitRevolutions = revolutions;
    mRevolutionsPerPosition = static_cast<double>(mLimitRevolutions) / mMaxPosition;
}

// 0 ~ 10
void StepperMotor::setSpeed(const uint32_t level) {
    mSpeedLevel = level;

    if (mSpeedLevel < 0) {
        mSpeedLevel = 0;
    } else if (mSpeedLevel > 10) {
        mSpeedLevel = 10;
    }
}

void StepperMotor::stopMovement(const bool alignPosition) {
    TRACE("StepperMotor::stopMovement");

    // NOTE: spinning and regular moving are exclusive
    if (true == mIsSpinning) {
        forceStop();
        mIsSpinning = false;
    } else if ((true == mEnabled) && (true == mStepper.isRunning())) {
        mStepper.stop();

        int extraSteps = mStepper.currentPosition() % static_cast<int>(cStepsPerRevolution);
        TRACE_ARGS("stopMovement: extraSteps=%d", extraSteps);

        if ((0 != extraSteps) && (true == alignPosition)) {
            setEnabled(true);

            if (extraSteps > (cStepsPerRevolution / 2)) {
                mStepper.move(cStepsPerRevolution - extraSteps);
            } else {
                mStepper.move(-extraSteps);
            }

            // TODO: would me nice to do move using a single function
            mStepper.setSpeed(640 + mSpeedLevel * 36);
        } else {
            mStepper.setCurrentPosition(mStepper.targetPosition());
        }
    } else {
        WARNING("IGNORED. Stepper was not running");
    }
}

void StepperMotor::moveToPosition(const uint32_t pos) {
    TRACE_ARGS("StepperMotor::moveToPosition: %" PRIu32, pos);

    if (pos != getCurrentPosition()) {
        const uint32_t newPos = (((true == mPositionLimitEnabled) && (pos > mMaxPosition)) ? mMaxPosition : pos);

        moveStepperTo(newPos * mRevolutionsPerPosition);
    } else {
        forceStop();
        mListener->onMotorOperationFinished();
    }
}

bool StepperMotor::moveCW(const uint32_t revolutions) {
    TRACE_ARGS("StepperMotor::moveCW: %" PRIu32, revolutions);
    bool res = false;

    if ((false == mPositionLimitEnabled) || ((getCurrentRevolution() + revolutions) <= mLimitRevolutions)) {
        moveStepperTo(getCurrentRevolution() + revolutions);
        res = true;
    }

    return res;
}

bool StepperMotor::moveCCW(const uint32_t revolutions) {
    TRACE_ARGS("StepperMotor::moveCCW: %" PRIu32, revolutions);
    bool res = false;
    uint32_t availableRevolutions = (getCurrentRevolution() >= revolutions ? revolutions : getCurrentRevolution());

    if (availableRevolutions > 0) {
        moveStepperTo(getCurrentRevolution() - revolutions);
        res = true;
    }

    return res;
}

void StepperMotor::startSpinningCW() {
    mStepper.setMaxSpeed(1000);
    mStepper.setSpeed(640);
    mIsSpinning = true;
    setEnabled(true);
}

void StepperMotor::startSpinningCCW() {
    mStepper.setMaxSpeed(-1000);
    mStepper.setSpeed(-640);
    mIsSpinning = true;
    setEnabled(true);
}

void StepperMotor::moveStepperTo(const uint32_t revolutions) {
    setEnabled(true);
    mStepper.moveTo(revolutions * cStepsPerRevolution);
    mStepper.setSpeed(640 + mSpeedLevel * 36);
}

void StepperMotor::forceStop() {
    TRACE("StepperMotor::forceStop");

    mStepper.stop();
    mStepper.setCurrentPosition(mStepper.currentPosition());  // reset target position to stop movement
    mStepper.setSpeed(0);
    setEnabled(false);
}