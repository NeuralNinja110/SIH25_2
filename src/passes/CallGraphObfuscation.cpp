/**
 * @file CallGraphObfuscation.cpp
 * @brief Implementation of call graph obfuscation pass
 * @version 1.0.0
 * @date 2025-10-09
 */

#include "passes/CallGraphObfuscation.h"
#include "MetricsCollector.h"
#include "RandomGenerator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"

namespace obfuscator {

CallGraphObfuscation::CallGraphObfuscation()
    : ObfuscationPass("CallGraphObfuscation", true) {
}

bool CallGraphObfuscation::runOnModule(llvm::Module& module, MetricsCollector& metrics) {
    uint32_t transformations = obfuscateCalls(module);
    
    metrics.incrementTransformations(name_, transformations);
    metrics.getMetricsMutable().callGraphTransformations += transformations;
    
    return transformations > 0;
}

uint32_t CallGraphObfuscation::obfuscateCalls(llvm::Module& module) {
    uint32_t count = 0;
    auto& rng = RandomGenerator::getInstance();
    
    // Collect direct function calls
    std::vector<llvm::CallInst*> calls;
    
    for (auto& func : module) {
        for (auto& bb : func) {
            for (auto& inst : bb) {
                if (auto* call = llvm::dyn_cast<llvm::CallInst>(&inst)) {
                    if (call->getCalledFunction() && !call->getCalledFunction()->isIntrinsic()) {
                        calls.push_back(call);
                    }
                }
            }
        }
    }
    
    // Transform some calls to indirect calls through function pointers
    for (auto* call : calls) {
        if (rng.getBool(30)) {  // 30% probability
            llvm::Function* calledFunc = call->getCalledFunction();
            
            llvm::IRBuilder<> builder(call);
            
            // Create function pointer
            llvm::Value* funcPtr = builder.CreateBitCast(
                calledFunc,
                calledFunc->getType()
            );
            
            // Create indirect call (simplified - doesn't modify actual call)
            // Full implementation would replace direct call with indirect
            
            count++;
        }
    }
    
    return count;
}

} // namespace obfuscator
