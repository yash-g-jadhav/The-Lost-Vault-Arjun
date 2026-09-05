#include "core/Log.h"
#include <iostream>

LogLevel Log::s_currentLevel = LogLevel::Info;

void Log::Init() {
    s_currentLevel = LogLevel::Info;
}

void Log::SetLogLevel(LogLevel level) {
    s_currentLevel = level;
}

void Log::LogMessage(LogLevel level, const char* category, const std::string& message) {
    if (level < s_currentLevel) {
        return;
    }

    const char* levelStr = "INFO";
    switch (level) {
        case LogLevel::Debug: levelStr = "DEBUG"; break;
        case LogLevel::Info:  levelStr = "INFO";  break;
        case LogLevel::Warn:  levelStr = "WARN";  break;
        case LogLevel::Error: levelStr = "ERROR"; break;
    }

    if (level == LogLevel::Error) {
        std::cerr << "[" << levelStr << "][" << category << "] " << message << std::endl;
    } else {
        std::cout << "[" << levelStr << "][" << category << "] " << message << std::endl;
    }
}
