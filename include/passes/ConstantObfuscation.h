/**
 * @file ConstantObfuscation.h
 * @brief Constant obfuscation pass
 * @version 1.0.0
 * @date 2025-10-09
 */

#ifndef CONSTANT_OBFUSCATION_H
#define CONSTANT_OBFUSCATION_H

#include "ObfuscationPass.h"

namespace obfuscator {

class ConstantObfuscation : public ObfuscationPass {
public:
    explicit ConstantObfuscation(uint32_t complexity = 50);
    bool runOnModule(llvm::Module& module, MetricsCollector& metrics) override;

private:
    uint32_t complexity_;
    uint32_t obfuscateConstants(llvm::Function& func);
};

} // namespace obfuscator

#endif // CONSTANT_OBFUSCATION_H
