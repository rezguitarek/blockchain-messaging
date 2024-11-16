#pragma once
#include <unordered_map>
#include <list>
#include <mutex>
#include <memory>

template<typename K, typename V>
class LRUCache {
private:
    size_t capacity;
    std::list<K> lruList;
    std::unordered_map<K, std::pair<V, typename std::list<K>::iterator>> cache;
    mutable std::mutex cacheMutex;
    
public:
    LRUCache(size_t size) : capacity(size) {}
    
    void put(const K& key, const V& value) {
        std::lock_guard<std::mutex> lock(cacheMutex);
        
        auto it = cache.find(key);
        if (it != cache.end()) {
            lruList.erase(it->second.second);
        } else if (cache.size() >= capacity) {
            cache.erase(lruList.back());
            lruList.pop_back();
        }
        
        lruList.push_front(key);
        cache[key] = {value, lruList.begin()};
    }
    
    bool get(const K& key, V& value) const {
        std::lock_guard<std::mutex> lock(cacheMutex);
        
        auto it = cache.find(key);
        if (it == cache.end()) {
            return false;
        }
        
        lruList.splice(lruList.begin(), lruList, it->second.second);
        value = it->second.first;
        return true;
    }
};

class BlockCache {
private:
    LRUCache<std::string, Block> blockCache;
    LRUCache<std::string, Transaction> transactionCache;
    
public:
    BlockCache(size_t blockCacheSize = 1000, size_t txCacheSize = 5000)
        : blockCache(blockCacheSize),
          transactionCache(txCacheSize) {}
    
    void cacheBlock(const Block& block) {
        blockCache.put(block.getHash(), block);
        
        // Cache all transactions in the block
        for (const auto& tx : block.getTransactions()) {
            transactionCache.put(tx.getHash(), tx);
        }
    }
    
    bool getBlock(const std::string& hash, Block& block) {
        return blockCache.get(hash, block);
    }
    
    bool getTransaction(const std::string& hash, Transaction& tx) {
        return transactionCache.get(hash, tx);
    }
}; 