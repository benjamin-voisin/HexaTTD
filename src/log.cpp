#include "log.hpp"

#include <assert.h>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdio.h>

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
    size_t buffer_size = 100; // buffer start value, it will be resized if
                              // needed
    _buffer = static_cast<char *>(malloc(buffer_size * sizeof(char)));
    _buffer_size = buffer_size;
}

Logger::~Logger() { free(_buffer); }

Logger &Logger::operator<<(const char *value) {
    log("%s", value);
    return *this;
}

void Logger::vlog(const char *format, va_list args) {
    if (_level >= _log_level) {
        char timeStr[11] = {0};
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        strftime(timeStr, sizeof(timeStr), "[%H:%M:%S]", tm_info);
        memset(_buffer, 0, _buffer_size);
        auto written = vsnprintf(_buffer, _buffer_size, format, args);

        // If the buffer was not big enough, we expand it
        if (written > static_cast<int>(_buffer_size)) {
            // We realloc the buffer
            _buffer_size = written;
            _buffer = (char *)realloc(_buffer, _buffer_size * sizeof(char));

            // And we re-do the vsnprintf
            memset(_buffer, 0, _buffer_size);
            vsnprintf(_buffer, _buffer_size, format, args);
        }

        std::cout << timeStr << _text << " " << _buffer << std::endl;
    }
}

void Logger::log(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vlog(format, args);
    va_end(args);
}

void raylib_log(int msgType, const char *text, va_list args) {

    switch (msgType) {
    case LOG_INFO:
        Log::Info.vlog(text, args);
        break;
    case LOG_ERROR:
        Log::Error.vlog(text, args);
        break;
    case LOG_WARNING:
        Log::Warning.vlog(text, args);
        break;
    case LOG_DEBUG:
        Log::Debug.vlog(text, args);
        break;
    default:
        assert(false);
        break;
    }
}

void Logger::print_log_level() { std::cout << _log_level << std::endl; }

void Logger::set_log_level(Log::loglevel log_level) { _log_level = log_level; }
