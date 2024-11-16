#include <gtest/gtest.h>
#include "../src/core/transaction.hpp"
#include "../src/wallet/wallet.hpp"

class TransactionTest : public ::testing::Test {
protected:
    std::shared_ptr<Wallet> sender;
    std::shared_ptr<Wallet> recipient;
    
    void SetUp() override {
        sender = std::make_shared<Wallet>();
        recipient = std::make_shared<Wallet>();
    }
};

TEST_F(TransactionTest, CreateFinancialTransaction) {
    Transaction tx(sender->getAddress(), recipient->getAddress(), TransactionType::FINANCIAL);
    tx.setAmount(100.0);
    
    ASSERT_EQ(tx.getTotalOutput(), 100.0);
    ASSERT_EQ(tx.getStatus(), TransactionStatus::PENDING);
}

TEST_F(TransactionTest, SignAndVerifyTransaction) {
    Transaction tx(sender->getAddress(), recipient->getAddress(), TransactionType::FINANCIAL);
    tx.setAmount(50.0);
    
    ASSERT_TRUE(tx.sign(sender->getPrivateKey()));
    ASSERT_TRUE(tx.verify());
}

TEST_F(TransactionTest, MessageTransaction) {
    Transaction tx(sender->getAddress(), recipient->getAddress(), TransactionType::MESSAGE);
    std::string message = "Hello, blockchain!";
    
    tx.setMessage(message, recipient->getPublicKey());
    ASSERT_TRUE(tx.sign(sender->getPrivateKey()));
    
    std::string decrypted = tx.decryptMessage(recipient->getPrivateKey());
    ASSERT_EQ(decrypted, message);
} 