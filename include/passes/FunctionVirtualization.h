/**
 * @file FunctionVirtualization.h
 * @brief Function virtualization obfuscation pass
 * @version 1.0.0
 * @date 2025-10-09
 */

#ifndef FUNCTION_VIRTUALIZATION_H
#define FUNCTION_VIRTUALIZATION_H

#include "ObfuscationPass.h"

namespace obfuscator {

class FunctionVirtualization : public ObfuscationPass {
public:
    explicit FunctionVirtualization(uint32_t threshold = 50);
    bool runOnModule(llvm::Module& module, MetricsCollector& metrics) override;

private:
    uint32_t threshold_;
    bool virtualizeFunction(llvm::Function& func);
};

} // namespace obfuscator

#endif // FUNCTION_VIRTUALIZATION_H
