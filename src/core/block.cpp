#include "block.hpp"
#include <sstream>

Block::Block(uint32_t indexIn, const std::vector<Transaction>& transactionsIn, const std::string& previousHashIn) 
    : index(indexIn), 
      transactions(transactionsIn), 
      previousHash(previousHashIn),
      timestamp(std::time(nullptr)),
      nonce(0) {
    hash = calculateHash();
}

std::string Block::calculateHash() const {
    std::stringstream ss;
    ss << index << timestamp << previousHash;
    
    for (const Transaction& transaction : transactions) {
        ss << transaction.getHash();
    }
    ss << nonce;
    
    return SHA256::hash(ss.str());
}

void Block::mineBlock(uint32_t difficulty) {
    std::string target(difficulty, '0');
    
    while (hash.substr(0, difficulty) != target) {
        nonce++;
        hash = calculateHash();
    }
}

bool Block::isValid() const {
    // Verify block integrity
    if (calculateHash() != hash) return false;
    
    // Verify all transactions
    for (const Transaction& tx : transactions) {
        if (!tx.isValid()) return false;
    }
    
    return true;
} 