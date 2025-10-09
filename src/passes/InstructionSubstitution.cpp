/**
 * @file InstructionSubstitution.cpp
 * @brief Implementation of instruction substitution pass
 * @version 1.0.0
 * @date 2025-10-09
 */

#include "passes/InstructionSubstitution.h"
#include "MetricsCollector.h"
#include "RandomGenerator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"

namespace obfuscator {

InstructionSubstitution::InstructionSubstitution(uint32_t probability)
    : ObfuscationPass("InstructionSubstitution", true), probability_(probability) {
}

bool InstructionSubstitution::runOnModule(llvm::Module& module, MetricsCollector& metrics) {
    bool modified = false;
    uint32_t totalSubstitutions = 0;
    
    for (auto& func : module) {
        // Skip if already processed
        if (func.getMetadata("obfuscated.InstructionSubstitution")) {
            continue;
        }
        
        if (shouldObfuscateFunction(func)) {
            uint32_t substituted = substituteInstructions(func);
            totalSubstitutions += substituted;
            if (substituted > 0) {
                // Mark function as obfuscated
                llvm::LLVMContext& ctx = func.getContext();
                llvm::MDNode* node = llvm::MDNode::get(ctx, 
                    llvm::MDString::get(ctx, "InstructionSubstitution"));
                func.setMetadata("obfuscated.InstructionSubstitution", node);
                modified = true;
            }
        }
    }
    
    metrics.incrementTransformations(name_, totalSubstitutions);
    metrics.getMetricsMutable().instructionSubstitutions += totalSubstitutions;
    
    return modified;
}

uint32_t InstructionSubstitution::substituteInstructions(llvm::Function& func) {
    uint32_t count = 0;
    auto& rng = RandomGenerator::getInstance();
    
    std::vector<llvm::Instruction*> toReplace;
    
    for (auto& bb : func) {
        for (auto& inst : bb) {
            if (auto* binOp = llvm::dyn_cast<llvm::BinaryOperator>(&inst)) {
                if (rng.getBool(probability_)) {
                    toReplace.push_back(&inst);
                }
            }
        }
    }
    
    for (auto* inst : toReplace) {
        auto* binOp = llvm::cast<llvm::BinaryOperator>(inst);
        llvm::Value* replacement = nullptr;
        
        switch (binOp->getOpcode()) {
            case llvm::Instruction::Add:
                replacement = substituteAdd(binOp);
                break;
            case llvm::Instruction::Sub:
                replacement = substituteSub(binOp);
                break;
            case llvm::Instruction::Xor:
                replacement = substituteXor(binOp);
                break;
            default:
                continue;
        }
        
        if (replacement) {
            inst->replaceAllUsesWith(replacement);
            count++;
        }
    }
    
    return count;
}

llvm::Value* InstructionSubstitution::substituteAdd(llvm::BinaryOperator* inst) {
    // Replace: a + b
    // With: (a - (-b))
    llvm::IRBuilder<> builder(inst);
    llvm::Value* a = inst->getOperand(0);
    llvm::Value* b = inst->getOperand(1);
    
    llvm::Value* negB = builder.CreateNeg(b);
    return builder.CreateSub(a, negB);
}

llvm::Value* InstructionSubstitution::substituteSub(llvm::BinaryOperator* inst) {
    // Replace: a - b
    // With: (a + (-b))
    llvm::IRBuilder<> builder(inst);
    llvm::Value* a = inst->getOperand(0);
    llvm::Value* b = inst->getOperand(1);
    
    llvm::Value* negB = builder.CreateNeg(b);
    return builder.CreateAdd(a, negB);
}

llvm::Value* InstructionSubstitution::substituteXor(llvm::BinaryOperator* inst) {
    // Replace: a ^ b
    // With: (a | b) & ~(a & b)
    llvm::IRBuilder<> builder(inst);
    llvm::Value* a = inst->getOperand(0);
    llvm::Value* b = inst->getOperand(1);
    
    llvm::Value* orVal = builder.CreateOr(a, b);
    llvm::Value* andVal = builder.CreateAnd(a, b);
    llvm::Value* notAnd = builder.CreateNot(andVal);
    return builder.CreateAnd(orVal, notAnd);
}

} // namespace obfuscator
