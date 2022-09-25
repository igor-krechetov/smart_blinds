/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */
#ifndef SMARTCURTAINS_LOGGINGHELPER_HPP
#define SMARTCURTAINS_LOGGINGHELPER_HPP

#include <ArduinoLog.h>

#define TRACE(_msg)                     Log.traceln(F(_msg))
#define WARNING(_msg)                   Log.warningln(F(_msg))
#define ERROR(_msg)                     Log.errorln(F(_msg))
#define NOTICE(_msg)                    Log.noticeln(F(_msg))

#define TRACE_ARGS(_msg, ...)           Log.traceln(F(_msg), __VA_ARGS__)
#define WARNING_ARGS(_msg, ...)         Log.warningln(F(_msg), __VA_ARGS__)
#define ERROR_ARGS(_msg, ...)           Log.errorln(F(_msg), __VA_ARGS__)
#define NOTICE_ARGS(_msg, ...)          Log.noticeln(F(_msg), __VA_ARGS__)


#endif // SMARTCURTAINS_LOGGINGHELPER_HPP