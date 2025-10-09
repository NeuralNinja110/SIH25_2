/**
 * @file ConstantObfuscation.cpp
 * @brief Implementation of constant obfuscation pass
 * @version 1.0.0
 * @date 2025-10-09
 */

#include "passes/ConstantObfuscation.h"
#include "MetricsCollector.h"
#include "RandomGenerator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Constants.h"

namespace obfuscator {

ConstantObfuscation::ConstantObfuscation(uint32_t complexity)
    : ObfuscationPass("ConstantObfuscation", true), complexity_(complexity) {
}

bool ConstantObfuscation::runOnModule(llvm::Module& module, MetricsCollector& metrics) {
    bool modified = false;
    uint32_t totalObfuscated = 0;
    
    for (auto& func : module) {
        // Skip if already processed
        if (func.getMetadata("obfuscated.ConstantObfuscation")) {
            continue;
        }
        
        if (shouldObfuscateFunction(func)) {
            uint32_t obfuscated = obfuscateConstants(func);
            totalObfuscated += obfuscated;
            if (obfuscated > 0) {
                // Mark function as obfuscated
                llvm::LLVMContext& ctx = func.getContext();
                llvm::MDNode* node = llvm::MDNode::get(ctx, 
                    llvm::MDString::get(ctx, "ConstantObfuscation"));
                func.setMetadata("obfuscated.ConstantObfuscation", node);
                modified = true;
            }
        }
    }
    
    metrics.incrementTransformations(name_, totalObfuscated);
    metrics.getMetricsMutable().constantsObfuscated += totalObfuscated;
    
    return modified;
}

uint32_t ConstantObfuscation::obfuscateConstants(llvm::Function& func) {
    uint32_t count = 0;
    auto& rng = RandomGenerator::getInstance();
    
    std::vector<std::pair<llvm::Instruction*, llvm::ConstantInt*>> constantUsers;
    
    // Find instructions using integer constants
    for (auto& bb : func) {
        for (auto& inst : bb) {
            for (auto& operand : inst.operands()) {
                if (auto* constInt = llvm::dyn_cast<llvm::ConstantInt>(operand.get())) {
                    if (constInt->getBitWidth() == 32 || constInt->getBitWidth() == 64) {
                        int64_t value = constInt->getSExtValue();
                        // Skip small constants and special values
                        if (value > 10 && value < 1000000 && rng.getBool(complexity_)) {
                            constantUsers.push_back({&inst, constInt});
                        }
                    }
                }
            }
        }
    }
    
    // Obfuscate constants
    for (auto& pair : constantUsers) {
        llvm::Instruction* inst = pair.first;
        llvm::ConstantInt* constInt = pair.second;
        int64_t value = constInt->getSExtValue();
        unsigned bitWidth = constInt->getBitWidth();
        
        llvm::IRBuilder<> builder(inst);
        
        // Choose obfuscation method
        uint32_t method = rng.getUInt32(0, 2);
        llvm::Value* obfuscated = nullptr;
        
        // Helper lambda to create constant with correct bit width
        auto getConstant = [&](int64_t val) -> llvm::ConstantInt* {
            if (bitWidth == 64) {
                return builder.getInt64(val);
            } else {
                return builder.getInt32(static_cast<int32_t>(val));
            }
        };
        
        switch (method) {
            case 0: {
                // Split into sum: C = A + B
                int64_t a = rng.getUInt32(1, std::abs(value) / 2 + 1);
                int64_t b = value - a;
                obfuscated = builder.CreateAdd(
                    getConstant(a),
                    getConstant(b)
                );
                break;
            }
            case 1: {
                // Use XOR: C = A ^ B where A ^ B = C
                int64_t a = rng.getUInt32(1, 65535);
                int64_t b = a ^ value;
                obfuscated = builder.CreateXor(
                    getConstant(a),
                    getConstant(b)
                );
                break;
            }
            case 2: {
                // Use multiplication: C = A * B
                if (value % 2 == 0 && value != 0) {
                    int64_t a = 2;
                    int64_t b = value / 2;
                    obfuscated = builder.CreateMul(
                        getConstant(a),
                        getConstant(b)
                    );
                } else {
                    // Fallback to addition
                    int64_t a = rng.getUInt32(1, std::abs(value) / 2 + 1);
                    int64_t b = value - a;
                    obfuscated = builder.CreateAdd(
                        getConstant(a),
                        getConstant(b)
                    );
                }
                break;
            }
        }
        
        if (obfuscated) {
            // Replace constant with obfuscated value
            for (auto& operand : inst->operands()) {
                if (operand.get() == constInt) {
                    operand.set(obfuscated);
                    count++;
                    break;
                }
            }
        }
    }
    
    return count;
}

} // namespace obfuscator
