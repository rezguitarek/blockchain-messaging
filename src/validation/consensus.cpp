#include "consensus.hpp"
#include <algorithm>
#include <chrono>

ConsensusManager::ConsensusManager(ConsensusType typeIn, uint32_t requiredValidatorsIn)
    : type(typeIn),
      requiredValidators(requiredValidatorsIn),
      consensusThreshold(75) {
}

bool ConsensusManager::achieveConsensus(const Block& block) {
    if (activeValidators.size() < requiredValidators) {
        return false;
    }
    
    ConsensusRound round;
    round.roundNumber = consensusHistory.size();
    round.blockHash = block.getHash();
    
    // Collect votes from validators
    collectValidatorVotes(block);
    
    // Calculate consensus
    uint32_t positiveVotes = 0;
    for (const auto& [_, vote] : round.validatorVotes) {
        if (vote) positiveVotes++;
    }
    
    double consensusPercentage = (positiveVotes * 100.0) / activeValidators.size();
    round.isComplete = true;
    consensusHistory.push_back(round);
    
    return consensusPercentage >= consensusThreshold;
}

void ConsensusManager::collectValidatorVotes(const Block& block) {
    std::vector<std::future<bool>> validatorFutures;
    
    // Parallel validation
    for (const auto& validator : activeValidators) {
        validatorFutures.push_back(
            std::async(std::launch::async, 
                      &Validator::validateBlock, 
                      validator.get(), 
                      std::ref(block))
        );
    }
    
    // Collect results
    for (size_t i = 0; i < activeValidators.size(); i++) {
        bool vote = validatorFutures[i].get();
        consensusHistory.back().validatorVotes[activeValidators[i]->getAddress()] = vote;
    }
} 