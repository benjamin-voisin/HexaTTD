#pragma once

#include "raylib.h"

#include <mutex>

class Log {
  private:
    int _log_level;

    static Log *_logPtr;
    static std::mutex _lock;
    Log();

  public:
    Log(const Log &obj) = delete;

    static Log *Get();
    static void Init();

    void print_log_level();
    void set_log_level(int log_level);

    int Info = LOG_INFO;
    int Warning = LOG_WARNING;
    int Fatal = LOG_FATAL;
    int Debug = LOG_DEBUG;
    int Error = LOG_ERROR;
};

void raylib_log(int msgType, const char *text, va_list args);
