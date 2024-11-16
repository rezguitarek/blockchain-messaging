#pragma once
#include <string>
#include <vector>
#include <array>

class SHA256 {
public:
    static std::string hash(const std::string& input);
    static std::string hash(const std::vector<uint8_t>& input);
    
    // Double SHA256 (commonly used in blockchain)
    static std::string doubleHash(const std::string& input);
    
    // Hash with salt
    static std::string hashWithSalt(const std::string& input, const std::string& salt);
    
private:
    static constexpr size_t BLOCK_SIZE = 64;
    static constexpr size_t HASH_SIZE = 32;
    
    static void transform(uint32_t state[8], const uint8_t block[64]);
    static void pad(std::vector<uint8_t>& message);
};

class HashUtils {
public:
    // Merkle root calculation
    static std::string calculateMerkleRoot(const std::vector<std::string>& transactions);
    
    // RIPEMD160(SHA256()) for address generation
    static std::string hash160(const std::string& input);
    
    // Various encoding utilities
    static std::string base58Encode(const std::vector<uint8_t>& input);
    static std::vector<uint8_t> base58Decode(const std::string& input);
}; 