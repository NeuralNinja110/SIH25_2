#ifndef MAOS_ENGINE_H
#define MAOS_ENGINE_H

#include "ATIE.h"
#include "QIRL.h"
#include "PCGE.h"
#include "ObfuscationConfig.h"
#include "llvm/IR/Module.h"
#include <memory>
#include <string>
#include <vector>

// Multi-Layered Adaptive Obfuscation System (MAOS)
// Main orchestration engine

namespace MAOS {

// Operating mode configuration
struct MAOSConfig {
    ObfuscationMode mode;
    
    // Size-Conservative parameters
    double maxSizeIncrease;          // 15% for size-conservative
    double maxTimeOverhead;           // 5% for size-conservative
    bool compressionEnabled;
    bool debugSymbolsPreserved;
    
    // Maximum Security parameters
    double securityThreshold;         // 95% for maximum security
    bool quantumEnhancement;
    bool antiAnalysisEnabled;
    bool selfModificationEnabled;
    
    // Genetic algorithm configuration
    struct GAConfig {
        int populationSize;
        int generations;
        int tournamentSize;
        double crossoverRate;
        double mutationRate;
    } geneticAlgorithm;
    
    // Pass limits
    int passLimit;
    
    // Advanced features
    bool hardwareAssistedProtection;
    bool runtimeIntegrityVerification;
    
    // Output configuration
    std::string outputBinary;
    std::string reportPath;
    bool generateJSONReport;
    bool generateHTMLReport;
    bool generateSecurityAudit;
};

// Comprehensive metrics for reporting
struct MAOSMetrics {
    // Input/Output
    std::string inputFile;
    std::string outputFile;
    size_t originalSize;
    size_t obfuscatedSize;
    double sizeIncreasePercentage;
    long compilationTimeMs;
    
    // Mode information
    ObfuscationMode mode;
    int totalCycles;
    int totalPassesExecuted;
    
    // ATIE Metrics
    struct ATIEMetrics {
        ComplexityMetrics inputComplexity;
        int mlSelectedPasses;
        double resistanceScore;
        double contextAdaptationScore;
    } atie;
    
    // QIRL Metrics
    struct QIRLMetrics {
        double overallEntropyScore;
        double quantumEntropyMeasure;
        int entangledBlockCount;
        int quantumPredicateCount;
        double averageComputationalHardness;
        double quantumKeyStrength;
    } qirl;
    
    // PCGE Metrics
    struct PCGEMetrics {
        int variantsGenerated;
        int selfModificationPoints;
        int templatesApplied;
        int polymorphicFunctions;
        double averageStructuralDifference;
        double semanticEquivalenceScore;
    } pcge;
    
    // Advanced Metrics
    double cyclomaticComplexityIncrease;
    double controlFlowGraphObfuscationRatio;
    double patternDetectionResistance;
    double cosineSimilarity;
    double shannonEntropy;
    
    // Resistance Metrics
    struct ResistanceMetrics {
        double idaProResistance;
        double ghidraResistance;
        double radare2Resistance;
        double binaryNinjaResistance;
        double overallResistance;
        double estimatedReverseEngineeringTime;  // in hours
    } resistance;
    
    // Performance Metrics
    struct PerformanceMetrics {
        double executionOverheadPercentage;
        double memoryOverheadPercentage;
        double cachePerformanceImpact;
        long averageExecutionTimeMs;
    } performance;
    
    // Transformation Details
    std::vector<std::string> appliedPasses;
    std::map<std::string, long> passExecutionTimes;
    std::map<std::string, int> transformationCounts;
    
    // Genetic Algorithm Evolution
    struct GAEvolutionMetrics {
        int generations;
        double bestFitness;
        double averageFitness;
        double diversityScore;
        int convergenceGeneration;
        std::vector<double> fitnessHistory;
    } gaEvolution;
};

// Main MAOS Engine
class MAOSEngine {
public:
    MAOSEngine(const MAOSConfig& config);
    ~MAOSEngine();
    
    // Main obfuscation pipeline
    bool obfuscate(const std::string& inputFile);
    
    // Phase 1: Analysis and Profiling
    bool analyzeCode(const std::string& inputFile);
    ComplexityMetrics profileCode();
    void identifyCriticalPaths();
    void mapVulnerabilitySurface();
    
    // Phase 2: Adaptive Transformation Planning
    std::vector<TransformationPass> planTransformations();
    void adaptToThreatModel();
    void manageResourceConstraints();
    
    // Phase 3: Multi-Layer Obfuscation Application
    bool applyStructuralLayer(llvm::Module& module);
    bool applySemanticLayer(llvm::Module& module);
    bool applySyntacticLayer(llvm::Module& module);
    bool applyQuantumLayer(llvm::Module& module);
    
    // Phase 4: Verification and Hardening
    bool verifySemanticEquivalence();
    bool validateSecurity();
    bool profilePerformance();
    
    // Component access
    AdaptiveTransformationEngine& getATIE();
    QIRLManager& getQIRL();
    PCGEManager& getPCGE();
    
    // Metrics and reporting
    const MAOSMetrics& getMetrics() const { return metrics_; }
    void generateReports();
    void generateJSONReport(const std::string& path);
    void generateHTMLReport(const std::string& path);
    void generateSecurityAudit(const std::string& path);
    
    // Configuration
    void setMode(ObfuscationMode mode);
    void updateConfig(const MAOSConfig& config);
    const MAOSConfig& getConfig() const { return config_; }
    
private:
    MAOSConfig config_;
    MAOSMetrics metrics_;
    
    // Core components
    std::unique_ptr<AdaptiveTransformationEngine> atie_;
    std::unique_ptr<QIRLManager> qirl_;
    std::unique_ptr<PCGEManager> pcge_;
    std::unique_ptr<GeneticAlgorithmOptimizer> gaOptimizer_;
    std::unique_ptr<ResistanceAssessor> resistanceAssessor_;
    
    // LLVM infrastructure
    std::unique_ptr<llvm::LLVMContext> context_;
    std::unique_ptr<llvm::Module> module_;
    
    // State tracking
    std::string currentInputFile_;
    std::string currentOutputFile_;
    bool initialized_;
    
    // Helper methods
    bool compileToIR(const std::string& inputFile);
    bool compileToBinary(const std::string& outputFile);
    void initializeComponents();
    void collectAllMetrics();
    void updateMetrics();
    
    // Mode-specific implementations
    void applySizeConservativeOptimizations();
    void applyMaximumSecurityTransformations();
    
    // Validation
    bool validateConfiguration();
    bool checkConstraints();
};

// Configuration builder
class MAOSConfigBuilder {
public:
    MAOSConfigBuilder();
    
    // Mode selection
    MAOSConfigBuilder& setSizeConservativeMode();
    MAOSConfigBuilder& setMaximumSecurityMode();
    
    // Size-conservative options
    MAOSConfigBuilder& setMaxSizeIncrease(double percentage);
    MAOSConfigBuilder& setMaxTimeOverhead(double percentage);
    MAOSConfigBuilder& enableCompression(bool enable = true);
    MAOSConfigBuilder& preserveDebugSymbols(bool preserve = true);
    
    // Maximum security options
    MAOSConfigBuilder& setSecurityThreshold(double threshold);
    MAOSConfigBuilder& enableQuantumEnhancement(bool enable = true);
    MAOSConfigBuilder& enableAntiAnalysis(bool enable = true);
    MAOSConfigBuilder& enableSelfModification(bool enable = true);
    
    // Genetic algorithm
    MAOSConfigBuilder& setPopulationSize(int size);
    MAOSConfigBuilder& setGenerations(int generations);
    MAOSConfigBuilder& setTournamentSize(int size);
    
    // Limits
    MAOSConfigBuilder& setPassLimit(int limit);
    
    // Output
    MAOSConfigBuilder& setOutputBinary(const std::string& path);
    MAOSConfigBuilder& setReportPath(const std::string& path);
    
    // Build
    MAOSConfig build();
    
private:
    MAOSConfig config_;
};

// Utility functions
namespace Utils {
    // Metrics calculations
    double calculateResistanceScore(int successful, int total);
    double calculatePerformanceOverhead(double obfuscated, double original);
    double calculatePatternDetectionRate(int detected, int total);
    double calculateCyclomaticComplexity(llvm::Function& F);
    double calculateEntropy(const std::vector<uint8_t>& data);
    double calculateCosineSimilarity(llvm::Module& M1, llvm::Module& M2);
    
    // File operations
    size_t getFileSize(const std::string& filename);
    std::string readFile(const std::string& filename);
    bool writeFile(const std::string& filename, const std::string& content);
    
    // Time utilities
    std::string getCurrentTimestamp();
    long measureExecutionTime(const std::function<void()>& func);
}

} // namespace MAOS

#endif // MAOS_ENGINE_H
