#pragma once
#include <string>
#include <vector>
#include "../core/block.hpp"

enum class ValidatorType {
    MESSAGE,
    FINANCIAL,
    HYBRID
};

class Validator {
private:
    std::string address;
    ValidatorType type;
    double stakingAmount;
    uint32_t reputation;
    std::vector<Block> validatedBlocks;
    
    // Hardware capabilities
    struct HardwareSpecs {
        uint32_t cpuPower;
        uint64_t memory;
        uint32_t bandwidth;
    } specs;
    
public:
    Validator(const std::string& addressIn, ValidatorType typeIn);
    
    // Validation methods
    bool validateBlock(const Block& block) const;
    bool validateTransaction(const Transaction& transaction) const;
    
    // Staking methods
    void stake(double amount);
    void unstake(double amount);
    
    // Reputation management
    void updateReputation(bool successfulValidation);
    uint32_t getReputation() const { return reputation; }
    
    // Hardware verification
    bool verifyHardwareCapabilities() const;
    void updateHardwareSpecs(const HardwareSpecs& newSpecs);
    
    // Getters
    std::string getAddress() const { return address; }
    ValidatorType getType() const { return type; }
    double getStakingAmount() const { return stakingAmount; }
}; 