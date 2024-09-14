#include "Logger.h"

Logger::Logger(const std::string& filePath) {
    logFile_.open(filePath, std::ios::app);
    if (!logFile_.is_open()) {
        std::cerr << "Unable to open log file: " << filePath << std::endl;
    }
}

Logger::~Logger() {
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

void Logger::log(const std::string& message) {
    if (logFile_.is_open()) {
        logFile_ << message << std::endl;
    }
}
