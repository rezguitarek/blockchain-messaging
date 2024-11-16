#include "memory_pool.hpp"
#include <algorithm>

bool MemoryPool::addTransaction(const Transaction& tx, uint32_t priority) {
    std::lock_guard<std::mutex> lock(poolMutex);
    
    if (isFull()) {
        cleanup();
        if (isFull()) {
            return false;
        }
    }
    
    PoolEntry entry{tx, std::time(nullptr), priority};
    pool.push_back(entry);
    
    // Sort by priority
    std::sort(pool.begin(), pool.end(),
              [](const PoolEntry& a, const PoolEntry& b) {
                  return a.priority > b.priority;
              });
    
    return true;
}

std::vector<Transaction> MemoryPool::getHighestPriorityTransactions(size_t count) {
    std::lock_guard<std::mutex> lock(poolMutex);
    std::vector<Transaction> result;
    
    count = std::min(count, pool.size());
    for (size_t i = 0; i < count; ++i) {
        result.push_back(pool[i].transaction);
    }
    
    return result;
}

void MemoryPool::cleanup(uint32_t maxAgeSeconds) {
    time_t now = std::time(nullptr);
    
    pool.erase(
        std::remove_if(pool.begin(), pool.end(),
                      [now, maxAgeSeconds](const PoolEntry& entry) {
                          return (now - entry.timestamp) > maxAgeSeconds;
                      }),
        pool.end()
    );
} 