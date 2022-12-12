#pragma once

#include "logger.h"

Logger::LogLevel Logger::_level = Logger::LogLevel::LOCALLOGLEVEL;

void Logger::setLogLevel(LogLevel level) {
    _level = level;
}

void Logger::log(LogLevel level, std::string message) {
    if (level < _level) return;

    switch (level) {
        case DEBUG:
            std::cout << "[DEBUG] " << message << std::endl;
            break;
        case INFO:
            std::cout << "[INFO] " << message << std::endl;
            break;
        case WARNING:
            std::cout << "[WARNING] " << message << std::endl;
            break;
        case ERR:
            std::cout << "[ERROR] " << message << std::endl;
            break;
    }
}

void Logger::debug(std::string message) {
    log(DEBUG, message);
}

void Logger::info(std::string message) {
    log(INFO, message);
}

void Logger::warning(std::string message) {
    log(WARNING, message);
}

void Logger::error(std::string message) {
    log(ERR, message);
}
