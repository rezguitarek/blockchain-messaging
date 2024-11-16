#include "wallet.hpp"
#include <random>
#include <sstream>
#include "../crypto/hash.hpp"

Wallet::Wallet() {
    generateNewKeys();
}

Wallet::Wallet(const std::string& privateKeyIn) {
    if (!importPrivateKey(privateKeyIn)) {
        generateNewKeys();
    }
}

void Wallet::generateNewKeys() {
    // Generate a new key pair using secure random number generation
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;
    
    // Generate private key
    std::stringstream ss;
    for (int i = 0; i < 4; i++) {
        ss << std::hex << dis(gen);
    }
    privateKey = ss.str();
    
    // Generate public key using elliptic curve cryptography
    publicKey = Encryption::generatePublicKey(privateKey);
    
    // Generate address (starting with 'M')
    address = "M" + SHA256::hash(publicKey).substr(0, 30);
}

Transaction Wallet::createTransaction(const std::string& recipient, double amount) {
    if (amount > balance) {
        throw std::runtime_error("Insufficient funds");
    }
    
    Transaction transaction(address, recipient, TransactionType::FINANCIAL);
    transaction.setAmount(amount);
    transaction.signTransaction(privateKey);
    
    history.pending.push_back(transaction);
    return transaction;
}

Transaction Wallet::createMessage(const std::string& recipient, const std::string& message) {
    Transaction messageTx(address, recipient, TransactionType::MESSAGE);
    messageTx.setMessage(message, recipient);
    messageTx.signTransaction(privateKey);
    
    messageBox.sentMessages.push_back(messageTx);
    return messageTx;
}

void Wallet::receiveMessage(const Transaction& messageTx) {
    if (messageTx.getRecipient() != address) {
        throw std::runtime_error("Message not intended for this wallet");
    }
    
    if (messageTx.verifySignature()) {
        messageBox.receivedMessages.push_back(messageTx);
    }
}

std::vector<std::string> Wallet::getDecryptedMessages() const {
    std::vector<std::string> decryptedMessages;
    
    for (const auto& messageTx : messageBox.receivedMessages) {
        try {
            std::string decryptedMessage = messageTx.decryptMessage(privateKey);
            decryptedMessages.push_back(decryptedMessage);
        } catch (const std::exception& e) {
            // Skip messages that can't be decrypted
            continue;
        }
    }
    
    return decryptedMessages;
} 