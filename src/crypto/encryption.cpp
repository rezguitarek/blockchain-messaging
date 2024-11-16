#include "encryption.hpp"
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/ec.h>

std::string Encryption::generatePrivateKey() {
    std::vector<uint8_t> key(KEY_SIZE / 8);
    if (RAND_bytes(key.data(), key.size()) != 1) {
        throw std::runtime_error("Failed to generate private key");
    }
    
    return bytesToHex(key);
}

std::string Encryption::generatePublicKey(const std::string& privateKey) {
    EC_KEY* ecKey = EC_KEY_new_by_curve_name(NID_secp256k1);
    if (!ecKey) {
        throw std::runtime_error("Failed to create EC key");
    }
    
    BIGNUM* priv = hexToBN(privateKey);
    if (!EC_KEY_set_private_key(ecKey, priv)) {
        BN_free(priv);
        EC_KEY_free(ecKey);
        throw std::runtime_error("Failed to set private key");
    }
    
    EC_POINT* pub = EC_POINT_new(EC_KEY_get0_group(ecKey));
    if (!EC_POINT_mul(EC_KEY_get0_group(ecKey), pub, priv, nullptr, nullptr, nullptr)) {
        BN_free(priv);
        EC_KEY_free(ecKey);
        EC_POINT_free(pub);
        throw std::runtime_error("Failed to generate public key");
    }
    
    std::string publicKey = EC_POINT_point2hex(EC_KEY_get0_group(ecKey), 
                                             pub, 
                                             POINT_CONVERSION_COMPRESSED, 
                                             nullptr);
    
    BN_free(priv);
    EC_KEY_free(ecKey);
    EC_POINT_free(pub);
    
    return publicKey;
}

std::vector<uint8_t> Encryption::encrypt(const std::string& message,
                                       const std::string& publicKey) {
    EncryptionKey key;
    key.key.resize(KEY_SIZE / 8);
    key.iv.resize(IV_SIZE);
    
    // Generate random IV
    if (RAND_bytes(key.iv.data(), IV_SIZE) != 1) {
        throw std::runtime_error("Failed to generate IV");
    }
    
    // Perform encryption
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, key.key.data(), key.iv.data());
    
    std::vector<uint8_t> ciphertext(message.size() + EVP_MAX_BLOCK_LENGTH);
    int len;
    
    EVP_EncryptUpdate(ctx, ciphertext.data(), &len,
                     reinterpret_cast<const uint8_t*>(message.data()),
                     message.size());
    
    int finalLen;
    EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &finalLen);
    
    EVP_CIPHER_CTX_free(ctx);
    
    ciphertext.resize(len + finalLen);
    return ciphertext;
} 