/**
 * @file MBAObfuscation.h
 * @brief Mixed Boolean-Arithmetic expression substitution to defeat SMT solvers
 * @version 2.0.0
 * @date 2025-10-13
 */

#ifndef MBA_OBFUSCATION_H
#define MBA_OBFUSCATION_H

#include "ObfuscationPass.h"
#include "llvm/IR/IRBuilder.h"
#include <random>

namespace obfuscator {

/**
 * @brief Mixed Boolean-Arithmetic (MBA) expression generator
 * 
 * Replaces simple arithmetic and logical operations with mathematically
 * equivalent but exponentially complex MBA expressions. Defeats SMT solvers
 * and symbolic execution engines.
 */
class MBAObfuscation : public ObfuscationPass {
public:
    explicit MBAObfuscation(uint32_t probability = 75);
    bool runOnModule(llvm::Module& module, MetricsCollector& metrics) override;

private:
    uint32_t probability_; // Percentage of operations to transform
    
    /**
     * @brief Transform arithmetic operations to MBA equivalents
     */
    uint32_t transformArithmeticOperations(llvm::Function& func);
    
    /**
     * @brief Generate MBA equivalent for addition: a + b
     * Identity: a + b = (a ^ b) + 2 * (a & b)
     */
    llvm::Value* generateMBAAdd(llvm::IRBuilder<>& builder,
                                llvm::Value* a, llvm::Value* b);
    
    /**
     * @brief Generate MBA equivalent for subtraction: a - b
     * Identity: a - b = (a ^ b) - 2 * (~a & b)
     */
    llvm::Value* generateMBASub(llvm::IRBuilder<>& builder,
                                llvm::Value* a, llvm::Value* b);
    
    /**
     * @brief Generate MBA equivalent for AND: a & b
     * Identity: a & b = (a + b - (a | b)) / 2
     */
    llvm::Value* generateMBAAnd(llvm::IRBuilder<>& builder,
                                llvm::Value* a, llvm::Value* b);
    
    /**
     * @brief Generate MBA equivalent for OR: a | b
     * Identity: a | b = (a + b + (a & b)) - (a & b)
     */
    llvm::Value* generateMBAOr(llvm::IRBuilder<>& builder,
                               llvm::Value* a, llvm::Value* b);
    
    /**
     * @brief Generate MBA equivalent for XOR: a ^ b
     * Identity: a ^ b = (a | b) - (a & b)
     */
    llvm::Value* generateMBAXor(llvm::IRBuilder<>& builder,
                                llvm::Value* a, llvm::Value* b);
    
    /**
     * @brief Add random noise cancellation
     * Returns: (expr + noise) - noise
     */
    llvm::Value* addNoiseCancellation(llvm::IRBuilder<>& builder,
                                      llvm::Value* expr);
};

} // namespace obfuscator

#endif // MBA_OBFUSCATION_H
