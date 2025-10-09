/**
 * @file BogusControlFlow.cpp
 * @brief Implementation of bogus control flow pass
 * @version 1.0.0
 * @date 2025-10-09
 */

#include "passes/BogusControlFlow.h"
#include "MetricsCollector.h"
#include "RandomGenerator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

namespace obfuscator {

BogusControlFlow::BogusControlFlow(uint32_t probability)
    : ObfuscationPass("BogusControlFlow", true), probability_(probability) {
}

bool BogusControlFlow::runOnModule(llvm::Module& module, MetricsCollector& metrics) {
    bool modified = false;
    uint32_t totalBogusBlocks = 0;
    
    for (auto& func : module) {
        // Skip if already processed
        if (func.getMetadata("obfuscated.BogusControlFlow")) {
            continue;
        }
        
        if (shouldObfuscateFunction(func)) {
            uint32_t added = addBogusBlocks(func);
            totalBogusBlocks += added;
            if (added > 0) {
                // Mark function as obfuscated
                llvm::LLVMContext& ctx = func.getContext();
                llvm::MDNode* node = llvm::MDNode::get(ctx, 
                    llvm::MDString::get(ctx, "BogusControlFlow"));
                func.setMetadata("obfuscated.BogusControlFlow", node);
                modified = true;
            }
        }
    }
    
    metrics.incrementTransformations(name_, totalBogusBlocks);
    metrics.getMetricsMutable().bogusBlocksAdded += totalBogusBlocks;
    
    return modified;
}

uint32_t BogusControlFlow::addBogusBlocks(llvm::Function& func) {
    uint32_t count = 0;
    auto& rng = RandomGenerator::getInstance();
    
    std::vector<llvm::BasicBlock*> originalBlocks;
    for (auto& bb : func) {
        originalBlocks.push_back(&bb);
    }
    
    for (auto* bb : originalBlocks) {
        if (!rng.getBool(probability_)) {
            continue;
        }
        
        // Don't modify blocks with only terminator
        if (bb->size() <= 1) {
            continue;
        }
        
        // Create bogus block
        createBogusBlock(bb);
        count++;
    }
    
    return count;
}

void BogusControlFlow::createBogusBlock(llvm::BasicBlock* original) {
    auto& rng = RandomGenerator::getInstance();
    
    // Find a split point
    auto it = original->begin();
    uint32_t pos = rng.getUInt32(1, original->size() - 1);
    std::advance(it, pos);
    
    if (it->isTerminator()) {
        return;
    }
    
    // Split the basic block
    llvm::BasicBlock* afterBlock = original->splitBasicBlock(it, "after.bogus");
    
    // Create bogus block with impossible condition
    llvm::BasicBlock* bogusBlock = llvm::BasicBlock::Create(
        original->getContext(), "bogus.block", original->getParent(), afterBlock);
    
    llvm::IRBuilder<> bogusBuilder(bogusBlock);
    
    // Add some bogus computations
    llvm::Value* val1 = bogusBuilder.getInt32(rng.getUInt32(1, 100));
    llvm::Value* val2 = bogusBuilder.getInt32(rng.getUInt32(1, 100));
    llvm::Value* bogusComp = bogusBuilder.CreateMul(val1, val2);
    bogusBuilder.CreateBr(afterBlock);
    
    // CRITICAL: Update PHI nodes in afterBlock to account for bogusBlock
    std::vector<llvm::PHINode*> phis;
    for (llvm::PHINode& phi : afterBlock->phis()) {
        phis.push_back(&phi);
    }
    
    for (llvm::PHINode* phi : phis) {
        // Get the incoming value that came from original
        llvm::Value* incomingVal = phi->getIncomingValueForBlock(original);
        if (incomingVal) {
            // Add the same value as incoming from bogusBlock (even though it won't execute)
            phi->addIncoming(incomingVal, bogusBlock);
        }
    }
    
    // Replace unconditional branch with opaque predicate
    original->getTerminator()->eraseFromParent();
    llvm::IRBuilder<> builder(original);
    
    // Create opaque predicate (7 * 7 == 49, always true)
    llvm::Value* seven = builder.getInt32(7);
    llvm::Value* squared = builder.CreateMul(seven, seven);
    llvm::Value* fortynine = builder.getInt32(49);
    llvm::Value* cond = builder.CreateICmpEQ(squared, fortynine);
    
    builder.CreateCondBr(cond, afterBlock, bogusBlock);
}

} // namespace obfuscator
