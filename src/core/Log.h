#pragma once

#include <string>
#include <iostream>
#include <sstream>

enum class LogLevel {
    Debug,
    Info,
    Warn,
    Error
};

class Log {
public:
    static void Init();
    static void LogMessage(LogLevel level, const char* category, const std::string& message);
    static void SetLogLevel(LogLevel level);

private:
    static LogLevel s_currentLevel;
};

#define LOG_DEBUG(category, message) \
    do { \
        std::ostringstream ss; \
        ss << message; \
        Log::LogMessage(LogLevel::Debug, category, ss.str()); \
    } while(0)

#define LOG_INFO(category, message) \
    do { \
        std::ostringstream ss; \
        ss << message; \
        Log::LogMessage(LogLevel::Info, category, ss.str()); \
    } while(0)

#define LOG_WARN(category, message) \
    do { \
        std::ostringstream ss; \
        ss << message; \
        Log::LogMessage(LogLevel::Warn, category, ss.str()); \
    } while(0)

#define LOG_ERROR(category, message) \
    do { \
        std::ostringstream ss; \
        ss << message; \
        Log::LogMessage(LogLevel::Error, category, ss.str()); \
    } while(0)
