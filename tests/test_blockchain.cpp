#include <gtest/gtest.h>
#include "../src/core/blockchain.hpp"
#include "../src/core/transaction.hpp"
#include "../src/wallet/wallet.hpp"

class BlockchainTest : public ::testing::Test {
protected:
    std::shared_ptr<Blockchain> blockchain;
    std::shared_ptr<Wallet> wallet1;
    std::shared_ptr<Wallet> wallet2;
    
    void SetUp() override {
        blockchain = std::make_shared<Blockchain>();
        wallet1 = std::make_shared<Wallet>();
        wallet2 = std::make_shared<Wallet>();
    }
};

TEST_F(BlockchainTest, GenesisBlockCreation) {
    ASSERT_EQ(blockchain->getChainLength(), 1);
    ASSERT_EQ(blockchain->getLatestBlock().getPreviousHash(), "0");
}

TEST_F(BlockchainTest, AddBlock) {
    Transaction tx(wallet1->getAddress(), wallet2->getAddress(), TransactionType::FINANCIAL);
    tx.setAmount(50.0);
    tx.sign(wallet1->getPrivateKey());
    
    std::vector<Transaction> transactions = {tx};
    Block block(blockchain->getChainLength(), 
                transactions, 
                blockchain->getLatestBlock().getHash());
    
    ASSERT_TRUE(blockchain->addBlock(block));
    ASSERT_EQ(blockchain->getChainLength(), 2);
}

TEST_F(BlockchainTest, ValidateChain) {
    ASSERT_TRUE(blockchain->isChainValid());
    
    // Add some valid blocks
    for (int i = 0; i < 5; i++) {
        Transaction tx(wallet1->getAddress(), wallet2->getAddress(), TransactionType::FINANCIAL);
        tx.setAmount(10.0 * i);
        tx.sign(wallet1->getPrivateKey());
        
        std::vector<Transaction> transactions = {tx};
        Block block(blockchain->getChainLength(), 
                    transactions, 
                    blockchain->getLatestBlock().getHash());
        blockchain->addBlock(block);
    }
    
    ASSERT_TRUE(blockchain->isChainValid());
} 