#pragma once
#include <string>
#include <vector>

enum class MessageType {
    HANDSHAKE,
    BLOCK_REQUEST,
    BLOCK_RESPONSE,
    TRANSACTION_BROADCAST,
    PEER_DISCOVERY,
    VALIDATION_REQUEST,
    VALIDATION_RESPONSE,
    CONTRACT_DEPLOYMENT,
    CONTRACT_EXECUTION,
    SYNC_REQUEST,
    SYNC_RESPONSE
};

struct ProtocolMessage {
    MessageType type;
    std::string sender;
    std::string payload;
    uint64_t timestamp;
    std::string signature;
    
    std::string serialize() const;
    static ProtocolMessage deserialize(const std::string& data);
    bool verify() const;
};

class NetworkProtocol {
public:
    static constexpr uint16_t DEFAULT_PORT = 8333;
    static constexpr size_t MAX_MESSAGE_SIZE = 1024 * 1024; // 1MB
    static constexpr uint32_t PROTOCOL_VERSION = 1;
    
    struct HandshakeData {
        uint32_t version;
        std::string nodeId;
        uint64_t timestamp;
        std::vector<std::string> capabilities;
    };
    
    static ProtocolMessage createHandshake(const HandshakeData& data);
    static ProtocolMessage createBlockRequest(uint64_t height);
    static ProtocolMessage createTransactionBroadcast(const Transaction& tx);
    static ProtocolMessage createValidationRequest(const Block& block);
}; 