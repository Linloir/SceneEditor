#pragma once

#include <iostream>

#define LOCALLOGLEVEL DEBUG

class Logger {
public:
    enum LogLevel { DEBUG, INFO, WARNING, ERROR };
    
private:
    static LogLevel _level;
    
public:
    static void setLogLevel(LogLevel level);
    static void log(LogLevel level, std::string message);
    static void debug(std::string message);
    static void info(std::string message);
    static void warning(std::string message);
    static void error(std::string message);
};
