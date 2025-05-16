#pragma once

#include "raylib.h"

#include <string>

#if __has_attribute(format) || defined(__GNUC__)
#define PRINTF_FORMAT(fmt, args) __attribute__((__format__(printf, fmt, args)))
#else
#define PRINTF_FORMAT(fmt, args)
#endif

using std::string;

namespace Log {
const string ansi_reset = "\033[0m";
const string ansi_color_red = "\033[31m";
const string ansi_color_red_bold = "\033[1;31m";
const string ansi_color_green = "\033[32m";
const string ansi_color_yellow = "\033[33m";
const string ansi_color_magenta = "\033[35m";
const string ansi_color_magenta_bold = "\033[1;35m";
const string ansi_color_cyan = "\033[36m";
const string ansi_color_light_gray = "\033[37m";
enum loglevel {
    Level_Info = LOG_INFO,
    Level_Warning = LOG_WARNING,
    Level_Fatal = LOG_FATAL,
    Level_Debug = LOG_DEBUG,
    Level_Error = LOG_ERROR,
};

class Logger {
  private:
    static Log::loglevel _log_level;
    Log::loglevel _level;
    string _text;
    char *_buffer;
    size_t _buffer_size;

  public:
    Logger(loglevel level);
    ~Logger();

    void print_log_level();
    void set_log_level(Log::loglevel log_level);

    PRINTF_FORMAT(2, 3) void log(const char *text, ...);
    void vlog(const char *text, va_list args);
    Logger &operator<<(const char *value);
};

extern Logger Info;
extern Logger Warning;
extern Logger Fatal;
extern Logger Debug;
extern Logger Error;
}; // namespace Log

void raylib_log(int msgType, const char *text, va_list args);
