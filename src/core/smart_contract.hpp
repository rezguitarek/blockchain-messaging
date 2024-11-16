#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "transaction.hpp"

class SmartContract {
private:
    std::string address;
    std::string bytecode;
    std::string abi;
    std::unordered_map<std::string, std::string> state;
    bool isActive;
    
    struct ContractFunction {
        std::string name;
        std::vector<std::string> parameters;
        std::string returnType;
        std::vector<uint8_t> code;
    };
    
    std::vector<ContractFunction> functions;
    
public:
    SmartContract(const std::string& bytecodeIn, const std::string& abiIn);
    
    // Contract execution
    bool execute(const Transaction& transaction);
    std::string call(const std::string& function, const std::vector<std::string>& params);
    
    // State management
    void setState(const std::string& key, const std::string& value);
    std::string getState(const std::string& key) const;
    
    // Contract lifecycle
    void deploy();
    void terminate();
    
    // Validation
    bool validateTransaction(const Transaction& transaction) const;
    bool verifyState() const;
}; 