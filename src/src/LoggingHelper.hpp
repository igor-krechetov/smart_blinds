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
#ifndef SMARTCURTAINS_LOGGINGHELPER_HPP
#define SMARTCURTAINS_LOGGINGHELPER_HPP

extern unsigned long gStackAddressBegin;
unsigned long getLatestStackAdress();

#if !defined(DISABLE_APP_LOGGING)
    #include <ArduinoLog.h>

    #define TRACE(_msg) Log.traceln(F(_msg))
    #define WARNING(_msg) Log.warningln(F(_msg))
    #define ERROR(_msg) Log.errorln(F(_msg))
    #define NOTICE(_msg) Log.noticeln(F(_msg))

    #define TRACE_ARGS(_msg, ...) Log.traceln(F(_msg), __VA_ARGS__)
    #define WARNING_ARGS(_msg, ...) Log.warningln(F(_msg), __VA_ARGS__)
    #define ERROR_ARGS(_msg, ...) Log.errorln(F(_msg), __VA_ARGS__)
    #define NOTICE_ARGS(_msg, ...) Log.noticeln(F(_msg), __VA_ARGS__)

    #define PRINT_FREE_HEAP()                                                                                                  \
        TRACE_ARGS("F<%s>, L<%d>, USED STACK: %" PRIu32 ", FREE HEAP: %" PRIu32 ", MAX BLOCK: %" PRIu32 ", Fragmentation: %d", \
                   __FILE__,                                                                                                   \
                   __LINE__,                                                                                                   \
                   (uint32_t)(gStackAddressBegin - getLatestStackAdress()),                                                    \
                   ESP.getFreeHeap(),                                                                                          \
                   ESP.getMaxFreeBlockSize(),                                                                                  \
                   (int)ESP.getHeapFragmentation())
#else
    #define TRACE(_msg)
    #define WARNING(_msg)
    #define ERROR(_msg)
    #define NOTICE(_msg)

    #define TRACE_ARGS(_msg, ...)
    #define WARNING_ARGS(_msg, ...)
    #define ERROR_ARGS(_msg, ...)
    #define NOTICE_ARGS(_msg, ...)

    #define PRINT_FREE_HEAP()                                                                              \
        Serial.printf("F<%s>, L<%d>, USED STACK: %" PRIu32 ", FREE HEAP: %" PRIu32 ", MAX BLOCK: %" PRIu32 \
                      ", Fragmentation: %d\n",                                                             \
                      __FILE__,                                                                            \
                      __LINE__,                                                                            \
                      (uint32_t)(gStackAddressBegin - getLatestStackAdress()),                             \
                      ESP.getFreeHeap(),                                                                   \
                      ESP.getMaxFreeBlockSize(),                                                           \
                      (int)ESP.getHeapFragmentation())
#endif

#endif  // SMARTCURTAINS_LOGGINGHELPER_HPP