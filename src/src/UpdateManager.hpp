/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */
#ifndef UPDATEMANAGER_HPP
#define UPDATEMANAGER_HPP

#include <Arduino.h>
#include "ConfigurationManager.hpp"

class IUpdateListener {
public:
    virtual void onUpdateStarted() = 0;
    virtual void onUpdateReadyToReboot() = 0;
    virtual void onUpdateFailed() = 0;
};

class UpdateManager {
public:
    UpdateManager(IUpdateListener* listener);

    void start(const ConfigurationManager &config);
    void processEvents();

private:
    IUpdateListener* mListener = nullptr;
};

#endif // UPDATEMANAGER_HPP