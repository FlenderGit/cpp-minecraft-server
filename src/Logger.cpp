#include "Logger.hpp"

#include <iostream>

void Logger::log(LogLevel level, std::string message) {
    switch (level) {
        case INFO:
            std::cout << PREFIX_INFO << message << std::endl;
            break;
        case WARNING:
            std::cout << PREFIX_WARNING << message << std::endl;
            break;
        case ERROR:
            std::cout << PREFIX_ERROR << message << std::endl;
            break;
    }
}