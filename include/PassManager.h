/**
 * @file PassManager.h
 * @brief Manager for orchestrating obfuscation passes
 * @version 1.0.0
 * @date 2025-10-09
 */

#ifndef PASS_MANAGER_H
#define PASS_MANAGER_H

#include <memory>
#include <vector>
#include "llvm/IR/Module.h"
#include "ObfuscationPass.h"
#include "ObfuscationConfig.h"
#include "MetricsCollector.h"

namespace obfuscator {

/**
 * @class PassManager
 * @brief Manages and orchestrates obfuscation passes
 */
class PassManager {
public:
    /**
     * @brief Construct a new Pass Manager
     * @param config Obfuscation configuration
     */
    explicit PassManager(const ObfuscationConfig& config);

    /**
     * @brief Destroy the Pass Manager
     */
    ~PassManager();

    /**
     * @brief Add an obfuscation pass to the pipeline
     * @param pass Unique pointer to obfuscation pass
     */
    void addPass(std::unique_ptr<ObfuscationPass> pass);

    /**
     * @brief Run all passes on a module
     * @param module LLVM module to transform
     * @param metrics Metrics collector for reporting
     * @return true if any transformations were made
     */
    bool runPasses(llvm::Module& module, MetricsCollector& metrics);

    /**
     * @brief Get number of registered passes
     * @return Number of passes
     */
    size_t getPassCount() const { return passes_.size(); }

    /**
     * @brief Clear all registered passes
     */
    void clearPasses();

private:
    /**
     * @brief Initialize passes based on configuration
     */
    void initializePasses();

    ObfuscationConfig config_;
    std::vector<std::unique_ptr<ObfuscationPass>> passes_;
};

} // namespace obfuscator

#endif // PASS_MANAGER_H
