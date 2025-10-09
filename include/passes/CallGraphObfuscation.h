/**
 * @file CallGraphObfuscation.h
 * @brief Call graph obfuscation pass
 * @version 1.0.0
 * @date 2025-10-09
 */

#ifndef CALL_GRAPH_OBFUSCATION_H
#define CALL_GRAPH_OBFUSCATION_H

#include "ObfuscationPass.h"

namespace obfuscator {

class CallGraphObfuscation : public ObfuscationPass {
public:
    CallGraphObfuscation();
    bool runOnModule(llvm::Module& module, MetricsCollector& metrics) override;

private:
    uint32_t obfuscateCalls(llvm::Module& module);
};

} // namespace obfuscator

#endif // CALL_GRAPH_OBFUSCATION_H
