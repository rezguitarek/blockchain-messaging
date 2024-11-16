#include <gtest/gtest.h>
#include "../src/crypto/encryption.hpp"
#include "../src/crypto/hash.hpp"

TEST(CryptoTest, SHA256Hashing) {
    std::string input = "test message";
    std::string hash = SHA256::hash(input);
    
    ASSERT_EQ(hash.length(), 64); // 32 bytes in hex
    ASSERT_EQ(SHA256::hash(input), hash); // Consistency check
}

TEST(CryptoTest, KeyPairGeneration) {
    std::string privateKey = Encryption::generatePrivateKey();
    std::string publicKey = Encryption::generatePublicKey(privateKey);
    
    ASSERT_FALSE(privateKey.empty());
    ASSERT_FALSE(publicKey.empty());
    ASSERT_NE(privateKey, publicKey);
}

TEST(CryptoTest, SignatureVerification) {
    std::string privateKey = Encryption::generatePrivateKey();
    std::string publicKey = Encryption::generatePublicKey(privateKey);
    std::string message = "test message";
    
    std::vector<uint8_t> signature = Encryption::sign(message, privateKey);
    ASSERT_TRUE(Encryption::verify(message, signature, publicKey));
    
    // Test with modified message
    ASSERT_FALSE(Encryption::verify("modified message", signature, publicKey));
}

TEST(CryptoTest, MessageEncryption) {
    std::string privateKey = Encryption::generatePrivateKey();
    std::string publicKey = Encryption::generatePublicKey(privateKey);
    std::string message = "secret message";
    
    std::vector<uint8_t> encrypted = Encryption::encrypt(message, publicKey);
    std::string decrypted = Encryption::decrypt(encrypted, privateKey);
    
    ASSERT_EQ(message, decrypted);
} 