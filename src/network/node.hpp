#pragma once
#include <queue>
#include <thread>
#include <atomic>
#include "../core/blockchain.hpp"
#include "../wallet/wallet.hpp"

class Node {
private:
    std::string nodeId;
    std::shared_ptr<Blockchain> blockchain;
    std::shared_ptr<Wallet> wallet;
    std::unique_ptr<P2PNetwork> network;
    
    struct NodeState {
        bool isSyncing;
        bool isValidating;
        uint64_t lastBlockHeight;
        std::time_t lastUpdate;
    } state;
    
    // Threading
    std::thread validationThread;
    std::thread syncThread;
    std::atomic<bool> running;
    
    // Message queues
    std::queue<Block> pendingBlocks;
    std::queue<Transaction> pendingTransactions;
    
public:
    Node(const std::string& nodeIdIn, uint16_t port);
    ~Node();
    
    // Node lifecycle
    void start();
    void stop();
    
    // Blockchain operations
    void syncBlockchain();
    void validateAndAddBlock(const Block& block);
    void broadcastBlock(const Block& block);
    
    // Transaction handling
    void submitTransaction(const Transaction& transaction);
    void processTransactions();
    
    // Validation
    void startValidating();
    void stopValidating();
    
    // State management
    NodeState getState() const;
    bool isSynced() const;
    
private:
    void validationLoop();
    void syncLoop();
    void handleOrphanBlocks();
}; 