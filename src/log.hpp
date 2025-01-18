#pragma once

#include "raylib.h"

#include <string>

namespace Log {
enum loglevel {
    Level_Info = LOG_INFO,
    Level_Warning = LOG_WARNING,
    Level_Fatal = LOG_FATAL,
    Level_Debug = LOG_DEBUG,
    Level_Error = LOG_ERROR,
};

class Logger {
  protected:
    static Log::loglevel _log_level;
    Log::loglevel _level;
    std::string _text;

  public:
    Logger(loglevel level);

    void print_log_level();
    void set_log_level(Log::loglevel log_level);

    void log(const char *text, ...);
    Logger &operator<<(const char *value);
};

extern Logger Info;
extern Logger Warning;
extern Logger Fatal;
extern Logger Debug;
extern Logger Error;
}; // namespace Log

void raylib_log(int msgType, const char *text, va_list args);
