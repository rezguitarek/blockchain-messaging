#pragma once
#include <string>
#include <exception>
#include <functional>
#include "logger.hpp"

class BlockchainException : public std::exception {
private:
    std::string message;
    std::string details;
    int errorCode;
    
public:
    BlockchainException(const std::string& msg, int code, const std::string& detail = "")
        : message(msg), details(detail), errorCode(code) {}
        
    const char* what() const noexcept override {
        return message.c_str();
    }
    
    int getErrorCode() const { return errorCode; }
    std::string getDetails() const { return details; }
};

class ErrorHandler {
public:
    static void handleError(const BlockchainException& e,
                          std::function<void(const BlockchainException&)> callback = nullptr) {
        LOG_ERROR("Error " + std::to_string(e.getErrorCode()) + ": " + e.what());
        
        if (!e.getDetails().empty()) {
            LOG_DEBUG("Error details: " + e.getDetails());
        }
        
        if (callback) {
            callback(e);
        }
    }
    
    static void handleStandardException(const std::exception& e,
                                      const std::string& context) {
        LOG_ERROR(context + ": " + e.what());
    }
};

// Error codes
enum BlockchainErrorCode {
    INVALID_TRANSACTION = 1001,
    INVALID_BLOCK = 1002,
    CONSENSUS_FAILURE = 1003,
    NETWORK_ERROR = 1004,
    VALIDATION_ERROR = 1005,
    CONTRACT_ERROR = 1006
}; 