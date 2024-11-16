#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <sstream>
#include <queue>
#include <thread>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

class Logger {
private:
    static Logger* instance;
    static std::mutex instanceMutex;
    
    std::ofstream logFile;
    std::mutex logMutex;
    LogLevel minLevel;
    bool consoleOutput;
    
    // Async logging
    std::queue<std::string> messageQueue;
    std::thread loggingThread;
    std::mutex queueMutex;
    std::condition_variable queueCondition;
    bool running;
    
    Logger();
    ~Logger();
    
public:
    static Logger* getInstance();
    
    void log(LogLevel level, const std::string& message,
             const std::string& file, int line);
    
    void setMinLevel(LogLevel level) { minLevel = level; }
    void setConsoleOutput(bool enable) { consoleOutput = enable; }
    
    // Async logging methods
    void startAsyncLogging();
    void stopAsyncLogging();
    
private:
    void processLogQueue();
    std::string levelToString(LogLevel level);
    std::string formatMessage(LogLevel level, const std::string& message,
                            const std::string& file, int line);
};

// Macro for easy logging
#define LOG_DEBUG(msg) Logger::getInstance()->log(LogLevel::DEBUG, msg, __FILE__, __LINE__)
#define LOG_INFO(msg) Logger::getInstance()->log(LogLevel::INFO, msg, __FILE__, __LINE__)
#define LOG_WARNING(msg) Logger::getInstance()->log(LogLevel::WARNING, msg, __FILE__, __LINE__)
#define LOG_ERROR(msg) Logger::getInstance()->log(LogLevel::ERROR, msg, __FILE__, __LINE__)
#define LOG_CRITICAL(msg) Logger::getInstance()->log(LogLevel::CRITICAL, msg, __FILE__, __LINE__) 