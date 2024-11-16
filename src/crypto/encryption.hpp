#pragma once
#include <string>
#include <vector>
#include <memory>

class Encryption {
private:
    static const uint32_t KEY_SIZE = 256;
    static const uint32_t IV_SIZE = 16;
    
    struct EncryptionKey {
        std::vector<uint8_t> key;
        std::vector<uint8_t> iv;
    };
    
public:
    // Key generation
    static std::string generatePrivateKey();
    static std::string generatePublicKey(const std::string& privateKey);
    
    // Encryption/Decryption
    static std::vector<uint8_t> encrypt(const std::string& message, 
                                      const std::string& publicKey);
    static std::string decrypt(const std::vector<uint8_t>& ciphertext,
                             const std::string& privateKey);
    
    // Digital signatures
    static std::vector<uint8_t> sign(const std::string& message,
                                   const std::string& privateKey);
    static bool verify(const std::string& message,
                      const std::vector<uint8_t>& signature,
                      const std::string& publicKey);
    
    // Key management
    static bool validateKeyPair(const std::string& privateKey,
                              const std::string& publicKey);
    static std::string deriveAddress(const std::string& publicKey);
}; 