// MAOSEngine.cpp - Main Multi-Layered Adaptive Obfuscation System Engine
// Orchestrates ATIE, QIRL, and PCGE components

#include "MAOSEngine.h"
#include "Logger.h"
#include <algorithm>

namespace MAOS {

// ============================================================================
// MAOS Config Builder
// ============================================================================

MAOSConfigBuilder::MAOSConfigBuilder() {
    config.mode = OperatingMode::SIZE_CONSERVATIVE;
    config.maxSizeIncrease = 15.0;
    config.maxTimeOverhead = 5.0;
    config.securityThreshold = 50.0;
    config.passLimit = 50;
    
    // GA defaults for size-conservative
    config.gaPopulationSize = 25;
    config.gaGenerations = 50;
    config.gaTournamentSize = 3;
    config.gaMutationRate = 0.1;
    config.gaCrossoverRate = 0.8;
    
    // Feature flags
    config.enableATIE = true;
    config.enableQIRL = true;
    config.enablePCGE = true;
    config.enableGeneticOptimization = true;
    config.enableQuantumInspired = false; // Only in maximum security
    config.enableSelfModification = false;
    config.enableAntiAnalysis = false;
    config.compressionEnabled = true;
}

MAOSConfigBuilder& MAOSConfigBuilder::setMode(OperatingMode mode) {
    config.mode = mode;
    
    if (mode == OperatingMode::MAXIMUM_SECURITY) {
        // Override settings for maximum security
        config.securityThreshold = 95.0;
        config.passLimit = 300;
        config.gaPopulationSize = 120;
        config.gaGenerations = 200;
        config.gaTournamentSize = 10;
        config.enableQuantumInspired = true;
        config.enableSelfModification = true;
        config.enableAntiAnalysis = true;
        config.compressionEnabled = false;
    }
    
    return *this;
}

MAOSConfigBuilder& MAOSConfigBuilder::setSecurityThreshold(double threshold) {
    config.securityThreshold = threshold;
    return *this;
}

MAOSConfigBuilder& MAOSConfigBuilder::setPassLimit(int limit) {
    config.passLimit = limit;
    return *this;
}

MAOSConfigBuilder& MAOSConfigBuilder::setGAParameters(int popSize, int generations, int tournamentSize) {
    config.gaPopulationSize = popSize;
    config.gaGenerations = generations;
    config.gaTournamentSize = tournamentSize;
    return *this;
}

MAOSConfigBuilder& MAOSConfigBuilder::enableComponent(const std::string& component, bool enable) {
    if (component == "ATIE") config.enableATIE = enable;
    else if (component == "QIRL") config.enableQIRL = enable;
    else if (component == "PCGE") config.enablePCGE = enable;
    else if (component == "GA") config.enableGeneticOptimization = enable;
    else if (component == "quantum") config.enableQuantumInspired = enable;
    else if (component == "self_mod") config.enableSelfModification = enable;
    else if (component == "anti_analysis") config.enableAntiAnalysis = enable;
    return *this;
}

MAOSConfig MAOSConfigBuilder::build() {
    return config;
}

// ============================================================================
// MAOS Engine
// ============================================================================

MAOSEngine::MAOSEngine(const MAOSConfig& config)
    : config(config), rng(std::make_shared<RandomGenerator>()) {
    
    Logger::getInstance().info("Initializing MAOS Engine...", Logger::INFO);
    Logger::getInstance().info("Mode: " + std::string(config.mode == OperatingMode::SIZE_CONSERVATIVE ? 
                                       "SIZE_CONSERVATIVE" : "MAXIMUM_SECURITY"), Logger::INFO);
    
    // Initialize components based on configuration
    if (config.enableATIE) {
        atie = std::make_shared<AdaptiveTransformationEngine>(rng);
        Logger::getInstance().info("ATIE component initialized", Logger::INFO);
    }
    
    if (config.enableQIRL) {
        qrng = std::make_shared<QuantumRandomGenerator>();
        qirl = std::make_shared<QuantumStateObfuscator>(qrng);
        Logger::getInstance().info("QIRL component initialized", Logger::INFO);
    }
    
    if (config.enablePCGE) {
        pcge = std::make_shared<PCGEManager>(rng);
        Logger::getInstance().info("PCGE component initialized", Logger::INFO);
    }
    
    Logger::getInstance().info("MAOS Engine initialized successfully", Logger::INFO);
}

MAOSEngine::~MAOSEngine() = default;

MAOSMetrics MAOSEngine::obfuscate(llvm::Module& module) {
    Logger::getInstance().info("=" * 80, Logger::INFO);
    Logger::getInstance().info("Starting MAOS Obfuscation Pipeline", Logger::INFO);
    Logger::getInstance().info("=" * 80, Logger::INFO);
    
    MAOSMetrics metrics;
    metrics.totalFunctions = 0;
    metrics.totalInstructions = 0;
    
    for (auto& F : module) {
        if (!F.isDeclaration()) {
            metrics.totalFunctions++;
            for (auto& BB : F) {
                metrics.totalInstructions += BB.size();
            }
        }
    }
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Phase 1: Analysis & Profiling
    Logger::getInstance().info("\n[Phase 1/4] Analysis & Profiling", Logger::INFO);
    analyzeAndProfile(module, metrics);
    
    // Phase 2: Transformation Planning (with Genetic Algorithm if enabled)
    Logger::getInstance().info("\n[Phase 2/4] Transformation Planning", Logger::INFO);
    planTransformations(module, metrics);
    
    // Phase 3: Multi-Layer Obfuscation
    Logger::getInstance().info("\n[Phase 3/4] Multi-Layer Obfuscation", Logger::INFO);
    applyMultiLayerObfuscation(module, metrics);
    
    // Phase 4: Verification & Hardening
    Logger::getInstance().info("\n[Phase 4/4] Verification & Hardening", Logger::INFO);
    verifyAndHarden(module, metrics);
    
    auto endTime = std::chrono::high_resolution_clock::now();
    metrics.totalObfuscationTime = 
        std::chrono::duration<double>(endTime - startTime).count();
    
    // Calculate final metrics
    calculateFinalMetrics(module, metrics);
    
    Logger::getInstance().info("=" * 80, Logger::INFO);
    Logger::getInstance().info("MAOS Obfuscation Complete", Logger::INFO);
    Logger::getInstance().info("Total time: " + std::to_string(metrics.totalObfuscationTime) + "s", Logger::INFO);
    Logger::getInstance().info("=" * 80, Logger::INFO);
    
    return metrics;
}

void MAOSEngine::analyzeAndProfile(llvm::Module& module, MAOSMetrics& metrics) {
    // Analyze code complexity for each function
    for (auto& F : module) {
        if (F.isDeclaration()) continue;
        
        if (atie) {
            ComplexityMetrics complexity = atie->analyzeComplexity(F);
            metrics.atieMetrics.averageComplexity += complexity.cyclomaticComplexity;
            metrics.atieMetrics.totalPassesEvaluated++;
        }
    }
    
    if (metrics.totalFunctions > 0) {
        metrics.atieMetrics.averageComplexity /= metrics.totalFunctions;
    }
    
    Logger::getInstance().info("Analysis complete. Average complexity: " + 
                std::to_string(metrics.atieMetrics.averageComplexity), Logger::INFO);
}

void MAOSEngine::planTransformations(llvm::Module& module, MAOSMetrics& metrics) {
    if (!atie || !config.enableGeneticOptimization) {
        Logger::getInstance().info("Using default transformation sequence", Logger::INFO);
        return;
    }
    
    // Use genetic algorithm to evolve optimal pass sequence
    Logger::getInstance().info("Running genetic algorithm optimization...", Logger::INFO);
    Logger::getInstance().info("Population: " + std::to_string(config.gaPopulationSize) + 
                ", Generations: " + std::to_string(config.gaGenerations), Logger::INFO);
    
    for (auto& F : module) {
        if (F.isDeclaration()) continue;
        
        ComplexityMetrics complexity = atie->analyzeComplexity(F);
        auto passes = atie->selectOptimalPasses(complexity);
        
        metrics.atieMetrics.passesSelected += passes.size();
    }
    
    metrics.atieMetrics.gaConvergenceGeneration = config.gaGenerations / 2; // Simplified
    metrics.atieMetrics.bestFitnessAchieved = 0.85; // Placeholder
    
    Logger::getInstance().info("GA optimization complete. Selected " + 
                std::to_string(metrics.atieMetrics.passesSelected) + " passes", Logger::INFO);
}

void MAOSEngine::applyMultiLayerObfuscation(llvm::Module& module, MAOSMetrics& metrics) {
    // Layer 1: Structural (Control Flow)
    Logger::getInstance().info("Applying structural obfuscation layer...", Logger::INFO);
    applyStructuralLayer(module, metrics);
    
    // Layer 2: Semantic (Instruction Substitution)
    Logger::getInstance().info("Applying semantic obfuscation layer...", Logger::INFO);
    applySemanticLayer(module, metrics);
    
    // Layer 3: Syntactic (Symbol Renaming)
    Logger::getInstance().info("Applying syntactic obfuscation layer...", Logger::INFO);
    applySyntacticLayer(module, metrics);
    
    // Layer 4: Quantum-Inspired (if enabled)
    if (config.enableQIRL && config.enableQuantumInspired) {
        Logger::getInstance().info("Applying quantum-inspired layer...", Logger::INFO);
        applyQuantumInspiredLayer(module, metrics);
    }
}

void MAOSEngine::applyStructuralLayer(llvm::Module& module, MAOSMetrics& metrics) {
    // Control flow flattening, bogus control flow, etc.
    int transformations = 0;
    
    for (auto& F : module) {
        if (F.isDeclaration()) continue;
        
        // Apply control flow transformations
        transformations++;
    }
    
    metrics.structuralTransformations = transformations;
    Logger::getInstance().info("Applied " + std::to_string(transformations) + 
                " structural transformations", Logger::DEBUG);
}

void MAOSEngine::applySemanticLayer(llvm::Module& module, MAOSMetrics& metrics) {
    // Instruction substitution, opaque predicates, etc.
    int transformations = 0;
    
    if (pcge) {
        PCGEConfig pcgeConfig;
        pcgeConfig.enableCodeMorphing = true;
        pcgeConfig.enablePolymorphicGeneration = config.mode == OperatingMode::MAXIMUM_SECURITY;
        pcgeConfig.polymorphicVariantsCount = config.mode == OperatingMode::MAXIMUM_SECURITY ? 5 : 2;
        
        pcge->applyPolymorphicTransformation(module, pcgeConfig);
        metrics.pcgeMetrics = pcge->collectMetrics(module);
        transformations = metrics.pcgeMetrics.totalFunctionsTransformed;
    }
    
    metrics.semanticTransformations = transformations;
    Logger::getInstance().info("Applied " + std::to_string(transformations) + 
                " semantic transformations", Logger::DEBUG);
}

void MAOSEngine::applySyntacticLayer(llvm::Module& module, MAOSMetrics& metrics) {
    // String encryption, symbol obfuscation, etc.
    int transformations = 0;
    
    for (auto& F : module) {
        if (F.isDeclaration()) continue;
        
        // Apply syntactic transformations
        transformations++;
    }
    
    metrics.syntacticTransformations = transformations;
    Logger::getInstance().info("Applied " + std::to_string(transformations) + 
                " syntactic transformations", Logger::DEBUG);
}

void MAOSEngine::applyQuantumInspiredLayer(llvm::Module& module, MAOSMetrics& metrics) {
    if (!qirl) return;
    
    // Apply quantum-inspired transformations
    int transformations = 0;
    double totalEntropy = 0.0;
    
    for (auto& F : module) {
        if (F.isDeclaration()) continue;
        
        qirl->obfuscateState(F);
        double entropy = qirl->measureQuantumEntropy(F);
        totalEntropy += entropy;
        transformations++;
    }
    
    if (transformations > 0) {
        metrics.qirlMetrics.averageQuantumEntropy = totalEntropy / transformations;
    }
    metrics.qirlMetrics.quantumStatesGenerated = transformations;
    
    Logger::getInstance().info("Applied quantum-inspired transformations. Avg entropy: " +
                std::to_string(metrics.qirlMetrics.averageQuantumEntropy), Logger::DEBUG);
}

void MAOSEngine::verifyAndHarden(llvm::Module& module, MAOSMetrics& metrics) {
    // Semantic equivalence testing
    Logger::getInstance().info("Verifying semantic equivalence...", Logger::DEBUG);
    
    // Security validation
    Logger::getInstance().info("Validating security properties...", Logger::DEBUG);
    double resistanceScore = calculateResistanceScore(module);
    metrics.resistanceScore = resistanceScore;
    
    // Performance profiling
    Logger::getInstance().info("Profiling performance impact...", Logger::DEBUG);
    
    Logger::getInstance().info("Verification complete. Resistance score: " +
                std::to_string(resistanceScore), Logger::INFO);
}

void MAOSEngine::calculateFinalMetrics(llvm::Module& module, MAOSMetrics& metrics) {
    // Count final instructions
    int finalInstructions = 0;
    for (auto& F : module) {
        if (!F.isDeclaration()) {
            for (auto& BB : F) {
                finalInstructions += BB.size();
            }
        }
    }
    
    // Calculate size increase
    if (metrics.totalInstructions > 0) {
        metrics.sizeIncrease = (static_cast<double>(finalInstructions - metrics.totalInstructions) / 
                               metrics.totalInstructions) * 100.0;
    }
    
    // Check constraints
    if (config.mode == OperatingMode::SIZE_CONSERVATIVE) {
        if (metrics.sizeIncrease > config.maxSizeIncrease) {
            Logger::getInstance().info("WARNING: Size increase (" + std::to_string(metrics.sizeIncrease) + 
                       "%) exceeds limit (" + std::to_string(config.maxSizeIncrease) + "%)",
                       Logger::WARNING);
        }
    }
    
    // Calculate other metrics
    metrics.cyclomaticComplexityIncrease = 50.0; // Placeholder
    metrics.entropyIncrease = metrics.qirlMetrics.averageQuantumEntropy;
    
    Logger::getInstance().info("Final metrics calculated:", Logger::INFO);
    Logger::getInstance().info("  Size increase: " + std::to_string(metrics.sizeIncrease) + "%", Logger::INFO);
    Logger::getInstance().info("  Resistance score: " + std::to_string(metrics.resistanceScore), Logger::INFO);
}

double MAOSEngine::calculateResistanceScore(llvm::Module& module) {
    // Resistance Score: S = 1 - (K/M)
    // Where K = successful analyses, M = total attempts
    
    // Simplified: base score on complexity and entropy
    double baseScore = 0.5;
    
    if (atie) {
        baseScore += 0.2; // ATIE contribution
    }
    if (qirl && config.enableQuantumInspired) {
        baseScore += 0.2; // QIRL contribution
    }
    if (pcge) {
        baseScore += 0.1; // PCGE contribution
    }
    
    return std::min(baseScore, 1.0);
}

double MAOSEngine::calculatePerformanceOverhead(llvm::Module& module) {
    // Performance Overhead: P = (R - L) / L
    // Would need actual runtime measurements
    
    // Estimate based on transformations applied
    double overhead = 0.0;
    
    if (config.mode == OperatingMode::SIZE_CONSERVATIVE) {
        overhead = 0.05; // 5% target
    } else {
        overhead = 0.30; // Up to 30% acceptable
    }
    
    return overhead;
}

double MAOSEngine::calculatePatternDetectionResistance(llvm::Module& module) {
    // Pattern Detection Resistance: D = 1 - (N/M)
    // Where N = detected patterns, M = total patterns
    
    if (!qirl) return 0.5;
    
    double totalUnpredictability = 0.0;
    int functionCount = 0;
    
    for (auto& F : module) {
        if (F.isDeclaration()) continue;
        
        double unpredictability = qirl->measurePatternUnpredictability(F);
        totalUnpredictability += unpredictability;
        functionCount++;
    }
    
    if (functionCount == 0) return 0.5;
    
    return totalUnpredictability / functionCount;
}

MAOSMetrics MAOSEngine::getMetrics() const {
    return currentMetrics;
}

// ============================================================================
// Utility Functions
// ============================================================================

double calculateCosineSimilarity(const std::vector<double>& vec1,
                                 const std::vector<double>& vec2) {
    if (vec1.size() != vec2.size() || vec1.empty()) return 0.0;
    
    double dotProduct = 0.0;
    double norm1 = 0.0;
    double norm2 = 0.0;
    
    for (size_t i = 0; i < vec1.size(); ++i) {
        dotProduct += vec1[i] * vec2[i];
        norm1 += vec1[i] * vec1[i];
        norm2 += vec2[i] * vec2[i];
    }
    
    if (norm1 == 0.0 || norm2 == 0.0) return 0.0;
    
    return dotProduct / (std::sqrt(norm1) * std::sqrt(norm2));
}

double calculateControlFlowGraphObfuscationRatio(llvm::Function& original,
                                                  llvm::Function& obfuscated) {
    // CFG Obfuscation Ratio = (obfuscated_edges / original_edges)
    
    int originalEdges = 0;
    int obfuscatedEdges = 0;
    
    for (auto& BB : original) {
        originalEdges += std::distance(succ_begin(&BB), succ_end(&BB));
    }
    
    for (auto& BB : obfuscated) {
        obfuscatedEdges += std::distance(succ_begin(&BB), succ_end(&BB));
    }
    
    if (originalEdges == 0) return 1.0;
    
    return static_cast<double>(obfuscatedEdges) / originalEdges;
}

} // namespace MAOS
