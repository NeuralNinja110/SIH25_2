/**
 * @file AntiDebug.h
 * @brief Anti-debugging obfuscation pass
 * @version 1.0.0
 * @date 2025-10-09
 */

#ifndef ANTI_DEBUG_H
#define ANTI_DEBUG_H

#include "ObfuscationPass.h"

namespace obfuscator {

class AntiDebug : public ObfuscationPass {
public:
    AntiDebug();
    bool runOnModule(llvm::Module& module, MetricsCollector& metrics) override;

private:
    uint32_t insertAntiDebugChecks(llvm::Module& module);
    void createPtraceCheck(llvm::Function& func);
    void createTimingCheck(llvm::Function& func);
};

} // namespace obfuscator

#endif // ANTI_DEBUG_H
