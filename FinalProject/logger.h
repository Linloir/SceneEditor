#pragma once

#include <iostream>

#define LOCALLOGLEVEL DEBUG

class Logger {
public:
    enum LogLevel { DEBUG, INFO, WARNING, ERR };
    
private:
    static LogLevel _level;
    static std::string _previous;
    
public:
    static void setLogLevel(LogLevel level);
    static void log(LogLevel level, std::string message);
    static void debug(std::string message);
    static void info(std::string message);
    static void warning(std::string message);
    static void error(std::string message);
};
