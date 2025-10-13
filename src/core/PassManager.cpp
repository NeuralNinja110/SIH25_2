/**
 * @file PassManager.cpp
 * @brief Implementation of PassManager with advanced quantum-enhanced passes
 * @version 2.0.0
 * @date 2025-10-13
 */

#include "PassManager.h"
#include "passes/MBAObfuscation.h"
#include "passes/QuantumOpaquePredicates.h"
#include "passes/HardwareCacheObfuscation.h"
#include "passes/GrammarMetamorphic.h"
#include "passes/ControlFlowFlattening.h"
#include "passes/DeadCodeInjection.h"
#include "passes/StringEncryption.h"
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
    Logger::getInstance().info("Initializing advanced quantum-enhanced obfuscation passes (v2.0)");
    
    // LAYER 1: MBA Expression Substitution (defeats SMT solvers)
    if (config_.enableInstructionSubstitution) {
        auto pass = std::make_unique<MBAObfuscation>(
            config_.substitutionProbability);
        pass->setSeed(config_.seed);
        addPass(std::move(pass));
    }
    
    // LAYER 2: String Encryption with Runtime Decryption
    if (config_.enableStringEncryption) {
        auto pass = std::make_unique<StringEncryption>(
            config_.stringEncryptionAlgorithm);
        pass->setSeed(config_.seed);
        addPass(std::move(pass));
    }
    
    // LAYER 3: Constant Obfuscation
    if (config_.enableConstantObfuscation) {
        auto pass = std::make_unique<ConstantObfuscation>(
            config_.constantObfuscationComplexity);
        pass->setSeed(config_.seed);
        addPass(std::move(pass));
    }
    
    // LAYER 4: Quantum-Inspired Opaque Predicates (exponential complexity)
    if (config_.enableOpaquePredicates) {
        auto pass = std::make_unique<QuantumOpaquePredicates>(
            config_.opaquePredicateCount);
        pass->setSeed(config_.seed);
        addPass(std::move(pass));
    }
    
    // LAYER 5: Dead Code Injection
    if (config_.enableDeadCodeInjection) {
        auto pass = std::make_unique<DeadCodeInjection>(
            config_.deadCodeRatio);
        pass->setSeed(config_.seed);
        addPass(std::move(pass));
    }
    
    // LAYER 6: Grammar-Based Metamorphic Code (structural unpredictability)
    if (config_.enableInstructionSubstitution) {  // Reuse same config flag
        auto pass = std::make_unique<GrammarMetamorphic>(
            config_.substitutionProbability / 2);  // Lower rate for grammar transforms
        pass->setSeed(config_.seed);
        addPass(std::move(pass));
    }
    
    // LAYER 7: Quantum Control Flow Flattening
    if (config_.enableControlFlowFlattening) {
        auto pass = std::make_unique<ControlFlowFlattening>(
            config_.flatteningComplexity);
        pass->setSeed(config_.seed);
        addPass(std::move(pass));
    }
    
    // LAYER 8: Hardware Cache-Based Obfuscation (hardware-locked)
    if (config_.enableHardwareCacheObfuscation) {
        auto pass = std::make_unique<HardwareCacheObfuscation>(
            config_.cacheObfuscationIntensity);
        pass->setSeed(config_.seed);
        addPass(std::move(pass));
    }
    
    // LAYER 9: Call Graph Obfuscation
    if (config_.enableCallGraphObfuscation) {
        auto pass = std::make_unique<CallGraphObfuscation>();
        pass->setSeed(config_.seed);
        addPass(std::move(pass));
    }
    
    // LAYER 10: Anti-Debugging Protections
    if (config_.enableAntiDebug) {
        auto pass = std::make_unique<AntiDebug>();
        pass->setSeed(config_.seed);
        addPass(std::move(pass));
    }
    
    Logger::getInstance().info("Initialized " + std::to_string(passes_.size()) + 
                               " quantum-enhanced obfuscation passes");
}

} // namespace obfuscator
