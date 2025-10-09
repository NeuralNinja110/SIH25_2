/**
 * @file ControlFlowFlattening.cpp
 * @brief Implementation of control flow flattening pass
 * @version 1.0.0
 * @date 2025-10-09
 */

#include "passes/ControlFlowFlattening.h"
#include "MetricsCollector.h"
#include "RandomGenerator.h"
#include "Logger.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Constants.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <vector>

namespace obfuscator {

ControlFlowFlattening::ControlFlowFlattening(uint32_t complexity)
    : ObfuscationPass("ControlFlowFlattening", true), complexity_(complexity) {
}

bool ControlFlowFlattening::runOnModule(llvm::Module& module, MetricsCollector& metrics) {
    bool modified = false;
    uint32_t transformationCount = 0;
    
    for (auto& func : module) {
        // Skip if already flattened
        if (func.getMetadata("obfuscated.ControlFlowFlattening")) {
            continue;
        }
        
        if (shouldObfuscateFunction(func) && canFlatten(func)) {
            if (flattenFunction(func)) {
                // Mark function as flattened
                llvm::LLVMContext& ctx = func.getContext();
                llvm::MDNode* node = llvm::MDNode::get(ctx, 
                    llvm::MDString::get(ctx, "ControlFlowFlattening"));
                func.setMetadata("obfuscated.ControlFlowFlattening", node);
                
                modified = true;
                transformationCount++;
            }
        }
    }
    
    metrics.incrementTransformations(name_, transformationCount);
    metrics.getMetricsMutable().controlFlowTransformations += transformationCount;
    
    return modified;
}

bool ControlFlowFlattening::canFlatten(llvm::Function& func) const {
    // Don't flatten very small functions
    if (func.size() < 4) {
        return false;
    }
    
    // Don't flatten if too complex (too many blocks)
    if (func.size() > 100) {
        return false;
    }
    
    return true;
}

bool ControlFlowFlattening::flattenFunction(llvm::Function& func) {
    Logger::getInstance().debug("Flattening function: " + func.getName().str());
    
    // Get entry block
    llvm::BasicBlock& entryBlock = func.getEntryBlock();
    
    // Collect all basic blocks except entry
    std::vector<llvm::BasicBlock*> originalBlocks;
    for (auto& bb : func) {
        if (&bb != &entryBlock) {
            originalBlocks.push_back(&bb);
        }
    }
    
    if (originalBlocks.empty()) {
        return false;
    }
    
    // Create switch variable
    llvm::IRBuilder<> builder(&entryBlock, entryBlock.begin());
    llvm::AllocaInst* switchVar = builder.CreateAlloca(
        builder.getInt32Ty(), nullptr, "switch.var");
    
    // Initialize switch variable with first block index
    builder.CreateStore(builder.getInt32(1), switchVar);
    
    // Create dispatch block with switch statement
    llvm::BasicBlock* dispatchBlock = llvm::BasicBlock::Create(
        func.getContext(), "dispatch", &func);
    
    // Create default block
    llvm::BasicBlock* defaultBlock = llvm::BasicBlock::Create(
        func.getContext(), "default", &func);
    llvm::IRBuilder<> defaultBuilder(defaultBlock);
    defaultBuilder.CreateUnreachable();
    
    // Build switch instruction
    llvm::IRBuilder<> dispatchBuilder(dispatchBlock);
    llvm::LoadInst* switchValue = dispatchBuilder.CreateLoad(
        builder.getInt32Ty(), switchVar, "switch.val");
    llvm::SwitchInst* switchInst = dispatchBuilder.CreateSwitch(
        switchValue, defaultBlock, originalBlocks.size());
    
    // Assign case numbers to blocks and update terminators
    for (size_t i = 0; i < originalBlocks.size(); ++i) {
        llvm::BasicBlock* bb = originalBlocks[i];
        uint32_t caseNum = i + 1;
        
        // Add case to switch
        switchInst->addCase(builder.getInt32(caseNum), bb);
        
        // Update terminator to go back to dispatch
        llvm::Instruction* terminator = bb->getTerminator();
        if (!terminator) continue;
        
        if (auto* br = llvm::dyn_cast<llvm::BranchInst>(terminator)) {
            llvm::IRBuilder<> bbBuilder(bb);
            bbBuilder.SetInsertPoint(terminator);
            
            if (br->isUnconditional()) {
                // Find destination block index
                llvm::BasicBlock* dest = br->getSuccessor(0);
                auto it = std::find(originalBlocks.begin(), originalBlocks.end(), dest);
                if (it != originalBlocks.end()) {
                    uint32_t destCase = std::distance(originalBlocks.begin(), it) + 1;
                    bbBuilder.CreateStore(builder.getInt32(destCase), switchVar);
                }
                bbBuilder.CreateBr(dispatchBlock);
                terminator->eraseFromParent();
            } else {
                // Conditional branch - keep condition but update destinations
                llvm::Value* cond = br->getCondition();
                llvm::BasicBlock* trueDest = br->getSuccessor(0);
                llvm::BasicBlock* falseDest = br->getSuccessor(1);
                
                auto trueIt = std::find(originalBlocks.begin(), originalBlocks.end(), trueDest);
                auto falseIt = std::find(originalBlocks.begin(), originalBlocks.end(), falseDest);
                
                if (trueIt != originalBlocks.end() && falseIt != originalBlocks.end()) {
                    uint32_t trueCase = std::distance(originalBlocks.begin(), trueIt) + 1;
                    uint32_t falseCase = std::distance(originalBlocks.begin(), falseIt) + 1;
                    
                    llvm::Value* selectedCase = bbBuilder.CreateSelect(
                        cond, builder.getInt32(trueCase), builder.getInt32(falseCase));
                    bbBuilder.CreateStore(selectedCase, switchVar);
                }
                bbBuilder.CreateBr(dispatchBlock);
                terminator->eraseFromParent();
            }
        } else if (llvm::isa<llvm::ReturnInst>(terminator)) {
            // Leave return instructions as is
        }
    }
    
    // Update entry block to jump to dispatch
    llvm::Instruction* entryTerm = entryBlock.getTerminator();
    if (entryTerm) {
        llvm::IRBuilder<> entryBuilder(&entryBlock);
        entryBuilder.SetInsertPoint(entryTerm);
        entryBuilder.CreateBr(dispatchBlock);
        entryTerm->eraseFromParent();
    } else {
        builder.SetInsertPoint(&entryBlock, entryBlock.end());
        builder.CreateBr(dispatchBlock);
    }
    
    return true;
}

} // namespace obfuscator
