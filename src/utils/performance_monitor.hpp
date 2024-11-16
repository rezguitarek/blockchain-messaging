#pragma once
#include <chrono>
#include <string>
#include <unordered_map>
#include <mutex>

class PerformanceMonitor {
private:
    struct Metric {
        double averageTime;
        uint64_t callCount;
        double minTime;
        double maxTime;
    };
    
    static std::unordered_map<std::string, Metric> metrics;
    static std::mutex metricsMutex;
    
public:
    class ScopedTimer {
    private:
        std::string operation;
        std::chrono::high_resolution_clock::time_point start;
        
    public:
        ScopedTimer(const std::string& op) 
            : operation(op), 
              start(std::chrono::high_resolution_clock::now()) {}
        
        ~ScopedTimer() {
            auto duration = std::chrono::high_resolution_clock::now() - start;
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            PerformanceMonitor::recordMetric(operation, ms);
        }
    };
    
    static void recordMetric(const std::string& operation, double timeMs);
    static Metric getMetric(const std::string& operation);
    static void reset();
}; 