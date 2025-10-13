/**
 * @file GrammarMetamorphic.cpp
 * @brief Implementation of grammar-based metamorphic code generation
 * @version 2.0.0
 * @date 2025-10-13
 */

#include "passes/GrammarMetamorphic.h"
#include "MetricsCollector.h"
#include "RandomGenerator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"

namespace obfuscator {

GrammarMetamorphic::GrammarMetamorphic(uint32_t transformationRate)
    : ObfuscationPass("GrammarMetamorphic", true), transformationRate_(transformationRate) {
}

bool GrammarMetamorphic::runOnModule(llvm::Module& module, MetricsCollector& metrics) {
    bool modified = false;
    uint32_t totalTransformations = 0;
    
    for (auto& func : module) {
        if (func.getMetadata("obfuscated.GrammarMetamorphic")) {
            continue;
        }
        
        if (shouldObfuscateFunction(func)) {
            uint32_t transformed = transformFunction(func);
            totalTransformations += transformed;
            if (transformed > 0) {
                llvm::LLVMContext& ctx = func.getContext();
                llvm::MDNode* node = llvm::MDNode::get(ctx,
                    llvm::MDString::get(ctx, "GrammarMetamorphic"));
                func.setMetadata("obfuscated.GrammarMetamorphic", node);
                modified = true;
            }
        }
    }
    
    metrics.incrementTransformations(name_, totalTransformations);
    
    return modified;
}

uint32_t GrammarMetamorphic::transformFunction(llvm::Function& func) {
    uint32_t transformed = 0;
    auto& rng = RandomGenerator::getInstance();
    
    std::vector<llvm::Instruction*> candidates;
    
    // Collect candidate instructions for grammar transformations
    for (auto& bb : func) {
        for (auto& inst : bb) {
            if (rng.getUInt32(0, 99) < transformationRate_) {
                // Binary operations
                if (auto* binOp = llvm::dyn_cast<llvm::BinaryOperator>(&inst)) {
                    if (binOp->getType()->isIntegerTy()) {
                        candidates.push_back(binOp);
                    }
                }
                // Branches
                else if (auto* branch = llvm::dyn_cast<llvm::BranchInst>(&inst)) {
                    if (branch->isConditional()) {
                        candidates.push_back(branch);
                    }
                }
            }
        }
    }
    
    // Apply grammar transformations
    for (auto* inst : candidates) {
        bool success = false;
        
        if (auto* binOp = llvm::dyn_cast<llvm::BinaryOperator>(inst)) {
            switch (binOp->getOpcode()) {
                case llvm::Instruction::Add:
                    success = applyAdditionToSubtraction(binOp);
                    break;
                case llvm::Instruction::Mul:
                    success = applyMultiplicationToShift(binOp);
                    break;
                case llvm::Instruction::UDiv:
                case llvm::Instruction::SDiv:
                    // Division to multiply is complex, skip for now
                    break;
                default:
                    // Try associativity transform
                    success = applyAssociativityTransform(binOp);
                    break;
            }
        }
        else if (auto* branch = llvm::dyn_cast<llvm::BranchInst>(inst)) {
            success = applyBranchInversion(branch);
        }
        
        if (success) {
            transformed++;
        }
    }
    
    return transformed;
}

bool GrammarMetamorphic::applyAdditionToSubtraction(llvm::BinaryOperator* add) {
    // Grammar rule: a = b + c → a = b - (-c)
    
    llvm::IRBuilder<> builder(add);
    llvm::Value* op1 = add->getOperand(0);
    llvm::Value* op2 = add->getOperand(1);
    
    // Create -c
    llvm::Value* negOp2 = builder.CreateNeg(op2);
    
    // Create b - (-c)
    llvm::Value* result = builder.CreateSub(op1, negOp2);
    
    // Replace uses
    add->replaceAllUsesWith(result);
    add->eraseFromParent();
    
    return true;
}

bool GrammarMetamorphic::applyMultiplicationToShift(llvm::BinaryOperator* mul) {
    // Grammar rule: a = b * c → a = b << log2(c) [if c is power of 2]
    
    uint32_t log2Val = 0;
    llvm::Value* op2 = mul->getOperand(1);
    
    if (!isPowerOfTwo(op2, log2Val)) {
        return false;  // Only works for power-of-2 constants
    }
    
    llvm::IRBuilder<> builder(mul);
    llvm::Value* op1 = mul->getOperand(0);
    
    // Create shift amount
    llvm::Value* shiftAmount = llvm::ConstantInt::get(op1->getType(), log2Val);
    
    // Create b << log2(c)
    llvm::Value* result = builder.CreateShl(op1, shiftAmount);
    
    // Replace uses
    mul->replaceAllUsesWith(result);
    mul->eraseFromParent();
    
    return true;
}

bool GrammarMetamorphic::applyDivisionToMultiply(llvm::BinaryOperator* div) {
    // This is complex for general case, skip for now
    return false;
}

bool GrammarMetamorphic::applyBranchInversion(llvm::BranchInst* branch) {
    // Grammar rule: if (a) then B else C → if (!a) then C else B
    
    if (!branch->isConditional()) {
        return false;
    }
    
    llvm::Value* condition = branch->getCondition();
    llvm::BasicBlock* trueBB = branch->getSuccessor(0);
    llvm::BasicBlock* falseBB = branch->getSuccessor(1);
    
    // Don't invert if blocks are the same
    if (trueBB == falseBB) {
        return false;
    }
    
    llvm::IRBuilder<> builder(branch);
    
    // Create !a
    llvm::Value* notCondition = nullptr;
    if (auto* icmp = llvm::dyn_cast<llvm::ICmpInst>(condition)) {
        // Invert comparison predicate
        notCondition = builder.CreateICmp(
            llvm::ICmpInst::getInversePredicate(icmp->getPredicate()),
            icmp->getOperand(0),
            icmp->getOperand(1));
    } else {
        // General boolean negation
        notCondition = builder.CreateNot(condition);
    }
    
    // Create new branch: if (!a) then C else B (swap successors)
    builder.CreateCondBr(notCondition, falseBB, trueBB);
    
    // Remove old branch
    branch->eraseFromParent();
    
    return true;
}

bool GrammarMetamorphic::applyAssociativityTransform(llvm::BinaryOperator* binOp) {
    // Grammar rule: x = (a op1 b) op2 c → x = a op1 (b op2 c)
    // Only safe for associative operations (Add, Mul, And, Or, Xor)
    
    llvm::Instruction::BinaryOps opcode = binOp->getOpcode();
    
    // Check if operation is associative
    if (opcode != llvm::Instruction::Add &&
        opcode != llvm::Instruction::Mul &&
        opcode != llvm::Instruction::And &&
        opcode != llvm::Instruction::Or &&
        opcode != llvm::Instruction::Xor) {
        return false;
    }
    
    // Check if left operand is also a binary operation of the same type
    auto* leftBinOp = llvm::dyn_cast<llvm::BinaryOperator>(binOp->getOperand(0));
    if (!leftBinOp || leftBinOp->getOpcode() != opcode) {
        return false;
    }
    
    // x = (a op b) op c
    llvm::Value* a = leftBinOp->getOperand(0);
    llvm::Value* b = leftBinOp->getOperand(1);
    llvm::Value* c = binOp->getOperand(1);
    
    llvm::IRBuilder<> builder(binOp);
    
    // Create (b op c)
    llvm::Value* bc = builder.CreateBinOp(opcode, b, c);
    
    // Create a op (b op c)
    llvm::Value* result = builder.CreateBinOp(opcode, a, bc);
    
    // Replace uses
    binOp->replaceAllUsesWith(result);
    binOp->eraseFromParent();
    
    return true;
}

bool GrammarMetamorphic::isPowerOfTwo(llvm::Value* val, uint32_t& log2Val) {
    auto* constInt = llvm::dyn_cast<llvm::ConstantInt>(val);
    if (!constInt) {
        return false;
    }
    
    uint64_t value = constInt->getZExtValue();
    if (value == 0 || (value & (value - 1)) != 0) {
        return false;  // Not a power of 2
    }
    
    // Calculate log2
    log2Val = 0;
    while (value > 1) {
        value >>= 1;
        log2Val++;
    }
    
    return true;
}

} // namespace obfuscator
