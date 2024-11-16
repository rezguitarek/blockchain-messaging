#include "hash.hpp"
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <sstream>
#include <iomanip>

std::string SHA256::hash(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.length());
    SHA256_Final(hash, &sha256);
    
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

std::string SHA256::doubleHash(const std::string& input) {
    return hash(hash(input));
}

std::string SHA256::hashWithSalt(const std::string& input, const std::string& salt) {
    return hash(input + salt);
}

std::string HashUtils::calculateMerkleRoot(const std::vector<std::string>& transactions) {
    if (transactions.empty()) {
        return SHA256::hash("");
    }
    
    std::vector<std::string> tree = transactions;
    
    while (tree.size() > 1) {
        if (tree.size() % 2 != 0) {
            tree.push_back(tree.back());
        }
        
        std::vector<std::string> newLevel;
        for (size_t i = 0; i < tree.size(); i += 2) {
            std::string combined = tree[i] + tree[i + 1];
            newLevel.push_back(SHA256::doubleHash(combined));
        }
        tree = newLevel;
    }
    
    return tree[0];
}

std::string HashUtils::hash160(const std::string& input) {
    // SHA256 first
    unsigned char sha256_result[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.length());
    SHA256_Final(sha256_result, &sha256);
    
    // RIPEMD160 second
    unsigned char ripemd160_result[RIPEMD160_DIGEST_LENGTH];
    RIPEMD160_CTX ripemd160;
    RIPEMD160_Init(&ripemd160);
    RIPEMD160_Update(&ripemd160, sha256_result, SHA256_DIGEST_LENGTH);
    RIPEMD160_Final(ripemd160_result, &ripemd160);
    
    // Convert to hex string
    std::stringstream ss;
    for(int i = 0; i < RIPEMD160_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)ripemd160_result[i];
    }
    return ss.str();
} 