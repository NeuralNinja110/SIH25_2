/**
 * @file StringEncryption.cpp
 * @brief Implementation of string encryption pass
 * @version 1.0.0
 * @date 2025-10-09
 */

#include "passes/StringEncryption.h"
#include "MetricsCollector.h"
#include "RandomGenerator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/GlobalVariable.h"

namespace obfuscator {

StringEncryption::StringEncryption(const std::string& algorithm)
    : ObfuscationPass("StringEncryption", true), algorithm_(algorithm) {
}

bool StringEncryption::runOnModule(llvm::Module& module, MetricsCollector& metrics) {
    // Skip if already processed (check module-level metadata)
    if (module.getNamedMetadata("obfuscated.StringEncryption")) {
        return false;
    }
    
    uint32_t encrypted = encryptStrings(module);
    
    if (encrypted > 0) {
        // Mark module as having strings encrypted
        llvm::NamedMDNode* md = module.getOrInsertNamedMetadata("obfuscated.StringEncryption");
        llvm::LLVMContext& ctx = module.getContext();
        llvm::MDNode* node = llvm::MDNode::get(ctx, 
            llvm::MDString::get(ctx, "StringEncryption"));
        md->addOperand(node);
    }
    
    metrics.incrementTransformations(name_, encrypted);
    
    // Estimate sizes (simplified)
    uint32_t originalSize = encrypted * 10;  // Approximate
    uint32_t encryptedSize = encrypted * 12;  // Approximate with overhead
    metrics.recordStringEncryption(encrypted, originalSize, encryptedSize);
    
    return encrypted > 0;
}

uint32_t StringEncryption::encryptStrings(llvm::Module& module) {
    uint32_t count = 0;
    auto& rng = RandomGenerator::getInstance();
    
    std::vector<llvm::GlobalVariable*> stringGlobals;
    
    // Find string constants
    for (auto& global : module.globals()) {
        if (global.hasInitializer()) {
            if (auto* init = llvm::dyn_cast<llvm::ConstantDataArray>(global.getInitializer())) {
                if (init->isString()) {
                    stringGlobals.push_back(&global);
                }
            }
        }
    }
    
    // Encrypt each string
    for (auto* global : stringGlobals) {
        auto* init = llvm::cast<llvm::ConstantDataArray>(global->getInitializer());
        std::string str = init->getAsString().str();
        
        if (str.length() < 2 || str.length() > 256) {
            continue;  // Skip very short or very long strings
        }
        
        // Generate random key
        uint8_t key = static_cast<uint8_t>(rng.getUInt32(1, 255));
        
        // Encrypt string
        std::vector<uint8_t> encrypted = encryptString(str, key);
        
        // Create encrypted global
        llvm::GlobalVariable* encryptedGlobal = createEncryptedString(module, str, key);
        
        if (encryptedGlobal) {
            count++;
        }
    }
    
    return count;
}

std::vector<uint8_t> StringEncryption::encryptString(const std::string& str, uint8_t key) {
    std::vector<uint8_t> encrypted;
    encrypted.reserve(str.length());
    
    if (algorithm_ == "xor") {
        // Simple XOR encryption
        for (char c : str) {
            encrypted.push_back(static_cast<uint8_t>(c) ^ key);
        }
    } else {
        // Default to XOR
        for (char c : str) {
            encrypted.push_back(static_cast<uint8_t>(c) ^ key);
        }
    }
    
    return encrypted;
}

llvm::GlobalVariable* StringEncryption::createEncryptedString(llvm::Module& module,
                                                              const std::string& str,
                                                              uint8_t key) {
    std::vector<uint8_t> encrypted = encryptString(str, key);
    
    // Create array type
    llvm::ArrayType* arrayType = llvm::ArrayType::get(
        llvm::Type::getInt8Ty(module.getContext()), encrypted.size());
    
    // Create constant array
    std::vector<llvm::Constant*> elements;
    for (uint8_t byte : encrypted) {
        elements.push_back(llvm::ConstantInt::get(
            llvm::Type::getInt8Ty(module.getContext()), byte));
    }
    
    llvm::Constant* arrayInit = llvm::ConstantArray::get(arrayType, elements);
    
    // Create global variable
    llvm::GlobalVariable* encryptedGlobal = new llvm::GlobalVariable(
        module,
        arrayType,
        true,  // is constant
        llvm::GlobalValue::PrivateLinkage,
        arrayInit,
        "enc.str"
    );
    
    return encryptedGlobal;
}

} // namespace obfuscator
