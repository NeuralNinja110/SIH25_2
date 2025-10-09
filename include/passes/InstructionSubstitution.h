/**
 * @file InstructionSubstitution.h
 * @brief Instruction substitution obfuscation pass
 * @version 1.0.0
 * @date 2025-10-09
 */

#ifndef INSTRUCTION_SUBSTITUTION_H
#define INSTRUCTION_SUBSTITUTION_H

#include "ObfuscationPass.h"
#include "llvm/IR/Instructions.h"
#include <cstdint>

namespace obfuscator {

class InstructionSubstitution : public ObfuscationPass {
public:
    explicit InstructionSubstitution(uint32_t probability = 50);
    bool runOnModule(llvm::Module& module, MetricsCollector& metrics) override;

private:
    uint32_t probability_;
    uint32_t substituteInstructions(llvm::Function& func);
    llvm::Value* substituteAdd(llvm::BinaryOperator* inst);
    llvm::Value* substituteSub(llvm::BinaryOperator* inst);
    llvm::Value* substituteXor(llvm::BinaryOperator* inst);
};

} // namespace obfuscator

#endif // INSTRUCTION_SUBSTITUTION_H
