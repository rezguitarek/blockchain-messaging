#include "node.hpp"
#include <chrono>
#include <algorithm>

Node::Node(const std::string& nodeIdIn, uint16_t port)
    : nodeId(nodeIdIn),
      blockchain(std::make_shared<Blockchain>()),
      wallet(std::make_shared<Wallet>()),
      network(std::make_unique<P2PNetwork>(nodeId, port)),
      running(false) {
    
    state = {false, false, 0, std::time(nullptr)};
}

Node::~Node() {
    stop();
}

void Node::start() {
    if (running) return;
    
    running = true;
    network->start();
    
    // Start validation and sync threads
    validationThread = std::thread(&Node::validationLoop, this);
    syncThread = std::thread(&Node::syncLoop, this);
    
    // Initial blockchain sync
    syncBlockchain();
}

void Node::validationLoop() {
    while (running) {
        if (state.isValidating) {
            // Process pending transactions
            processTransactions();
            
            // Create new block if enough transactions
            if (pendingTransactions.size() >= blockchain->getMinTransactionsPerBlock()) {
                createAndBroadcastBlock();
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Node::syncLoop() {
    while (running) {
        if (!state.isSynced()) {
            syncBlockchain();
        }
        
        // Handle orphan blocks
        handleOrphanBlocks();
        
        // Update node state
        state.lastUpdate = std::time(nullptr);
        
        std::this_thread::sleep_for(std::chrono::seconds(30));
    }
}

void Node::submitTransaction(const Transaction& transaction) {
    if (!transaction.isValid()) {
        throw std::runtime_error("Invalid transaction");
    }
    
    // Add to pending transactions
    pendingTransactions.push(transaction);
    
    // Broadcast to network
    network->broadcastTransaction(transaction);
}

void Node::validateAndAddBlock(const Block& block) {
    if (!blockchain->validateBlock(block)) {
        throw std::runtime_error("Invalid block");
    }
    
    try {
        blockchain->addBlock(block);
        state.lastBlockHeight = blockchain->getChainLength();
        
        // Remove processed transactions from pending queue
        for (const auto& tx : block.getTransactions()) {
            removePendingTransaction(tx.getHash());
        }
        
        // Broadcast block to network
        broadcastBlock(block);
    } catch (const std::exception& e) {
        // Handle block addition failure
        std::cerr << "Failed to add block: " << e.what() << std::endl;
    }
} 