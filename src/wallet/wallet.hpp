#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../crypto/encryption.hpp"
#include "../core/transaction.hpp"

class Wallet {
private:
    std::string address;
    std::string privateKey;
    std::string publicKey;
    double balance;
    
    struct MessageBox {
        std::vector<Transaction> sentMessages;
        std::vector<Transaction> receivedMessages;
    } messageBox;
    
    // Transaction history
    struct TransactionHistory {
        std::vector<Transaction> sent;
        std::vector<Transaction> received;
        std::vector<Transaction> pending;
    } history;
    
public:
    Wallet();
    Wallet(const std::string& privateKeyIn);
    
    // Core wallet functions
    Transaction createTransaction(const std::string& recipient, double amount);
    Transaction createMessage(const std::string& recipient, const std::string& message);
    bool sendTransaction(const Transaction& transaction);
    
    // Message handling
    void receiveMessage(const Transaction& messageTx);
    std::vector<std::string> getDecryptedMessages() const;
    
    // Balance management
    void updateBalance(double newBalance);
    double getBalance() const { return balance; }
    
    // Key management
    void generateNewKeys();
    bool importPrivateKey(const std::string& key);
    bool exportPrivateKey(const std::string& password);
    
    // Getters
    std::string getAddress() const { return address; }
    std::string getPublicKey() const { return publicKey; }
    TransactionHistory getTransactionHistory() const { return history; }
}; 