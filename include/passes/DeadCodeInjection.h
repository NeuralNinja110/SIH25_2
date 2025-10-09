/**
 * @file DeadCodeInjection.h
 * @brief Dead code injection obfuscation pass
 * @version 1.0.0
 * @date 2025-10-09
 */

#ifndef DEAD_CODE_INJECTION_H
#define DEAD_CODE_INJECTION_H

#include "ObfuscationPass.h"

namespace obfuscator {

class DeadCodeInjection : public ObfuscationPass {
public:
    explicit DeadCodeInjection(uint32_t ratio = 20);
    bool runOnModule(llvm::Module& module, MetricsCollector& metrics) override;

private:
    uint32_t ratio_;
    uint32_t injectDeadCode(llvm::Function& func);
};

} // namespace obfuscator

#endif // DEAD_CODE_INJECTION_H
