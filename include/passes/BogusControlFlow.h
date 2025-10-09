/**
 * @file BogusControlFlow.h
 * @brief Bogus control flow obfuscation pass
 * @version 1.0.0
 * @date 2025-10-09
 */

#ifndef BOGUS_CONTROL_FLOW_H
#define BOGUS_CONTROL_FLOW_H

#include "ObfuscationPass.h"
#include <cstdint>

namespace obfuscator {

class BogusControlFlow : public ObfuscationPass {
public:
    explicit BogusControlFlow(uint32_t probability = 30);
    bool runOnModule(llvm::Module& module, MetricsCollector& metrics) override;

private:
    uint32_t probability_;
    uint32_t addBogusBlocks(llvm::Function& func);
    void createBogusBlock(llvm::BasicBlock* original);
};

} // namespace obfuscator

#endif // BOGUS_CONTROL_FLOW_H
