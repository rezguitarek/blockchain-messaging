#pragma once
#include <string>
#include <vector>
#include <ctime>
#include "../crypto/hash.hpp"

class Block {
private:
    uint32_t index;
    time_t timestamp;
    std::string previousHash;
    std::string hash;
    std::vector<Transaction> transactions;
    uint32_t nonce;
    
public:
    Block(uint32_t indexIn, const std::vector<Transaction>& transactionsIn, const std::string& previousHashIn);
    
    // Core functionality
    std::string calculateHash() const;
    void mineBlock(uint32_t difficulty);
    
    // Getters
    std::string getHash() const { return hash; }
    std::string getPreviousHash() const { return previousHash; }
    std::vector<Transaction> getTransactions() const { return transactions; }
    time_t getTimestamp() const { return timestamp; }
    
    // Validation
    bool isValid() const;
}; 