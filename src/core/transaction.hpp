#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../crypto/hash.hpp"

enum class TransactionStatus {
    PENDING,
    CONFIRMED,
    FAILED,
    EXPIRED
};

class TransactionInput {
public:
    std::string previousTxHash;
    uint32_t outputIndex;
    std::string signature;
    std::string publicKey;
    
    bool verify() const;
    std::string getHash() const;
};

class TransactionOutput {
public:
    std::string recipient;
    double amount;
    std::string scriptPubKey;
    
    bool isSpent;
    std::string getHash() const;
};

class Transaction {
private:
    std::string hash;
    std::vector<TransactionInput> inputs;
    std::vector<TransactionOutput> outputs;
    uint32_t lockTime;
    TransactionStatus status;
    time_t timestamp;
    
    // Message-specific fields
    std::string encryptedMessage;
    std::string messageRecipient;
    
    // Smart contract interaction fields
    std::string contractAddress;
    std::string methodSignature;
    std::vector<std::string> parameters;
    
public:
    Transaction(TransactionType type);
    Transaction(const std::string& sender, const std::string& recipient, TransactionType type);
    
    // Core transaction methods
    void addInput(const TransactionInput& input);
    void addOutput(const TransactionOutput& output);
    bool sign(const std::string& privateKey);
    bool verify() const;
    
    // Message methods
    void setMessage(const std::string& message, const std::string& recipientPublicKey);
    std::string decryptMessage(const std::string& recipientPrivateKey) const;
    
    // Smart contract methods
    void setContractCall(const std::string& contractAddr, 
                        const std::string& method,
                        const std::vector<std::string>& params);
    
    // Getters
    std::string getHash() const { return hash; }
    TransactionStatus getStatus() const { return status; }
    double getTotalInput() const;
    double getTotalOutput() const;
    
    // Validation
    bool isValid() const;
    bool hasValidFee() const;
}; 