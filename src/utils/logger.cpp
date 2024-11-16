#include "logger.hpp"
#include <ctime>
#include <iomanip>

Logger* Logger::instance = nullptr;
std::mutex Logger::instanceMutex;

Logger::Logger() 
    : minLevel(LogLevel::INFO),
      consoleOutput(true),
      running(false) {
    logFile.open("blockchain.log", std::ios::app);
    startAsyncLogging();
}

Logger::~Logger() {
    stopAsyncLogging();
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger* Logger::getInstance() {
    if (instance == nullptr) {
        std::lock_guard<std::mutex> lock(instanceMutex);
        if (instance == nullptr) {
            instance = new Logger();
        }
    }
    return instance;
}

void Logger::log(LogLevel level, const std::string& message,
                const std::string& file, int line) {
    if (level < minLevel) return;
    
    std::string formattedMessage = formatMessage(level, message, file, line);
    
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        messageQueue.push(formattedMessage);
    }
    queueCondition.notify_one();
    
    if (consoleOutput) {
        std::cout << formattedMessage << std::endl;
    }
}

void Logger::processLogQueue() {
    while (running) {
        std::unique_lock<std::mutex> lock(queueMutex);
        queueCondition.wait(lock, [this] { 
            return !messageQueue.empty() || !running; 
        });
        
        while (!messageQueue.empty()) {
            std::string message = messageQueue.front();
            messageQueue.pop();
            
            std::lock_guard<std::mutex> filelock(logMutex);
            logFile << message << std::endl;
        }
    }
}

std::string Logger::formatMessage(LogLevel level, const std::string& message,
                                const std::string& file, int line) {
    std::stringstream ss;
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
       << " [" << levelToString(level) << "] "
       << file << ":" << line << " - "
       << message;
    
    return ss.str();
} 