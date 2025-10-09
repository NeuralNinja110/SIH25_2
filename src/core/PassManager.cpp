/**
 * @file PassManager.cpp
 * @brief Implementation of PassManager
 * @version 1.0.0
 * @date 2025-10-09
 */

#include "PassManager.h"
#include "passes/ControlFlowFlattening.h"
#include "passes/OpaquePredicates.h"
#include "passes/InstructionSubstitution.h"
#include "passes/BogusControlFlow.h"
#include "passes/DeadCodeInjection.h"
#include "passes/StringEncryption.h"
#include "passes/FunctionVirtualization.h"
#include "passes/CallGraphObfuscation.h"
#include "passes/ConstantObfuscation.h"
#include "passes/AntiDebug.h"
#include "Logger.h"
#include "RandomGenerator.h"

namespace obfuscator {

PassManager::PassManager(const ObfuscationConfig& config)
    : config_(config) {
    initializePasses();
}

PassManager::~PassManager() {
}

void PassManager::addPass(std::unique_ptr<ObfuscationPass> pass) {
    passes_.push_back(std::move(pass));
}

bool PassManager::runPasses(llvm::Module& module, MetricsCollector& metrics) {
    bool modified = false;
    
    // Set random seed
    RandomGenerator::getInstance().seed(config_.seed);
    
    for (auto& pass : passes_) {
        if (!pass->isEnabled()) {
            continue;
        }
        
        Logger::getInstance().info("Running pass: " + pass->getName());
        
        auto startTime = std::chrono::high_resolution_clock::now();
        bool passModified = pass->runOnModule(module, metrics);
        auto endTime = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            endTime - startTime);
        metrics.recordTiming(pass->getName(), duration);
        
        if (passModified) {
            Logger::getInstance().info("Pass " + pass->getName() + " made transformations");
            modified = true;
        } else {
            Logger::getInstance().info("Pass " + pass->getName() + " made no changes");
        }
    }
    
    return modified;
}

void PassManager::clearPasses() {
    passes_.clear();
}

void PassManager::initializePasses() {
    Logger::getInstance().info("Initializing obfuscation passes");
    
    // Add passes based on configuration
    if (config_.enableInstructionSubstitution) {
        auto pass = std::make_unique<InstructionSubstitution>(
            config_.substitutionProbability);
        pass->setSeed(config_.seed);
        addPass(std::move(pass));
    }
    
    if (config_.enableStringEncryption) {
        auto pass = std::make_unique<StringEncryption>(
            config_.stringEncryptionAlgorithm);
        pass->setSeed(config_.seed);
        addPass(std::move(pass));
    }
    
    if (config_.enableConstantObfuscation) {
        auto pass = std::make_unique<ConstantObfuscation>(
            config_.constantObfuscationComplexity);
        pass->setSeed(config_.seed);
        addPass(std::move(pass));
    }
    
    if (config_.enableOpaquePredicates) {
        auto pass = std::make_unique<OpaquePredicates>(
            config_.opaquePredicateCount);
        pass->setSeed(config_.seed);
        addPass(std::move(pass));
    }
    
    if (config_.enableBogusControlFlow) {
        auto pass = std::make_unique<BogusControlFlow>(
            config_.bogusBlockProbability);
        pass->setSeed(config_.seed);
        addPass(std::move(pass));
    }
    
    if (config_.enableDeadCodeInjection) {
        auto pass = std::make_unique<DeadCodeInjection>(
            config_.deadCodeRatio);
        pass->setSeed(config_.seed);
        addPass(std::move(pass));
    }
    
    if (config_.enableControlFlowFlattening) {
        auto pass = std::make_unique<ControlFlowFlattening>(
            config_.flatteningComplexity);
        pass->setSeed(config_.seed);
        addPass(std::move(pass));
    }
    
    if (config_.enableFunctionVirtualization) {
        auto pass = std::make_unique<FunctionVirtualization>(
            config_.virtualizationThreshold);
        pass->setSeed(config_.seed);
        addPass(std::move(pass));
    }
    
    if (config_.enableCallGraphObfuscation) {
        auto pass = std::make_unique<CallGraphObfuscation>();
        pass->setSeed(config_.seed);
        addPass(std::move(pass));
    }
    
    if (config_.enableAntiDebug) {
        auto pass = std::make_unique<AntiDebug>();
        pass->setSeed(config_.seed);
        addPass(std::move(pass));
    }
    
    Logger::getInstance().info("Initialized " + std::to_string(passes_.size()) + " passes");
}

} // namespace obfuscator
