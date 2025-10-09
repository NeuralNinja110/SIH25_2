/**
 * @file ControlFlowFlattening.h
 * @brief Control flow flattening obfuscation pass
 * @version 1.0.0
 * @date 2025-10-09
 * 
 * This pass transforms the control flow graph into a flattened structure
 * where all basic blocks are dispatched through a central switch statement.
 */

#ifndef CONTROL_FLOW_FLATTENING_H
#define CONTROL_FLOW_FLATTENING_H

#include "ObfuscationPass.h"
#include <cstdint>

namespace obfuscator {

/**
 * @class ControlFlowFlattening
 * @brief Flattens control flow to obscure program logic
 */
class ControlFlowFlattening : public ObfuscationPass {
public:
    /**
     * @brief Construct a new Control Flow Flattening pass
     * @param complexity Level of flattening complexity (0-100)
     */
    explicit ControlFlowFlattening(uint32_t complexity = 50);

    /**
     * @brief Run control flow flattening on module
     * @param module LLVM module to transform
     * @param metrics Metrics collector
     * @return true if transformations were made
     */
    bool runOnModule(llvm::Module& module, MetricsCollector& metrics) override;

private:
    uint32_t complexity_;

    /**
     * @brief Flatten control flow of a single function
     * @param func Function to flatten
     * @return true if function was flattened
     */
    bool flattenFunction(llvm::Function& func);

    /**
     * @brief Check if function is suitable for flattening
     * @param func Function to check
     * @return true if function can be flattened
     */
    bool canFlatten(llvm::Function& func) const;
};

} // namespace obfuscator

#endif // CONTROL_FLOW_FLATTENING_H
