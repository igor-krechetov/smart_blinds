/*
 * Copyright (C) 2022 Igor Krechetov
 * Distributed under MIT License. See file LICENSE for details (https://opensource.org/licenses/MIT)
 */
#ifndef SMARTCURTAINS_LOGGINGHELPER_HPP
#define SMARTCURTAINS_LOGGINGHELPER_HPP

#include <ArduinoLog.h>

#ifndef DISABLE_LOGGING
  #define TRACE(_msg)                     Log.traceln(F(_msg))
  #define WARNING(_msg)                   Log.warningln(F(_msg))
  #define ERROR(_msg)                     Log.errorln(F(_msg))
  #define NOTICE(_msg)                    Log.noticeln(F(_msg))
  
  #define TRACE_ARGS(_msg, ...)           Log.traceln(F(_msg), __VA_ARGS__)
  #define WARNING_ARGS(_msg, ...)         Log.warningln(F(_msg), __VA_ARGS__)
  #define ERROR_ARGS(_msg, ...)           Log.errorln(F(_msg), __VA_ARGS__)
  #define NOTICE_ARGS(_msg, ...)          Log.noticeln(F(_msg), __VA_ARGS__)
  
  #define PRINT_FREE_HEAP()               TRACE_ARGS("FREE HEAP: %" PRIu32 ", MAX BLOCK: %" PRIu32 ", Fragmentation: %d\n", ESP.getFreeHeap(), ESP.getMaxFreeBlockSize(), (int)ESP.getHeapFragmentation())
#else
  #define TRACE(_msg)
  #define WARNING(_msg)
  #define ERROR(_msg)
  #define NOTICE(_msg)
  
  #define TRACE_ARGS(_msg, ...)
  #define WARNING_ARGS(_msg, ...)
  #define ERROR_ARGS(_msg, ...)
  #define NOTICE_ARGS(_msg, ...)
  
  #define PRINT_FREE_HEAP()               Serial.printf("FREE HEAP: %" PRIu32 ", MAX BLOCK: %" PRIu32 ", Fragmentation: %d\n", ESP.getFreeHeap(), ESP.getMaxFreeBlockSize(), (int)ESP.getHeapFragmentation())
#endif

#endif // SMARTCURTAINS_LOGGINGHELPER_HPP