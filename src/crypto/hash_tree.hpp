#pragma once
#include <vector>
#include <string>
#include "hash.hpp"

class MerkleTree {
private:
    struct Node {
        std::string hash;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        
        Node(const std::string& hashIn) : hash(hashIn) {}
    };
    
    std::shared_ptr<Node> root;
    std::vector<std::string> leaves;
    
public:
    MerkleTree(const std::vector<std::string>& transactions);
    
    std::string getRootHash() const;
    bool verifyTransaction(const std::string& transaction, const std::vector<std::string>& proof) const;
    std::vector<std::string> getProof(const std::string& transaction) const;
    
private:
    std::shared_ptr<Node> buildTree(const std::vector<std::string>& nodes);
    std::string calculateParentHash(const std::string& left, const std::string& right);
}; 