#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include "transaction.hpp"

class SmartContractEngine {
private:
    struct ContractState {
        std::string bytecode;
        std::unordered_map<std::string, std::string> storage;
        std::string owner;
        bool isActive;
    };
    
    std::unordered_map<std::string, ContractState> contracts;
    std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>> standardFunctions;
    
public:
    SmartContractEngine();
    
    // Contract deployment and execution
    std::string deployContract(const std::string& bytecode, 
                             const std::string& owner);
    
    bool executeContract(const std::string& contractAddress,
                        const std::string& method,
                        const std::vector<std::string>& params,
                        const std::string& caller);
    
    // State management
    bool updateContractState(const std::string& contractAddress,
                           const std::string& key,
                           const std::string& value);
    
    std::string getContractState(const std::string& contractAddress,
                                const std::string& key) const;
    
    // Contract validation
    bool validateContract(const std::string& bytecode) const;
    bool isContractActive(const std::string& contractAddress) const;
    
private:
    void initializeStandardFunctions();
    bool validateMethodCall(const std::string& method,
                          const std::vector<std::string>& params) const;
}; 