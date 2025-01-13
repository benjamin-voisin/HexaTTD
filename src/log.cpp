#include "log.hpp"
#include <mutex>

#include <iostream>

Log::Log() : _log_level{0} {}

void Log::Init() { _logPtr = nullptr; }

void raylib_log(int msgType, const char *text, va_list args) {
    /* char timeStr[64] = { 0 }; */
    /* time_t now = time(NULL); */
    /* struct tm *tm_info = localtime(&now); */

    /* strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info); */
    /* printf("[%s] ", timeStr); */

    /* switch (msgType) */
    /* { */
    /*     case LOG_INFO: printf("[INFO] : "); break; */
    /*     case LOG_ERROR: printf("[ERROR]: "); break; */
    /*     case LOG_WARNING: printf("[WARN] : "); break; */
    /*     case LOG_DEBUG: printf("[DEBUG]: "); break; */
    /*     default: break; */
    /* } */

    /* vprintf(text, args); */
    Log::Get()->print_log_level();
}

Log *Log::Get() {
    if (_logPtr == nullptr) {
        _lock.lock();
        if (_logPtr == nullptr) {
            _logPtr = new Log;
        }
        _lock.unlock();
    }
    return _logPtr;
}

void Log::print_log_level() { std::cout << _log_level << std::endl; }

void Log::set_log_level(int log_level) { _log_level = log_level; }
