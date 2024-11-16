#pragma once
#include <vector>
#include <mutex>
#include <memory>
#include "../core/transaction.hpp"

class MemoryPool {
private:
    struct PoolEntry {
        Transaction transaction;
        time_t timestamp;
        uint32_t priority;
    };
    
    std::vector<PoolEntry> pool;
    std::mutex poolMutex;
    size_t maxSize;
    
public:
    MemoryPool(size_t maxSizeIn = 5000) : maxSize(maxSizeIn) {}
    
    bool addTransaction(const Transaction& tx, uint32_t priority = 1);
    std::vector<Transaction> getHighestPriorityTransactions(size_t count);
    void removeTransaction(const std::string& txHash);
    void cleanup(uint32_t maxAgeSeconds = 3600);
    
    size_t size() const { return pool.size(); }
    bool isFull() const { return pool.size() >= maxSize; }
}; 