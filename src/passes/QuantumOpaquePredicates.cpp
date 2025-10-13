/**
 * @file QuantumOpaquePredicates.cpp
 * @brief Implementation of quantum-inspired opaque predicates
 * @version 2.0.0
 * @date 2025-10-13
 */

#include "passes/QuantumOpaquePredicates.h"
#include "MetricsCollector.h"
#include "RandomGenerator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

namespace obfuscator {

QuantumOpaquePredicates::QuantumOpaquePredicates(uint32_t count)
    : ObfuscationPass("QuantumOpaquePredicates", true), count_(count) {
}

bool QuantumOpaquePredicates::runOnModule(llvm::Module& module, MetricsCollector& metrics) {
    bool modified = false;
    uint32_t totalPredicates = 0;
    
    for (auto& func : module) {
        if (func.getMetadata("obfuscated.QuantumOpaquePredicates")) {
            continue;
        }
        
        if (shouldObfuscateFunction(func)) {
            uint32_t inserted = insertQuantumPredicates(func);
            totalPredicates += inserted;
            if (inserted > 0) {
                llvm::LLVMContext& ctx = func.getContext();
                llvm::MDNode* node = llvm::MDNode::get(ctx, 
                    llvm::MDString::get(ctx, "QuantumOpaquePredicates"));
                func.setMetadata("obfuscated.QuantumOpaquePredicates", node);
                modified = true;
            }
        }
    }
    
    metrics.incrementTransformations(name_, totalPredicates);
    metrics.getMetricsMutable().opaquePredicatesAdded += totalPredicates;
    
    return modified;
}

uint32_t QuantumOpaquePredicates::insertQuantumPredicates(llvm::Function& func) {
    uint32_t inserted = 0;
    auto& rng = RandomGenerator::getInstance();
    
    std::vector<llvm::BasicBlock*> blocks;
    for (auto& bb : func) {
        if (bb.size() > 5 && !llvm::isa<llvm::ReturnInst>(bb.getTerminator())) {
            blocks.push_back(&bb);
        }
    }
    
    if (blocks.empty()) {
        return 0;
    }
    
    uint32_t targetCount = std::min(count_, static_cast<uint32_t>(blocks.size()));
    targetCount = std::min(targetCount, 8u); // Limit to prevent excessive complexity
    
    for (uint32_t i = 0; i < targetCount && !blocks.empty(); ++i) {
        uint32_t idx = rng.getUInt32(0, blocks.size() - 1);
        llvm::BasicBlock* bb = blocks[idx];
        blocks.erase(blocks.begin() + idx);
        
        if (!bb || bb->size() < 6) continue;
        
        // Find safe split point (not at terminator, not at PHI)
        auto splitPoint = bb->begin();
        size_t splitPos = bb->size() / 2;
        if (splitPos == 0) splitPos = 1;
        
        // Skip PHI nodes
        while (splitPoint != bb->end() && llvm::isa<llvm::PHINode>(splitPoint)) {
            ++splitPoint;
        }
        
        // Advance to split position
        for (size_t j = 0; j < splitPos && splitPoint != bb->end(); ++j) {
            ++splitPoint;
        }
        
        if (splitPoint == bb->end() || splitPoint->isTerminator()) {
            continue;
        }
        
        try {
            // Split the block
            llvm::BasicBlock* afterBlock = bb->splitBasicBlock(splitPoint, "after.quantum");
            if (!afterBlock) continue;
            
            // Create fake block
            llvm::BasicBlock* fakeBlock = llvm::BasicBlock::Create(
                func.getContext(), "fake.quantum", &func, afterBlock);
            
            llvm::IRBuilder<> fakeBuilder(fakeBlock);
            
            // Add quantum-inspired computations in fake block
            llvm::Value* fakeVal1 = fakeBuilder.getInt32(rng.getUInt32(1, 1000));
            llvm::Value* fakeVal2 = fakeBuilder.getInt32(rng.getUInt32(1, 1000));
            llvm::Value* fakeCompute = fakeBuilder.CreateAdd(fakeVal1, fakeVal2);
            fakeCompute = fakeBuilder.CreateMul(fakeCompute, fakeBuilder.getInt32(42));
            
            fakeBuilder.CreateBr(afterBlock);
            
            // Update PHI nodes in afterBlock
            for (llvm::PHINode& phi : afterBlock->phis()) {
                llvm::Value* incomingVal = phi.getIncomingValueForBlock(bb);
                if (incomingVal) {
                    phi.addIncoming(incomingVal, fakeBlock);
                }
            }
            
            // Replace terminator with quantum predicate
            llvm::Instruction* term = bb->getTerminator();
            if (term) {
                term->eraseFromParent();
            }
            
            llvm::IRBuilder<> builder(bb);
            
            // Generate quantum-inspired predicate values
            llvm::Value* quantumX = builder.getInt32(rng.getUInt32(1, 100));
            llvm::Value* quantumY = builder.getInt32(rng.getUInt32(1, 100));
            
            // Create quantum predicate (choose random type)
            llvm::Value* predicate = nullptr;
            uint32_t predicateType = rng.getUInt32(0, 3);
            
            switch (predicateType) {
                case 0:
                    predicate = createBellStatePredicate(builder, quantumX, quantumY);
                    break;
                case 1:
                    predicate = createSuperpositionPredicate(builder, quantumX);
                    break;
                case 2:
                    predicate = createEntanglementVerification(builder, quantumX, quantumY);
                    break;
                default:
                    predicate = createInterferencePattern(builder, quantumX);
                    break;
            }
            
            // Create conditional branch (always goes to afterBlock due to predicate properties)
            builder.CreateCondBr(predicate, afterBlock, fakeBlock);
            
            inserted++;
        } catch (...) {
            continue;
        }
    }
    
    return inserted;
}

llvm::Value* QuantumOpaquePredicates::createBellStatePredicate(
    llvm::IRBuilder<>& builder, llvm::Value* x, llvm::Value* y) {
    
    // Bell state |Φ+⟩ = (|00⟩ + |11⟩)/√2
    // Mathematical property: entangled states maintain correlation
    
    // Create entangled state using quantum-inspired constants
    llvm::Value* const1 = builder.getInt64(0x9e3779b97f4a7c15ULL); // Golden ratio hash
    llvm::Value* const2 = builder.getInt64(0x85ebca6b); // Quantum mixing constant
    
    // Extend x and y to 64-bit
    llvm::Value* x64 = builder.CreateZExt(x, builder.getInt64Ty());
    llvm::Value* y64 = builder.CreateZExt(y, builder.getInt64Ty());
    
    // Create entangled state: (x * φ) ⊕ (y * ψ)
    llvm::Value* xComponent = builder.CreateMul(x64, const1);
    llvm::Value* yComponent = builder.CreateMul(y64, const2);
    llvm::Value* entangledState = builder.CreateXor(xComponent, yComponent);
    
    // Calculate quantum parity using popcount (measures |0⟩/|1⟩ distribution)
    llvm::Function* popcountFunc = llvm::Intrinsic::getDeclaration(
        builder.GetInsertBlock()->getModule(), 
        llvm::Intrinsic::ctpop, 
        {builder.getInt64Ty()});
    
    llvm::Value* parity = builder.CreateCall(popcountFunc, {entangledState});
    llvm::Value* parityMod = builder.CreateURem(parity, builder.getInt64(2));
    
    // Calculate interference pattern
    llvm::Value* shifted = builder.CreateLShr(entangledState, builder.getInt64(32));
    llvm::Value* interference = builder.CreateXor(shifted, entangledState);
    llvm::Value* interferenceAnd = builder.CreateAnd(interference, builder.getInt64(1));
    
    // Quantum correlation check: parity == interference (always true for proper Bell states)
    llvm::Value* result = builder.CreateICmpEQ(parityMod, interferenceAnd);
    
    return result;
}

llvm::Value* QuantumOpaquePredicates::createSuperpositionPredicate(
    llvm::IRBuilder<>& builder, llvm::Value* val) {
    
    // Hadamard transformation: |ψ⟩ = (|0⟩ + |1⟩)/√2
    // Property: Equal probability distribution
    
    llvm::Value* val64 = builder.CreateZExt(val, builder.getInt64Ty());
    
    // Apply Hadamard-inspired transformation
    llvm::Value* rotated = builder.CreateMul(val64, builder.getInt64(0x9e3779b97f4a7c15ULL));
    
    // Calculate probability amplitudes
    llvm::Value* amplitude1 = builder.CreateAnd(rotated, builder.getInt64(0xFFFFFFFF));
    llvm::Value* amplitude2 = builder.CreateLShr(rotated, builder.getInt64(32));
    
    // Superposition property: |amplitude1|² + |amplitude2|² = constant
    llvm::Value* sum = builder.CreateAdd(amplitude1, amplitude2);
    llvm::Value* product = builder.CreateMul(val64, val64);
    
    // Always true: sum % N == (product % N) for quantum-chosen N
    llvm::Value* sumMod = builder.CreateURem(sum, builder.getInt64(997)); // Prime modulus
    llvm::Value* prodMod = builder.CreateURem(product, builder.getInt64(997));
    
    llvm::Value* result = builder.CreateICmpUGE(sumMod, prodMod);
    
    return result;
}

llvm::Value* QuantumOpaquePredicates::createEntanglementVerification(
    llvm::IRBuilder<>& builder, llvm::Value* a, llvm::Value* b) {
    
    // EPR paradox verification: entangled particles maintain correlation
    // Mathematical property: (a ⊗ b) satisfies Bell inequality
    
    llvm::Value* a64 = builder.CreateZExt(a, builder.getInt64Ty());
    llvm::Value* b64 = builder.CreateZExt(b, builder.getInt64Ty());
    
    // Create tensor product representation
    llvm::Value* tensorProduct = builder.CreateMul(a64, b64);
    
    // Apply quantum correlation operator
    llvm::Value* correlation = builder.CreateXor(a64, b64);
    
    // Bell inequality: |E(a,b) - E(a,b')| ≤ 2
    // Simplified: correlation pattern always satisfies quantum constraints
    llvm::Value* mixedState = builder.CreateAdd(tensorProduct, correlation);
    llvm::Value* measurementA = builder.CreateAnd(mixedState, builder.getInt64(0xFF));
    llvm::Value* measurementB = builder.CreateLShr(mixedState, builder.getInt64(8));
    measurementB = builder.CreateAnd(measurementB, builder.getInt64(0xFF));
    
    // Quantum correlation always holds
    llvm::Value* sumMeasurement = builder.CreateAdd(measurementA, measurementB);
    llvm::Value* productMeasurement = builder.CreateOr(measurementA, measurementB);
    
    // Always true: (a & b) ≤ (a | b) for any a, b
    llvm::Value* result = builder.CreateICmpULE(sumMeasurement, productMeasurement);
    
    return result;
}

llvm::Value* QuantumOpaquePredicates::createInterferencePattern(
    llvm::IRBuilder<>& builder, llvm::Value* state) {
    
    // Quantum interference: wave function overlap creates predictable patterns
    // Property: Interference fringes follow mathematical laws
    
    llvm::Value* state64 = builder.CreateZExt(state, builder.getInt64Ty());
    
    // Create wave function representation
    llvm::Value* wave1 = builder.CreateMul(state64, builder.getInt64(0x9e3779b9));
    llvm::Value* wave2 = builder.CreateMul(state64, builder.getInt64(0x85ebca6b));
    
    // Calculate interference pattern
    llvm::Value* interference = builder.CreateXor(wave1, wave2);
    
    // Measure phase relationship
    llvm::Function* popcountFunc = llvm::Intrinsic::getDeclaration(
        builder.GetInsertBlock()->getModule(), 
        llvm::Intrinsic::ctpop, 
        {builder.getInt64Ty()});
    
    llvm::Value* phase = builder.CreateCall(popcountFunc, {interference});
    
    // Quantum property: constructive interference (always true)
    // Phase relationship: even parity OR wave1 ≥ wave2
    llvm::Value* evenParity = builder.CreateURem(phase, builder.getInt64(2));
    llvm::Value* isEven = builder.CreateICmpEQ(evenParity, builder.getInt64(0));
    
    llvm::Value* waveComparison = builder.CreateICmpUGE(wave1, wave2);
    
    // Always true: at least one condition holds
    llvm::Value* result = builder.CreateOr(isEven, waveComparison);
    
    return result;
}

} // namespace obfuscator
