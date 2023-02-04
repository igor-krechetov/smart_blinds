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

    void start(const ConfigurationManager& config);
    void processEvents();

private:
    IUpdateListener* mListener = nullptr;
};

#endif  // UPDATEMANAGER_HPP