#include "blockchain.hpp"
#include <stdexcept>
#include <algorithm>

Blockchain::Blockchain() 
    : difficulty(4),
      miningReward(100),
      consensusThreshold(75) {
    // Create genesis block
    std::vector<Transaction> genesisTransactions;
    chain.emplace_back(Block(0, genesisTransactions, "0"));
}

void Blockchain::addBlock(Block& block) {
    if (!validateBlock(block)) {
        throw std::runtime_error("Invalid block");
    }
    
    if (!reachConsensus(block)) {
        throw std::runtime_error("Consensus not reached");
    }
    
    chain.push_back(block);
    
    // Process all transactions in the block
    for (const auto& transaction : block.getTransactions()) {
        processTransaction(transaction);
    }
}

void Blockchain::processTransaction(const Transaction& transaction) {
    if (transaction.getType() == TransactionType::FINANCIAL) {
        std::string sender = transaction.getSender();
        std::string recipient = transaction.getRecipient();
        double amount = transaction.getAmount();
        
        // Update balances
        accountBalances[sender] -= amount;
        accountBalances[recipient] += amount;
    }
}

bool Blockchain::validateBlock(const Block& block) const {
    // Basic validation
    if (!block.isValid()) return false;
    
    // Validate previous hash
    if (block.getPreviousHash() != getLatestBlock().getHash()) return false;
    
    // Validate transactions
    for (const auto& transaction : block.getTransactions()) {
        if (!transaction.isValid()) return false;
        
        // Additional validation for financial transactions
        if (transaction.getType() == TransactionType::FINANCIAL) {
            if (getBalance(transaction.getSender()) < transaction.getAmount()) {
                return false;
            }
        }
    }
    
    return true;
}

bool Blockchain::reachConsensus(const Block& block) const {
    size_t validatorCount = 0;
    size_t approvalCount = 0;
    
    // Get appropriate validator pool based on transaction types
    bool hasFinancialTx = std::any_of(
        block.getTransactions().begin(),
        block.getTransactions().end(),
        [](const Transaction& tx) { 
            return tx.getType() == TransactionType::FINANCIAL; 
        }
    );
    
    const auto& validators = hasFinancialTx ? financialValidators : messageValidators;
    
    // Collect validator votes
    for (const auto& validator : validators) {
        if (validator->validateBlock(block)) {
            approvalCount++;
        }
        validatorCount++;
    }
    
    // Calculate consensus percentage
    double consensusPercentage = (approvalCount * 100.0) / validatorCount;
    return consensusPercentage >= consensusThreshold;
} 