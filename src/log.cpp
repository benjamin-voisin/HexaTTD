#include "log.hpp"

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
        return "[DEBUG]";
    case Log::Level_Info:
        return "[INFO]";
    case Log::Level_Fatal:
        return "[FATAL]";
    case Log::Level_Warning:
        return "[WARNING]";
    case Log::Level_Error:
        return "[ERROR]";
    default:
        return "[?]";
    }
}

Logger::Logger(loglevel level) {
    _text = level_to_string(level);
    _level = level;
}

Logger &Logger::operator<<(const char *value) {
    log(value);
    return *this;
}

void Logger::log(const char *format, ...) {
    va_list args;
    char timeStr[64] = {0};
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);

    char text[100];
    snprintf(text, 100, format, args);

    std::cout << "[" << timeStr << "][" << _text << "] " << text << std::endl;
}

void raylib_log(int msgType, const char *text, va_list args) {

    vprintf(text, args);

    switch (msgType) {
    case LOG_INFO:
        Log::Info << text;
        break;
    case LOG_ERROR:
        Log::Error << text;
        break;
    case LOG_WARNING:
        Log::Warning << text;
        break;
    case LOG_DEBUG:
        Log::Debug << text;
        break;
    default:
        break;
    }
}

void Logger::print_log_level() { std::cout << _log_level << std::endl; }

void Logger::set_log_level(Log::loglevel log_level) { _log_level = log_level; }