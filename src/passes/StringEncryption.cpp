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
    
    // Create array type for encrypted data
    llvm::ArrayType* arrayType = llvm::ArrayType::get(
        llvm::Type::getInt8Ty(module.getContext()), encrypted.size());
    
    // Create constant array
    std::vector<llvm::Constant*> elements;
    for (uint8_t byte : encrypted) {
        elements.push_back(llvm::ConstantInt::get(
            llvm::Type::getInt8Ty(module.getContext()), byte));
    }
    
    llvm::Constant* arrayInit = llvm::ConstantArray::get(arrayType, elements);
    
    // Create encrypted global variable
    llvm::GlobalVariable* encryptedGlobal = new llvm::GlobalVariable(
        module,
        arrayType,
        false,  // NOT constant - will be decrypted at runtime
        llvm::GlobalValue::InternalLinkage,
        arrayInit,
        "obf.enc.str"
    );
    
    // Create decryption stub function
    createDecryptionStub(module, encryptedGlobal, key, encrypted.size());
    
    return encryptedGlobal;
}

void StringEncryption::createDecryptionStub(llvm::Module& module, 
                                            llvm::GlobalVariable* encStr,
                                                uint8_t key, size_t len) {
    // Create constructor function that runs before main
    llvm::FunctionType* ctorType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(module.getContext()), false);
    
    llvm::Function* ctor = llvm::Function::Create(
        ctorType, 
        llvm::GlobalValue::InternalLinkage,
        "obf.decrypt.ctor", 
        module);
    
    llvm::BasicBlock* entry = llvm::BasicBlock::Create(
        module.getContext(), "entry", ctor);
    llvm::IRBuilder<> builder(entry);
    
    // Get pointer to encrypted string
    llvm::Type* int8Ty = llvm::Type::getInt8Ty(module.getContext());
    llvm::PointerType* int8PtrTy = llvm::PointerType::get(int8Ty, 0);
    llvm::Value* ptr = builder.CreateBitCast(encStr, int8PtrTy);
    
    // Create decryption loop
    llvm::Value* idx = builder.CreateAlloca(builder.getInt64Ty());
    builder.CreateStore(builder.getInt64(0), idx);
    
    llvm::BasicBlock* loopHeader = llvm::BasicBlock::Create(
        module.getContext(), "loop.header", ctor);
    llvm::BasicBlock* loopBody = llvm::BasicBlock::Create(
        module.getContext(), "loop.body", ctor);
    llvm::BasicBlock* loopEnd = llvm::BasicBlock::Create(
        module.getContext(), "loop.end", ctor);
    
    builder.CreateBr(loopHeader);
    
    // Loop header
    builder.SetInsertPoint(loopHeader);
    llvm::Value* i = builder.CreateLoad(builder.getInt64Ty(), idx);
    llvm::Value* cond = builder.CreateICmpULT(i, builder.getInt64(len));
    builder.CreateCondBr(cond, loopBody, loopEnd);
    
    // Loop body - decrypt byte
    builder.SetInsertPoint(loopBody);
    llvm::Value* elemPtr = builder.CreateGEP(builder.getInt8Ty(), ptr, i);
    llvm::Value* encByte = builder.CreateLoad(builder.getInt8Ty(), elemPtr);
    llvm::Value* decByte = builder.CreateXor(encByte, builder.getInt8(key));
    builder.CreateStore(decByte, elemPtr);
    llvm::Value* nextI = builder.CreateAdd(i, builder.getInt64(1));
    builder.CreateStore(nextI, idx);
    builder.CreateBr(loopHeader);
    
    // Loop end
    builder.SetInsertPoint(loopEnd);
    builder.CreateRetVoid();
    
    // Register constructor to run before main
    // Add to llvm.global_ctors array
    llvm::Type* ctorStructTy = llvm::StructType::get(
        builder.getInt32Ty(),
        llvm::PointerType::get(ctorType, 0),
        int8PtrTy
    );
    
    std::vector<llvm::Constant*> ctorEntry;
    ctorEntry.push_back(builder.getInt32(65535));  // priority
    ctorEntry.push_back(ctor);  // function
    ctorEntry.push_back(llvm::ConstantPointerNull::get(int8PtrTy));  // data
    
    llvm::Constant* ctorStruct = llvm::ConstantStruct::get(
        llvm::cast<llvm::StructType>(ctorStructTy), ctorEntry);
    
    llvm::GlobalVariable* ctorsArray = module.getGlobalVariable("llvm.global_ctors");
    if (!ctorsArray) {
        llvm::ArrayType* arrayTy = llvm::ArrayType::get(ctorStructTy, 1);
        std::vector<llvm::Constant*> ctors = {ctorStruct};
        llvm::Constant* arrayInit = llvm::ConstantArray::get(arrayTy, ctors);
        
        new llvm::GlobalVariable(
            module, arrayTy, false,
            llvm::GlobalValue::AppendingLinkage,
            arrayInit, "llvm.global_ctors");
    }
}

} // namespace obfuscator
