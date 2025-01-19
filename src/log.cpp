#include "log.hpp"

#include <assert.h>
#include <cstdio>
#include <iostream>

using Log::Logger;

#ifdef DEBUG
Log::loglevel Logger::_log_level = Log::Level_Debug;
#else
Log::loglevel Logger::_log_level = Log::Level_Info;
#endif // DEBUG

Logger Log::Info = Logger(Log::Level_Info);
Logger Log::Warning = Logger(Log::Level_Warning);
Logger Log::Fatal = Logger(Log::Level_Fatal);
Logger Log::Debug = Logger(Log::Level_Debug);
Logger Log::Error = Logger(Log::Level_Error);

std::string level_to_string(Log::loglevel level) {
    switch (level) {
    case Log::Level_Debug:
        return Log::ansi_color_cyan + "[DEBUG]" + Log::ansi_reset;
    case Log::Level_Info:
        return Log::ansi_color_green + "[INFO]" + Log::ansi_reset;
    case Log::Level_Fatal:
        return Log::ansi_color_red + "[FATAL]" + Log::ansi_reset;
    case Log::Level_Warning:
        return Log::ansi_color_yellow + "[WARNING]" + Log::ansi_reset;
    case Log::Level_Error:
        return Log::ansi_color_red + "[ERROR]" + Log::ansi_reset;
    default:
        return "[?]";
    }
}

Logger::Logger(loglevel level) {
    _text = level_to_string(level);
    _level = level;
    _buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
}

Logger::~Logger() { free(_buffer); }

Logger &Logger::operator<<(const char *value) {
    log(value);
    return *this;
}

void Logger::log(const char *format, ...) {
    va_list args;
    char timeStr[11] = {0};
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(timeStr, sizeof(timeStr), "[%H:%M:%S]", tm_info);

    snprintf(_buffer, BUFFER_SIZE, format, args);

    std::cout << timeStr << _text << " " << _buffer << std::endl;
}

void raylib_log(int msgType, const char *text, va_list args) {

    /* vprintf(text, args); */

    switch (msgType) {
    case LOG_INFO:
        /* Log::Info << text; */
        Log::Info.log(text, args);
        break;
    case LOG_ERROR:
        /* Log::Error << text; */
        Log::Error.log(text, args);
        break;
    case LOG_WARNING:
        /* Log::Warning << text; */
        Log::Warning.log(text, args);
        break;
    case LOG_DEBUG:
        /* Log::Debug << text; */
        Log::Debug.log(text, args);
        break;
    default:
        assert(false);
        break;
    }
}

void Logger::print_log_level() { std::cout << _log_level << std::endl; }

void Logger::set_log_level(Log::loglevel log_level) { _log_level = log_level; }
