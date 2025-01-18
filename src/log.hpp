#pragma once

#include "raylib.h"

#include <string>

namespace Log {
const std::string ansi_reset = "\033[0m";
const std::string ansi_color_red = "\033[31m";
const std::string ansi_color_red_bold = "\033[1;31m";
const std::string ansi_color_green = "\033[32m";
const std::string ansi_color_yellow = "\033[33m";
const std::string ansi_color_magenta = "\033[35m";
const std::string ansi_color_magenta_bold = "\033[1;35m";
const std::string ansi_color_cyan = "\033[36m";
const std::string ansi_color_light_gray = "\033[37m";
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
