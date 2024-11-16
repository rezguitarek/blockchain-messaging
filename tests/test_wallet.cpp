#include <gtest/gtest.h>
#include "../src/wallet/wallet.hpp"
#include "../src/core/blockchain.hpp"

class WalletTest : public ::testing::Test {
protected:
    std::shared_ptr<Wallet> wallet;
    std::shared_ptr<Blockchain> blockchain;
    
    void SetUp() override {
        wallet = std::make_shared<Wallet>();
        blockchain = std::make_shared<Blockchain>();
    }
};

TEST_F(WalletTest, WalletCreation) {
    ASSERT_FALSE(wallet->getAddress().empty());
    ASSERT_FALSE(wallet->getPublicKey().empty());
    ASSERT_TRUE(wallet->getAddress().substr(0, 1) == "M");
}

TEST_F(WalletTest, KeyGeneration) {
    std::string originalAddress = wallet->getAddress();
    wallet->generateNewKeys();
    
    ASSERT_NE(originalAddress, wallet->getAddress());
    ASSERT_TRUE(wallet->getAddress().substr(0, 1) == "M");
}

TEST_F(WalletTest, TransactionCreation) {
    std::shared_ptr<Wallet> recipient = std::make_shared<Wallet>();
    
    Transaction tx = wallet->createTransaction(recipient->getAddress(), 50.0);
    ASSERT_TRUE(tx.verify());
    ASSERT_EQ(tx.getTotalOutput(), 50.0);
}

TEST_F(WalletTest, MessageHandling) {
    std::shared_ptr<Wallet> recipient = std::make_shared<Wallet>();
    std::string testMessage = "Test encrypted message";
    
    Transaction msgTx = wallet->createMessage(recipient->getAddress(), testMessage);
    recipient->receiveMessage(msgTx);
    
    auto messages = recipient->getDecryptedMessages();
    ASSERT_FALSE(messages.empty());
    ASSERT_EQ(messages[0], testMessage);
} 