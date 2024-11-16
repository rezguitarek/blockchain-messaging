#pragma once
#include <string>
#include <vector>

class KeyDerivation {
public:
    static std::string deriveChildKey(const std::string& masterKey, uint32_t index);
    static std::string createHDWallet(const std::string& seed);
    static std::string generateMnemonic();
    static std::string mnemonicToSeed(const std::string& mnemonic, const std::string& passphrase = "");
    
private:
    static const std::vector<std::string> wordList;
    static constexpr uint32_t HARDENED_OFFSET = 0x80000000;
    
    static std::vector<uint8_t> hmacSHA512(const std::vector<uint8_t>& key, 
                                          const std::vector<uint8_t>& data);
}; 