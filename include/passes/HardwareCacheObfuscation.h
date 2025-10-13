/**
 * @file HardwareCacheObfuscation.h
 * @brief CPU cache timing-based obfuscation for hardware-locked protection
 * @version 2.0.0
 * @date 2025-10-13
 */

#ifndef HARDWARE_CACHE_OBFUSCATION_H
#define HARDWARE_CACHE_OBFUSCATION_H

#include "ObfuscationPass.h"
#include "llvm/IR/IRBuilder.h"
#include <random>

namespace obfuscator {

/**
 * @brief Hardware-assisted obfuscation using CPU cache timing
 * 
 * Creates hardware-dependent code that uses cache line timing to generate
 * obfuscation keys. Requires physical hardware access to analyze, defeats
 * VM-based reverse engineering.
 */
class HardwareCacheObfuscation : public ObfuscationPass {
public:
    HardwareCacheObfuscation(uint32_t intensity = 50);
    bool runOnModule(llvm::Module& module, MetricsCollector& metrics) override;

private:
    uint32_t intensity_;  ///< Obfuscation intensity (0-100)
    
    /**
     * @brief Create cache-based key generation function
     */
    llvm::Function* createCacheKeyGenerator(llvm::Module& module);
    
    /**
     * @brief Create RDTSC timing measurement
     */
    llvm::Value* createRDTSC(llvm::IRBuilder<>& builder);
    
    /**
     * @brief Insert cache line access pattern
     */
    void insertCacheLineAccess(llvm::IRBuilder<>& builder, llvm::Value* cacheArray);
    
    /**
     * @brief Apply cache-based XOR to constants
     */
    uint32_t applyCacheBasedXOR(llvm::Module& module, llvm::Function* cacheKeyFunc);
};

} // namespace obfuscator

#endif // HARDWARE_CACHE_OBFUSCATION_H
