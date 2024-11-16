#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "block.hpp"
#include "../validation/validator.hpp"

class Blockchain {
private:
    std::vector<Block> chain;
    std::vector<Transaction> pendingTransactions;
    std::unordered_map<std::string, double> accountBalances;
    uint32_t difficulty;
    double miningReward;
    
    // Validator pools
    std::vector<std::shared_ptr<Validator>> messageValidators;
    std::vector<std::shared_ptr<Validator>> financialValidators;
    
    // Consensus parameters
    uint32_t consensusThreshold;
    
public:
    Blockchain();
    
    // Core blockchain operations
    void addBlock(Block& block);
    bool isChainValid() const;
    void minePendingTransactions(const std::string& minerAddress);
    
    // Transaction management
    void addTransaction(const Transaction& transaction);
    void processTransaction(const Transaction& transaction);
    
    // Validator management
    void registerValidator(std::shared_ptr<Validator> validator, bool isFinancialValidator);
    void removeValidator(const std::string& validatorAddress);
    double calculateValidatorReward(const std::string& validatorAddress) const;
    
    // Getters
    size_t getChainLength() const { return chain.size(); }
    Block getLatestBlock() const { return chain.back(); }
    double getBalance(const std::string& address) const;
    
    // Consensus methods
    bool validateBlock(const Block& block) const;
    bool reachConsensus(const Block& block) const;
}; 