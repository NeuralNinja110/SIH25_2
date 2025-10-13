/**
 * @file GrammarMetamorphic.h
 * @brief Grammar-based metamorphic code generation
 * @version 2.0.0
 * @date 2025-10-13
 */

#ifndef GRAMMAR_METAMORPHIC_H
#define GRAMMAR_METAMORPHIC_H

#include "ObfuscationPass.h"
#include "llvm/IR/IRBuilder.h"
#include <random>
#include <vector>

namespace obfuscator {

/**
 * @brief Metamorphic code generation using formal grammar transformations
 * 
 * Applies grammar-based transformations to generate semantically equivalent
 * but structurally different code variants, making pattern recognition
 * extremely difficult.
 */
class GrammarMetamorphic : public ObfuscationPass {
public:
    GrammarMetamorphic(uint32_t transformationRate = 50);
    bool runOnModule(llvm::Module& module, MetricsCollector& metrics) override;

private:
    uint32_t transformationRate_;  ///< Percentage of instructions to transform (0-100)
    
    /**
     * @brief Apply grammar transformations to function
     */
    uint32_t transformFunction(llvm::Function& func);
    
    /**
     * @brief Grammar rule: a = b + c → a = b - (-c)
     */
    bool applyAdditionToSubtraction(llvm::BinaryOperator* add);
    
    /**
     * @brief Grammar rule: a = b * c → a = b << log2(c) [if c is power of 2]
     */
    bool applyMultiplicationToShift(llvm::BinaryOperator* mul);
    
    /**
     * @brief Grammar rule: a = b / c → a = b * (1/c) [strength reduction]
     */
    bool applyDivisionToMultiply(llvm::BinaryOperator* div);
    
    /**
     * @brief Grammar rule: if (a) then B else C → if (!a) then C else B
     */
    bool applyBranchInversion(llvm::BranchInst* branch);
    
    /**
     * @brief Grammar rule: x = (a op1 b) op2 c → x = a op1 (b op2 c) [associativity]
     */
    bool applyAssociativityTransform(llvm::BinaryOperator* binOp);
    
    /**
     * @brief Check if value is a power of 2 constant
     */
    bool isPowerOfTwo(llvm::Value* val, uint32_t& log2Val);
};

} // namespace obfuscator

#endif // GRAMMAR_METAMORPHIC_H
