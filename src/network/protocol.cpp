#include "protocol.hpp"
#include "../crypto/encryption.hpp"
#include <sstream>
#include <json/json.h> // Using JsonCpp for serialization

std::string ProtocolMessage::serialize() const {
    Json::Value root;
    root["type"] = static_cast<int>(type);
    root["sender"] = sender;
    root["payload"] = payload;
    root["timestamp"] = Json::Value::UInt64(timestamp);
    root["signature"] = signature;
    
    Json::FastWriter writer;
    return writer.write(root);
}

ProtocolMessage ProtocolMessage::deserialize(const std::string& data) {
    Json::Value root;
    Json::Reader reader;
    
    if (!reader.parse(data, root)) {
        throw std::runtime_error("Failed to parse protocol message");
    }
    
    ProtocolMessage message;
    message.type = static_cast<MessageType>(root["type"].asInt());
    message.sender = root["sender"].asString();
    message.payload = root["payload"].asString();
    message.timestamp = root["timestamp"].asUInt64();
    message.signature = root["signature"].asString();
    
    return message;
}

bool ProtocolMessage::verify() const {
    // Create message digest for verification
    std::stringstream ss;
    ss << static_cast<int>(type) << sender << payload << timestamp;
    std::string messageDigest = ss.str();
    
    // Verify signature
    std::vector<uint8_t> sig(signature.begin(), signature.end());
    return Encryption::verify(messageDigest, sig, sender);
}

ProtocolMessage NetworkProtocol::createHandshake(const HandshakeData& data) {
    Json::Value payload;
    payload["version"] = data.version;
    payload["nodeId"] = data.nodeId;
    payload["timestamp"] = Json::Value::UInt64(data.timestamp);
    
    Json::Value capabilities(Json::arrayValue);
    for (const auto& cap : data.capabilities) {
        capabilities.append(cap);
    }
    payload["capabilities"] = capabilities;
    
    Json::FastWriter writer;
    
    ProtocolMessage message;
    message.type = MessageType::HANDSHAKE;
    message.sender = data.nodeId;
    message.payload = writer.write(payload);
    message.timestamp = std::time(nullptr);
    
    return message;
}

ProtocolMessage NetworkProtocol::createBlockRequest(uint64_t height) {
    Json::Value payload;
    payload["height"] = Json::Value::UInt64(height);
    
    Json::FastWriter writer;
    
    ProtocolMessage message;
    message.type = MessageType::BLOCK_REQUEST;
    message.payload = writer.write(payload);
    message.timestamp = std::time(nullptr);
    
    return message;
}

ProtocolMessage NetworkProtocol::createTransactionBroadcast(const Transaction& tx) {
    Json::Value payload;
    payload["hash"] = tx.getHash();
    payload["data"] = tx.serialize();
    
    Json::FastWriter writer;
    
    ProtocolMessage message;
    message.type = MessageType::TRANSACTION_BROADCAST;
    message.payload = writer.write(payload);
    message.timestamp = std::time(nullptr);
    
    return message;
} 