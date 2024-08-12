#pragma once

#include <string>

#define PREFIX_INFO "[INFO] "
#define PREFIX_WARNING "[WARNING] "
#define PREFIX_ERROR "[ERROR] "

enum LogLevel {
    INFO,
    WARNING,
    ERROR
};

class Logger {
    public:
        static void log(LogLevel level, std::string message);
};

