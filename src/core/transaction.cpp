#include "transaction.hpp"
#include "../crypto/encryption.hpp"
#include <sstream>
#include <stdexcept>

Transaction::Transaction(TransactionType type)
    : status(TransactionStatus::PENDING),
      timestamp(std::time(nullptr)),
      lockTime(0) {
    hash = calculateHash();
}

Transaction::Transaction(const std::string& sender, 
                       const std::string& recipient, 
                       TransactionType type)
    : Transaction(type) {
    // Create basic output
    TransactionOutput output;
    output.recipient = recipient;
    output.isSpent = false;
    outputs.push_back(output);
}

std::string Transaction::calculateHash() const {
    std::stringstream ss;
    ss << timestamp;
    
    for (const auto& input : inputs) {
        ss << input.getHash();
    }
    
    for (const auto& output : outputs) {
        ss << output.getHash();
    }
    
    ss << lockTime << static_cast<int>(status);
    
    if (!encryptedMessage.empty()) {
        ss << encryptedMessage << messageRecipient;
    }
    
    if (!contractAddress.empty()) {
        ss << contractAddress << methodSignature;
        for (const auto& param : parameters) {
            ss << param;
        }
    }
    
    return SHA256::doubleHash(ss.str());
}

bool Transaction::sign(const std::string& privateKey) {
    try {
        std::string message = calculateHash();
        std::vector<uint8_t> signature = Encryption::sign(message, privateKey);
        
        for (auto& input : inputs) {
            input.signature = std::string(signature.begin(), signature.end());
            input.publicKey = Encryption::generatePublicKey(privateKey);
        }
        
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

bool Transaction::verify() const {
    // Verify basic transaction structure
    if (inputs.empty() || outputs.empty()) {
        return false;
    }
    
    // Verify total input >= total output
    if (getTotalInput() < getTotalOutput()) {
        return false;
    }
    
    // Verify all inputs
    for (const auto& input : inputs) {
        if (!input.verify()) {
            return false;
        }
    }
    
    // Verify signatures
    for (const auto& input : inputs) {
        std::string message = calculateHash();
        std::vector<uint8_t> signature(input.signature.begin(), input.signature.end());
        
        if (!Encryption::verify(message, signature, input.publicKey)) {
            return false;
        }
    }
    
    return true;
}

void Transaction::setMessage(const std::string& message, 
                           const std::string& recipientPublicKey) {
    std::vector<uint8_t> encrypted = Encryption::encrypt(message, recipientPublicKey);
    encryptedMessage = std::string(encrypted.begin(), encrypted.end());
    messageRecipient = recipientPublicKey;
}

std::string Transaction::decryptMessage(const std::string& recipientPrivateKey) const {
    if (encryptedMessage.empty()) {
        throw std::runtime_error("No encrypted message found");
    }
    
    std::vector<uint8_t> encrypted(encryptedMessage.begin(), encryptedMessage.end());
    return Encryption::decrypt(encrypted, recipientPrivateKey);
}

void Transaction::setContractCall(const std::string& contractAddr,
                                const std::string& method,
                                const std::vector<std::string>& params) {
    contractAddress = contractAddr;
    methodSignature = method;
    parameters = params;
    hash = calculateHash(); // Recalculate hash with new data
}

double Transaction::getTotalInput() const {
    double total = 0;
    for (const auto& input : inputs) {
        // Get previous transaction output amount
        // This would typically involve blockchain lookup
        // Simplified for this example
        total += input.amount;
    }
    return total;
}

double Transaction::getTotalOutput() const {
    double total = 0;
    for (const auto& output : outputs) {
        total += output.amount;
    }
    return total;
} 