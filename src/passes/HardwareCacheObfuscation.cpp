/**
 * @file HardwareCacheObfuscation.cpp
 * @brief Implementation of hardware cache-based obfuscation
 * @version 2.0.0
 * @date 2025-10-13
 */

#include "passes/HardwareCacheObfuscation.h"
#include "MetricsCollector.h"
#include "RandomGenerator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/Constants.h"

namespace obfuscator {

HardwareCacheObfuscation::HardwareCacheObfuscation(uint32_t intensity)
    : ObfuscationPass("HardwareCacheObfuscation", true), intensity_(intensity) {
}

bool HardwareCacheObfuscation::runOnModule(llvm::Module& module, MetricsCollector& metrics) {
    // Skip if intensity is too low
    if (intensity_ < 20) {
        return false;
    }
    
    // Create cache key generator function
    llvm::Function* cacheKeyGen = createCacheKeyGenerator(module);
    
    if (!cacheKeyGen) {
        return false;
    }
    
    // Apply cache-based transformations to constants
    uint32_t transformations = applyCacheBasedXOR(module, cacheKeyGen);
    
    metrics.incrementTransformations(name_, transformations);
    
    return transformations > 0;
}

llvm::Function* HardwareCacheObfuscation::createCacheKeyGenerator(llvm::Module& module) {
    llvm::LLVMContext& ctx = module.getContext();
    
    // Create function: i64 obf.cache.key()
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getInt64Ty(ctx), false);
    llvm::Function* func = llvm::Function::Create(
        funcType, llvm::GlobalValue::InternalLinkage,
        "obf.cache.key", module);
    
    llvm::BasicBlock* entry = llvm::BasicBlock::Create(ctx, "entry", func);
    llvm::IRBuilder<> builder(entry);
    
    // Create aligned cache line array (64 bytes = typical cache line size)
    llvm::ArrayType* cacheArrayType = llvm::ArrayType::get(builder.getInt8Ty(), 64);
    llvm::Value* cacheArray = builder.CreateAlloca(cacheArrayType, nullptr, "cache.line");
    
    // Set alignment to cache line boundary
    if (llvm::AllocaInst* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(cacheArray)) {
        allocaInst->setAlignment(llvm::Align(64));
    }
    
    // Initialize timing key
    llvm::Value* timingKey = builder.getInt64(0);
    
    // Perform cache timing measurements (loop 100 iterations for stability)
    llvm::Value* loopCount = builder.CreateAlloca(builder.getInt32Ty());
    builder.CreateStore(builder.getInt32(0), loopCount);
    
    llvm::BasicBlock* loopHeader = llvm::BasicBlock::Create(ctx, "loop.header", func);
    llvm::BasicBlock* loopBody = llvm::BasicBlock::Create(ctx, "loop.body", func);
    llvm::BasicBlock* loopEnd = llvm::BasicBlock::Create(ctx, "loop.end", func);
    
    builder.CreateBr(loopHeader);
    
    // Loop header
    builder.SetInsertPoint(loopHeader);
    llvm::Value* i = builder.CreateLoad(builder.getInt32Ty(), loopCount);
    llvm::Value* cond = builder.CreateICmpULT(i, builder.getInt32(100));
    builder.CreateCondBr(cond, loopBody, loopEnd);
    
    // Loop body - cache timing measurement
    builder.SetInsertPoint(loopBody);
    
    // Get start time using RDTSC
    llvm::Value* startTime = createRDTSC(builder);
    
    // Access cache line (forces cache load)
    llvm::Value* idx = builder.CreateURem(i, builder.getInt32(64));
    llvm::Value* idxExt = builder.CreateZExt(idx, builder.getInt64Ty());
    llvm::Value* elemPtr = builder.CreateGEP(cacheArrayType, cacheArray, 
        {builder.getInt64(0), idxExt});
    llvm::Value* elemValue = builder.CreateTrunc(i, builder.getInt8Ty());
    builder.CreateStore(elemValue, elemPtr);
    
    // Force memory barrier
    llvm::Value* loaded = builder.CreateLoad(builder.getInt8Ty(), elemPtr);
    llvm::Value* dummy = builder.CreateAdd(loaded, builder.getInt8(1));
    builder.CreateStore(dummy, elemPtr);
    
    // Get end time
    llvm::Value* endTime = createRDTSC(builder);
    
    // Calculate timing difference
    llvm::Value* timing = builder.CreateSub(endTime, startTime);
    
    // Incorporate timing into key with bit rotation
    llvm::Value* shiftAmount = builder.CreateURem(i, builder.getInt32(64));
    llvm::Value* shiftAmountExt = builder.CreateZExt(shiftAmount, builder.getInt64Ty());
    llvm::Value* rotated = builder.CreateShl(timing, shiftAmountExt);
    timingKey = builder.CreateXor(timingKey, rotated);
    
    // Increment loop counter
    llvm::Value* nextI = builder.CreateAdd(i, builder.getInt32(1));
    builder.CreateStore(nextI, loopCount);
    builder.CreateBr(loopHeader);
    
    // Loop end
    builder.SetInsertPoint(loopEnd);
    
    // Final mixing of timing key
    llvm::Value* mixed1 = builder.CreateMul(timingKey, builder.getInt64(0x9e3779b97f4a7c15ULL));
    llvm::Value* mixed2 = builder.CreateXor(mixed1, builder.CreateLShr(mixed1, builder.getInt64(32)));
    
    builder.CreateRet(mixed2);
    
    return func;
}

llvm::Value* HardwareCacheObfuscation::createRDTSC(llvm::IRBuilder<>& builder) {
    // Create inline assembly for RDTSC (Read Time-Stamp Counter)
    // Returns 64-bit cycle count
    
    llvm::FunctionType* rdtscType = llvm::FunctionType::get(
        builder.getInt64Ty(), false);
    
    // RDTSC instruction: returns EDX:EAX (high:low 32 bits)
    // We combine them into 64-bit value
    llvm::InlineAsm* rdtscAsm = llvm::InlineAsm::get(
        rdtscType,
        "rdtsc; shl $$32, %rdx; or %rdx, %rax",
        "={rax},~{rdx}",
        true,  // has side effects
        false, // not align stack
        llvm::InlineAsm::AD_ATT);
    
    std::vector<llvm::Value*> args;
    llvm::Value* result = builder.CreateCall(rdtscAsm, args);
    
    return result;
}

uint32_t HardwareCacheObfuscation::applyCacheBasedXOR(llvm::Module& module, llvm::Function* cacheKeyFunc) {
    uint32_t count = 0;
    
    if (!cacheKeyFunc) {
        return 0;
    }
    
    // Process each function
    for (auto& func : module) {
        if (!shouldObfuscateFunction(func)) continue;
        
        // Generate cache key once at function entry
        llvm::BasicBlock* entryBB = &func.getEntryBlock();
        llvm::IRBuilder<> entryBuilder(entryBB, entryBB->getFirstInsertionPt());
        llvm::Value* cacheKey = entryBuilder.CreateCall(cacheKeyFunc);
        
        // Collect binary operations with constants in this function
        std::vector<std::pair<llvm::BinaryOperator*, llvm::ConstantInt*>> candidates;
        for (auto& bb : func) {
            for (auto& inst : bb) {
                if (auto* binOp = llvm::dyn_cast<llvm::BinaryOperator>(&inst)) {
                    if (auto* ci = llvm::dyn_cast<llvm::ConstantInt>(binOp->getOperand(1))) {
                        if (ci->getBitWidth() <= 64 && count < (intensity_ / 10)) {
                            candidates.push_back({binOp, ci});
                        }
                    }
                }
            }
        }
        
        // Transform the candidates
        for (auto& [binOp, constOp] : candidates) {
            // Insert transformations BEFORE the binary operation
            llvm::IRBuilder<> builder(binOp);
            
            // Truncate cache key to constant size
            llvm::Value* keyTrunc = builder.CreateTrunc(cacheKey, constOp->getType());
            
            // Create obfuscated constant: (const ^ key) ^ key = const
            llvm::Value* obfuscated = builder.CreateXor(constOp, keyTrunc);
            llvm::Value* restored = builder.CreateXor(obfuscated, keyTrunc);
            
            // Replace constant with restored value
            binOp->setOperand(1, restored);
            
            count++;
        }
    }
    
    return count;
}

} // namespace obfuscator
