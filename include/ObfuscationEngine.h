/**
 * @file ObfuscationEngine.h
 * @brief Core obfuscation engine for LLVM-based code obfuscation
 * @version 1.0.0
 * @date 2025-10-09
 * 
 * This file contains the main obfuscation engine that orchestrates
 * various obfuscation passes and transformations.
 */

#ifndef OBFUSCATION_ENGINE_H
#define OBFUSCATION_ENGINE_H

#include <string>
#include <memory>
#include <vector>

#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "ObfuscationConfig.h"
#include "ReportGenerator.h"
#include "PassManager.h"

namespace obfuscator {

/**
 * @class ObfuscationEngine
 * @brief Main engine for orchestrating obfuscation processes
 */
class ObfuscationEngine {
public:
    /**
     * @brief Construct a new Obfuscation Engine object
     * @param config Configuration parameters for obfuscation
     */
    explicit ObfuscationEngine(const ObfuscationConfig& config);
    
    /**
     * @brief Destroy the Obfuscation Engine object
     */
    ~ObfuscationEngine();

    /**
     * @brief Process a source file through obfuscation pipeline
     * @param inputFile Path to input source file
     * @param outputFile Path to output obfuscated file
     * @return true if obfuscation successful, false otherwise
     */
    bool processFile(const std::string& inputFile, const std::string& outputFile);

    /**
     * @brief Get the report generator for metrics collection
     * @return Shared pointer to report generator
     */
    std::shared_ptr<ReportGenerator> getReportGenerator() const;

private:
    /**
     * @brief Compile source file to LLVM IR
     * @param sourceFile Path to source file
     * @param irFile Path to output IR file
     * @return true if compilation successful
     */
    bool compileToIR(const std::string& sourceFile, const std::string& irFile);

    /**
     * @brief Load LLVM IR module from file
     * @param irFile Path to IR file
     * @return Unique pointer to loaded module
     */
    std::unique_ptr<llvm::Module> loadModule(const std::string& irFile);

    /**
     * @brief Apply obfuscation passes to module
     * @param module LLVM module to obfuscate
     * @return true if obfuscation successful
     */
    bool applyObfuscation(llvm::Module& module);

    /**
     * @brief Compile obfuscated IR to object file
     * @param module Obfuscated LLVM module
     * @param objectFile Path to output object file
     * @return true if compilation successful
     */
    bool compileToObject(llvm::Module& module, const std::string& objectFile);

    /**
     * @brief Link object file to final binary
     * @param objectFile Path to object file
     * @param binaryFile Path to output binary
     * @param inputFile Path to original input file (for detecting C++ files)
     * @return true if linking successful
     */
    bool linkToBinary(const std::string& objectFile, const std::string& binaryFile, const std::string& inputFile);

    ObfuscationConfig config_;
    std::unique_ptr<llvm::LLVMContext> context_;
    std::unique_ptr<PassManager> passManager_;
    std::shared_ptr<ReportGenerator> reportGenerator_;
};

} // namespace obfuscator

#endif // OBFUSCATION_ENGINE_H
