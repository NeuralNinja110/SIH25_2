/**
 * @file ObfuscationPass.h
 * @brief Base class for all obfuscation transformation passes
 * @version 1.0.0
 * @date 2025-10-09
 */

#ifndef OBFUSCATION_PASS_H
#define OBFUSCATION_PASS_H

#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include <string>
#include <cstdint>

namespace obfuscator {

class MetricsCollector;

/**
 * @class ObfuscationPass
 * @brief Abstract base class for obfuscation transformations
 */
class ObfuscationPass {
public:
    /**
     * @brief Construct a new Obfuscation Pass
     * @param name Pass name for identification
     * @param enabled Whether pass is enabled
     */
    explicit ObfuscationPass(const std::string& name, bool enabled = true);
    
    /**
     * @brief Virtual destructor
     */
    virtual ~ObfuscationPass() = default;

    /**
     * @brief Run the obfuscation pass on a module
     * @param module LLVM module to transform
     * @param metrics Metrics collector for reporting
     * @return true if transformations were made
     */
    virtual bool runOnModule(llvm::Module& module, MetricsCollector& metrics) = 0;

    /**
     * @brief Get pass name
     * @return Pass name string
     */
    const std::string& getName() const { return name_; }

    /**
     * @brief Check if pass is enabled
     * @return true if enabled
     */
    bool isEnabled() const { return enabled_; }

    /**
     * @brief Enable or disable the pass
     * @param enabled New enabled state
     */
    void setEnabled(bool enabled) { enabled_ = enabled; }

    /**
     * @brief Set random seed for reproducible transformations
     * @param seed Random seed value
     */
    void setSeed(uint32_t seed) { seed_ = seed; }

protected:
    std::string name_;
    bool enabled_;
    uint32_t seed_;

    /**
     * @brief Check if a function should be obfuscated
     * @param func Function to check
     * @return true if function should be transformed
     */
    bool shouldObfuscateFunction(llvm::Function& func) const;
};

} // namespace obfuscator

#endif // OBFUSCATION_PASS_H
