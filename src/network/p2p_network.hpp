#pragma once
#include <vector>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <thread>
#include "node.hpp"
#include "../core/blockchain.hpp"

class P2PNetwork {
private:
    std::string nodeId;
    uint16_t port;
    std::unordered_map<std::string, std::shared_ptr<Node>> peers;
    std::queue<Message> messageQueue;
    std::mutex networkMutex;
    std::thread networkThread;
    bool isRunning;
    
    // Network state
    struct NetworkState {
        uint32_t connectedPeers;
        uint32_t activeValidators;
        double networkLatency;
        uint32_t messageCount;
    } state;
    
public:
    P2PNetwork(const std::string& nodeIdIn, uint16_t portIn);
    ~P2PNetwork();
    
    // Core networking
    void start();
    void stop();
    void broadcastMessage(const Message& message);
    void broadcastTransaction(const Transaction& transaction);
    void broadcastBlock(const Block& block);
    
    // Peer management
    bool addPeer(const std::string& peerId, const std::string& address);
    void removePeer(const std::string& peerId);
    void synchronizeWithPeers();
    
    // Message handling
    void processMessage(const Message& message);
    void handleIncomingMessages();
    
    // Network optimization
    void optimizeConnections();
    void measureNetworkLatency();
    void handleNetworkPartition();
    
private:
    void runNetworkLoop();
    void validatePeerConnection(const std::string& peerId);
}; 