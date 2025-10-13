/**
 * @file MBAObfuscation.cpp
 * @brief Implementation of Mixed Boolean-Arithmetic obfuscation
 * @version 2.0.0
 * @date 2025-10-13
 */

#include "passes/MBAObfuscation.h"
#include "MetricsCollector.h"
#include "RandomGenerator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"

namespace obfuscator {

MBAObfuscation::MBAObfuscation(uint32_t probability)
    : ObfuscationPass("MBAObfuscation", true), probability_(probability) {
}

bool MBAObfuscation::runOnModule(llvm::Module& module, MetricsCollector& metrics) {
    bool modified = false;
    uint32_t totalTransformations = 0;
    
    for (auto& func : module) {
        if (func.getMetadata("obfuscated.MBAObfuscation")) {
            continue;
        }
        
        if (shouldObfuscateFunction(func)) {
            uint32_t transformed = transformArithmeticOperations(func);
            totalTransformations += transformed;
            if (transformed > 0) {
                llvm::LLVMContext& ctx = func.getContext();
                llvm::MDNode* node = llvm::MDNode::get(ctx,
                    llvm::MDString::get(ctx, "MBAObfuscation"));
                func.setMetadata("obfuscated.MBAObfuscation", node);
                modified = true;
            }
        }
    }
    
    metrics.incrementTransformations(name_, totalTransformations);
    
    return modified;
}

uint32_t MBAObfuscation::transformArithmeticOperations(llvm::Function& func) {
    uint32_t transformed = 0;
    auto& rng = RandomGenerator::getInstance();
    
    std::vector<llvm::Instruction*> candidates;
    
    // Collect candidate instructions
    for (auto& bb : func) {
        for (auto& inst : bb) {
            if (auto* binOp = llvm::dyn_cast<llvm::BinaryOperator>(&inst)) {
                // Only transform integer operations
                if (binOp->getType()->isIntegerTy()) {
                    llvm::Instruction::BinaryOps opcode = binOp->getOpcode();
                    if (opcode == llvm::Instruction::Add ||
                        opcode == llvm::Instruction::Sub ||
                        opcode == llvm::Instruction::And ||
                        opcode == llvm::Instruction::Or ||
                        opcode == llvm::Instruction::Xor) {
                        candidates.push_back(binOp);
                    }
                }
            }
        }
    }
    
    // Transform selected candidates
    for (auto* inst : candidates) {
        // Apply probability
        if (rng.getUInt32(0, 99) >= probability_) {
            continue;
        }
        
        auto* binOp = llvm::cast<llvm::BinaryOperator>(inst);
        llvm::IRBuilder<> builder(binOp);
        
        llvm::Value* op1 = binOp->getOperand(0);
        llvm::Value* op2 = binOp->getOperand(1);
        llvm::Value* mbaResult = nullptr;
        
        switch (binOp->getOpcode()) {
            case llvm::Instruction::Add:
                mbaResult = generateMBAAdd(builder, op1, op2);
                break;
            case llvm::Instruction::Sub:
                mbaResult = generateMBASub(builder, op1, op2);
                break;
            case llvm::Instruction::And:
                mbaResult = generateMBAAnd(builder, op1, op2);
                break;
            case llvm::Instruction::Or:
                mbaResult = generateMBAOr(builder, op1, op2);
                break;
            case llvm::Instruction::Xor:
                mbaResult = generateMBAXor(builder, op1, op2);
                break;
            default:
                continue;
        }
        
        if (mbaResult) {
            // Add noise cancellation for extra complexity
            mbaResult = addNoiseCancellation(builder, mbaResult);
            
            binOp->replaceAllUsesWith(mbaResult);
            binOp->eraseFromParent();
            transformed++;
        }
    }
    
    return transformed;
}

llvm::Value* MBAObfuscation::generateMBAAdd(llvm::IRBuilder<>& builder,
                                            llvm::Value* a, llvm::Value* b) {
    // MBA identity: a + b = (a ^ b) + 2 * (a & b)
    
    llvm::Value* xorPart = builder.CreateXor(a, b);
    llvm::Value* andPart = builder.CreateAnd(a, b);
    llvm::Value* one = llvm::ConstantInt::get(andPart->getType(), 1);
    llvm::Value* doubleAnd = builder.CreateShl(andPart, one);
    llvm::Value* result = builder.CreateAdd(xorPart, doubleAnd);
    
    return result;
}

llvm::Value* MBAObfuscation::generateMBASub(llvm::IRBuilder<>& builder,
                                            llvm::Value* a, llvm::Value* b) {
    // MBA identity: a - b = (a ^ b) - 2 * (~a & b)
    
    llvm::Value* xorPart = builder.CreateXor(a, b);
    llvm::Value* notA = builder.CreateNot(a);
    llvm::Value* andPart = builder.CreateAnd(notA, b);
    llvm::Value* one = llvm::ConstantInt::get(andPart->getType(), 1);
    llvm::Value* doubleAnd = builder.CreateShl(andPart, one);
    llvm::Value* result = builder.CreateSub(xorPart, doubleAnd);
    
    return result;
}

llvm::Value* MBAObfuscation::generateMBAAnd(llvm::IRBuilder<>& builder,
                                            llvm::Value* a, llvm::Value* b) {
    // MBA identity: a & b = (a + b - (a | b)) / 2
    
    llvm::Value* sum = builder.CreateAdd(a, b);
    llvm::Value* orPart = builder.CreateOr(a, b);
    llvm::Value* diff = builder.CreateSub(sum, orPart);
    llvm::Value* one = llvm::ConstantInt::get(diff->getType(), 1);
    llvm::Value* result = builder.CreateLShr(diff, one);
    
    return result;
}

llvm::Value* MBAObfuscation::generateMBAOr(llvm::IRBuilder<>& builder,
                                           llvm::Value* a, llvm::Value* b) {
    // MBA identity: a | b = a + b - (a & b)
    
    llvm::Value* sum = builder.CreateAdd(a, b);
    llvm::Value* andPart = builder.CreateAnd(a, b);
    llvm::Value* result = builder.CreateSub(sum, andPart);
    
    return result;
}

llvm::Value* MBAObfuscation::generateMBAXor(llvm::IRBuilder<>& builder,
                                            llvm::Value* a, llvm::Value* b) {
    // MBA identity: a ^ b = (a | b) - (a & b)
    
    llvm::Value* orPart = builder.CreateOr(a, b);
    llvm::Value* andPart = builder.CreateAnd(a, b);
    llvm::Value* result = builder.CreateSub(orPart, andPart);
    
    return result;
}

llvm::Value* MBAObfuscation::addNoiseCancellation(llvm::IRBuilder<>& builder,
                                                  llvm::Value* expr) {
    auto& rng = RandomGenerator::getInstance();
    
    // Generate random noise value
    uint64_t noiseValue = rng.getUInt32(1, 1000);
    llvm::Value* noise = llvm::ConstantInt::get(expr->getType(), noiseValue);
    
    // Add noise and immediately subtract it: (expr + noise) - noise
    llvm::Value* withNoise = builder.CreateAdd(expr, noise);
    llvm::Value* result = builder.CreateSub(withNoise, noise);
    
    return result;
}

} // namespace obfuscator
