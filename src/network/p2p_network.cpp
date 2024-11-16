#include "p2p_network.hpp"
#include <chrono>
#include <algorithm>

P2PNetwork::P2PNetwork(const std::string& nodeIdIn, uint16_t portIn)
    : nodeId(nodeIdIn),
      port(portIn),
      isRunning(false) {
    state = {0, 0, 0.0, 0};
}

P2PNetwork::~P2PNetwork() {
    stop();
}

void P2PNetwork::start() {
    if (isRunning) return;
    
    isRunning = true;
    networkThread = std::thread(&P2PNetwork::runNetworkLoop, this);
    
    // Initialize network connections
    synchronizeWithPeers();
    measureNetworkLatency();
}

void P2PNetwork::runNetworkLoop() {
    while (isRunning) {
        std::lock_guard<std::mutex> lock(networkMutex);
        
        // Process incoming messages
        handleIncomingMessages();
        
        // Optimize network connections
        optimizeConnections();
        
        // Check for network partitions
        handleNetworkPartition();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void P2PNetwork::broadcastMessage(const Message& message) {
    std::lock_guard<std::mutex> lock(networkMutex);
    
    for (const auto& [peerId, peer] : peers) {
        try {
            peer->sendMessage(message);
            state.messageCount++;
        } catch (const std::exception& e) {
            // Handle failed message delivery
            removePeer(peerId);
        }
    }
}

void P2PNetwork::handleIncomingMessages() {
    while (!messageQueue.empty()) {
        Message message = messageQueue.front();
        messageQueue.pop();
        
        switch (message.getType()) {
            case MessageType::TRANSACTION:
                processTransaction(message.getTransaction());
                break;
                
            case MessageType::BLOCK:
                processBlock(message.getBlock());
                break;
                
            case MessageType::PEER_DISCOVERY:
                handlePeerDiscovery(message);
                break;
                
            case MessageType::VALIDATION_REQUEST:
                handleValidationRequest(message);
                break;
        }
    }
}

void P2PNetwork::optimizeConnections() {
    std::vector<std::string> slowPeers;
    
    for (const auto& [peerId, peer] : peers) {
        if (peer->getLatency() > 1000) { // 1 second threshold
            slowPeers.push_back(peerId);
        }
    }
    
    // Remove slow peers and try to find better connections
    for (const auto& peerId : slowPeers) {
        removePeer(peerId);
        findNewPeer();
    }
} 