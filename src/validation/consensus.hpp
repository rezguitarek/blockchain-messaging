#pragma once
#include <vector>
#include <memory>
#include "../core/block.hpp"
#include "validator.hpp"

enum class ConsensusType {
    PROOF_OF_PARTICIPATION,
    PROOF_OF_STAKE,
    HYBRID
};

class ConsensusManager {
private:
    ConsensusType type;
    uint32_t requiredValidators;
    uint32_t consensusThreshold;
    std::vector<std::shared_ptr<Validator>> activeValidators;
    
    struct ConsensusRound {
        uint32_t roundNumber;
        std::string blockHash;
        std::unordered_map<std::string, bool> validatorVotes;
        bool isComplete;
    };
    
    std::vector<ConsensusRound> consensusHistory;
    
public:
    ConsensusManager(ConsensusType typeIn, uint32_t requiredValidatorsIn);
    
    // Consensus operations
    bool achieveConsensus(const Block& block);
    void collectValidatorVotes(const Block& block);
    bool finalizeBlock(const Block& block);
    
    // Validator management
    void registerValidator(std::shared_ptr<Validator> validator);
    void removeValidator(const std::string& validatorAddress);
    
    // Consensus metrics
    double getConsensusRate() const;
    uint32_t getActiveValidatorCount() const;
}; 