#include "validator.hpp"
#include <stdexcept>

Validator::Validator(const std::string& addressIn, ValidatorType typeIn)
    : address(addressIn),
      type(typeIn),
      stakingAmount(0),
      reputation(100) {
    verifyHardwareCapabilities();
}

bool Validator::validateBlock(const Block& block) const {
    // Basic block validation
    if (!block.isValid()) return false;
    
    // Validate all transactions in the block
    for (const auto& transaction : block.getTransactions()) {
        if (!validateTransaction(transaction)) {
            return false;
        }
    }
    
    // Additional validation based on validator type
    if (type == ValidatorType::FINANCIAL || type == ValidatorType::HYBRID) {
        // Perform more intensive validation for financial transactions
        // ...
    }
    
    return true;
}

bool Validator::validateTransaction(const Transaction& transaction) const {
    // Basic transaction validation
    if (!transaction.isValid()) return false;
    
    // Type-specific validation
    switch (transaction.getType()) {
        case TransactionType::FINANCIAL:
            if (type == ValidatorType::MESSAGE) return false;
            // Validate financial transaction
            break;
            
        case TransactionType::MESSAGE:
            // Validate message transaction
            break;
            
        case TransactionType::REWARD:
            // Validate reward transaction
            break;
    }
    
    return true;
}

bool Validator::verifyHardwareCapabilities() const {
    switch (type) {
        case ValidatorType::FINANCIAL:
        case ValidatorType::HYBRID:
            // Check for higher hardware requirements
            return specs.cpuPower >= 8000 && 
                   specs.memory >= 16384 && 
                   specs.bandwidth >= 100;
            
        case ValidatorType::MESSAGE:
            // Basic hardware requirements
            return specs.cpuPower >= 2000 && 
                   specs.memory >= 4096 && 
                   specs.bandwidth >= 20;
    }
    return false;
}

void Validator::stake(double amount) {
    if (amount <= 0) {
        throw std::invalid_argument("Staking amount must be positive");
    }
    stakingAmount += amount;
}

void Validator::updateReputation(bool successfulValidation) {
    if (successfulValidation) {
        reputation = std::min(reputation + 1, 100u);
    } else {
        reputation = reputation > 5 ? reputation - 5 : 0;
    }
} 