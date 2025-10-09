/**
 * @file OpaquePredicates.cpp
 * @brief Implementation of opaque predicate insertion
 * @version 1.0.0
 * @date 2025-10-09
 */

#include "passes/OpaquePredicates.h"
#include "MetricsCollector.h"
#include "RandomGenerator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

namespace obfuscator {

OpaquePredicates::OpaquePredicates(uint32_t count)
    : ObfuscationPass("OpaquePredicates", true), count_(count) {
}

bool OpaquePredicates::runOnModule(llvm::Module& module, MetricsCollector& metrics) {
    bool modified = false;
    uint32_t totalPredicates = 0;
    
    for (auto& func : module) {
        // Skip if already obfuscated by this pass
        if (func.getMetadata("obfuscated.OpaquePredicates")) {
            continue;
        }
        
        if (shouldObfuscateFunction(func)) {
            uint32_t inserted = insertPredicates(func);
            totalPredicates += inserted;
            if (inserted > 0) {
                // Mark function as obfuscated by this pass
                llvm::LLVMContext& ctx = func.getContext();
                llvm::MDNode* node = llvm::MDNode::get(ctx, 
                    llvm::MDString::get(ctx, "OpaquePredicates"));
                func.setMetadata("obfuscated.OpaquePredicates", node);
                modified = true;
            }
        }
    }
    
    metrics.incrementTransformations(name_, totalPredicates);
    metrics.getMetricsMutable().opaquePredicatesAdded += totalPredicates;
    
    return modified;
}

uint32_t OpaquePredicates::insertPredicates(llvm::Function& func) {
    uint32_t inserted = 0;
    auto& rng = RandomGenerator::getInstance();
    
    // Collect eligible basic blocks
    std::vector<llvm::BasicBlock*> blocks;
    for (auto& bb : func) {
        // More restrictive: need at least 3 instructions and not a return block
        if (bb.size() > 3 && !llvm::isa<llvm::ReturnInst>(bb.getTerminator())) {
            blocks.push_back(&bb);
        }
    }
    
    if (blocks.empty()) {
        return 0;
    }
    
    // Insert predicates
    uint32_t targetCount = std::min(count_, static_cast<uint32_t>(blocks.size()));
    // Limit to max 5 predicates to avoid issues
    targetCount = std::min(targetCount, 5u);
    
    for (uint32_t i = 0; i < targetCount; ++i) {
        if (blocks.empty()) break;
        
        uint32_t idx = rng.getUInt32(0, blocks.size() - 1);
        llvm::BasicBlock* bb = blocks[idx];
        
        // Skip if block is too small after removing from list
        if (!bb || bb->size() < 4) {
            blocks.erase(blocks.begin() + idx);
            continue;
        }
        
        // Split block in the middle
        auto splitPoint = bb->begin();
        size_t splitPos = bb->size() / 2;
        if (splitPos == 0) splitPos = 1;
        std::advance(splitPoint, splitPos);
        
        if (splitPoint == bb->end() || splitPoint->isTerminator()) {
            blocks.erase(blocks.begin() + idx);
            continue;
        }
        
        try {
            llvm::BasicBlock* afterBlock = bb->splitBasicBlock(splitPoint, "after.opaque");
            if (!afterBlock) {
                blocks.erase(blocks.begin() + idx);
                continue;
            }
            
            // Create fake block that will never execute
            llvm::BasicBlock* fakeBlock = llvm::BasicBlock::Create(
                func.getContext(), "fake.opaque", &func, afterBlock);
            
            // Add some fake instructions
            llvm::IRBuilder<> fakeBuilder(fakeBlock);
            llvm::Value* fakeVal = fakeBuilder.CreateAdd(
                fakeBuilder.getInt32(42), fakeBuilder.getInt32(58));
            fakeBuilder.CreateBr(afterBlock);
            
            // CRITICAL: Update PHI nodes in afterBlock to account for the new fakeBlock predecessor
            // We need to add incoming values for fakeBlock before changing bb's terminator
            std::vector<llvm::PHINode*> phis;
            for (llvm::PHINode& phi : afterBlock->phis()) {
                phis.push_back(&phi);
            }
            
            for (llvm::PHINode* phi : phis) {
                if (!phi) continue;
                // Get the incoming value that came from bb (which is now the same for both branches)
                llvm::Value* incomingVal = phi->getIncomingValueForBlock(bb);
                if (incomingVal) {
                    // Add the same value as incoming from fakeBlock
                    phi->addIncoming(incomingVal, fakeBlock);
                }
            }
            
            // Replace unconditional branch with conditional using opaque predicate
            llvm::Instruction* term = bb->getTerminator();
            if (term) {
                term->eraseFromParent();
            }
            
            llvm::IRBuilder<> builder(bb);
            llvm::Value* predicate = createOpaquePredicate(builder);
            builder.CreateCondBr(predicate, afterBlock, fakeBlock);
            
            inserted++;
            blocks.erase(blocks.begin() + idx);
        } catch (...) {
            // Skip this block if any error occurs
            blocks.erase(blocks.begin() + idx);
            continue;
        }
    }
    
    return inserted;
}

llvm::Value* OpaquePredicates::createOpaquePredicate(llvm::IRBuilder<>& builder) {
    auto& rng = RandomGenerator::getInstance();
    uint32_t choice = rng.getUInt32(0, 3);
    
    // Create various opaque predicates that always evaluate to true
    switch (choice) {
        case 0: {
            // (x * x) >= 0 for all x
            llvm::Value* x = builder.getInt32(rng.getUInt32(1, 100));
            llvm::Value* squared = builder.CreateMul(x, x);
            return builder.CreateICmpSGE(squared, builder.getInt32(0));
        }
        case 1: {
            // (x | y) >= x for all x, y
            llvm::Value* x = builder.getInt32(rng.getUInt32(1, 100));
            llvm::Value* y = builder.getInt32(rng.getUInt32(1, 100));
            llvm::Value* orVal = builder.CreateOr(x, y);
            return builder.CreateICmpSGE(orVal, x);
        }
        case 2: {
            // (x & y) <= x for all x, y
            llvm::Value* x = builder.getInt32(rng.getUInt32(1, 100));
            llvm::Value* y = builder.getInt32(rng.getUInt32(1, 100));
            llvm::Value* andVal = builder.CreateAnd(x, y);
            return builder.CreateICmpSLE(andVal, x);
        }
        default: {
            // 2 * x == x + x
            llvm::Value* x = builder.getInt32(rng.getUInt32(1, 100));
            llvm::Value* doubled = builder.CreateMul(x, builder.getInt32(2));
            llvm::Value* added = builder.CreateAdd(x, x);
            return builder.CreateICmpEQ(doubled, added);
        }
    }
}

} // namespace obfuscator
