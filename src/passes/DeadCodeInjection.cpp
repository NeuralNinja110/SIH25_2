/**
 * @file DeadCodeInjection.cpp
 * @brief Implementation of dead code injection pass
 * @version 1.0.0
 * @date 2025-10-09
 */

#include "passes/DeadCodeInjection.h"
#include "MetricsCollector.h"
#include "RandomGenerator.h"
#include "llvm/IR/IRBuilder.h"

namespace obfuscator {

DeadCodeInjection::DeadCodeInjection(uint32_t ratio)
    : ObfuscationPass("DeadCodeInjection", true), ratio_(ratio) {
}

bool DeadCodeInjection::runOnModule(llvm::Module& module, MetricsCollector& metrics) {
    bool modified = false;
    uint32_t totalDeadInstructions = 0;
    
    for (auto& func : module) {
        // Skip if already processed
        if (func.getMetadata("obfuscated.DeadCodeInjection")) {
            continue;
        }
        
        if (shouldObfuscateFunction(func)) {
            uint32_t injected = injectDeadCode(func);
            totalDeadInstructions += injected;
            if (injected > 0) {
                // Mark function as obfuscated
                llvm::LLVMContext& ctx = func.getContext();
                llvm::MDNode* node = llvm::MDNode::get(ctx, 
                    llvm::MDString::get(ctx, "DeadCodeInjection"));
                func.setMetadata("obfuscated.DeadCodeInjection", node);
                modified = true;
            }
        }
    }
    
    metrics.incrementTransformations(name_, totalDeadInstructions);
    metrics.getMetricsMutable().deadCodeInstructionsAdded += totalDeadInstructions;
    
    return modified;
}

uint32_t DeadCodeInjection::injectDeadCode(llvm::Function& func) {
    uint32_t count = 0;
    auto& rng = RandomGenerator::getInstance();
    
    std::vector<llvm::Instruction*> insertPoints;
    
    for (auto& bb : func) {
        for (auto& inst : bb) {
            if (!inst.isTerminator()) {
                if (rng.getBool(ratio_)) {
                    insertPoints.push_back(&inst);
                }
            }
        }
    }
    
    for (auto* inst : insertPoints) {
        llvm::IRBuilder<> builder(inst);
        
        // Generate random dead code patterns
        uint32_t pattern = rng.getUInt32(0, 4);
        
        switch (pattern) {
            case 0: {
                // Dead arithmetic
                llvm::Value* a = builder.getInt32(rng.getUInt32(1, 100));
                llvm::Value* b = builder.getInt32(rng.getUInt32(1, 100));
                llvm::Value* result = builder.CreateAdd(a, b);
                result = builder.CreateMul(result, builder.getInt32(2));
                count += 2;
                break;
            }
            case 1: {
                // Dead comparison
                llvm::Value* x = builder.getInt32(rng.getUInt32(1, 100));
                llvm::Value* y = builder.getInt32(rng.getUInt32(1, 100));
                llvm::Value* cmp = builder.CreateICmpSGT(x, y);
                count += 1;
                break;
            }
            case 2: {
                // Dead bitwise operations
                llvm::Value* val = builder.getInt32(rng.getUInt32(1, 255));
                llvm::Value* result = builder.CreateXor(val, builder.getInt32(0xFF));
                result = builder.CreateAnd(result, builder.getInt32(0x0F));
                count += 2;
                break;
            }
            case 3: {
                // Dead shift operations
                llvm::Value* val = builder.getInt32(rng.getUInt32(1, 100));
                llvm::Value* shifted = builder.CreateShl(val, builder.getInt32(2));
                shifted = builder.CreateLShr(shifted, builder.getInt32(2));
                count += 2;
                break;
            }
            case 4: {
                // Dead select
                llvm::Value* cond = builder.getInt1(rng.getBool(50));
                llvm::Value* a = builder.getInt32(rng.getUInt32(1, 100));
                llvm::Value* b = builder.getInt32(rng.getUInt32(1, 100));
                llvm::Value* result = builder.CreateSelect(cond, a, b);
                count += 1;
                break;
            }
        }
    }
    
    return count;
}

} // namespace obfuscator
