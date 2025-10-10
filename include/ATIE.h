#ifndef ATIE_H
#define ATIE_H

#include <vector>
#include <string>
#include <memory>
#include <map>

// Adaptive Transformation Intelligence Engine (ATIE)
// Machine Learning-Driven Pass Selection and Optimization

namespace MAOS {

// Code complexity metrics
struct ComplexityMetrics {
    int cyclomaticComplexity;
    int instructionCount;
    int basicBlockCount;
    int branchCount;
    int loopDepth;
    double entropyScore;
    double criticalityScore;
};

// Transformation pass metadata
struct TransformationPass {
    std::string passName;
    double securityImpact;
    double performanceOverhead;
    double sizeIncrease;
    int executionOrder;
    std::vector<std::string> dependencies;
    bool enabled;
};

// Genetic algorithm chromosome
struct Chromosome {
    std::vector<TransformationPass> passSequence;
    double fitnessScore;
    double securityScore;
    double performanceScore;
    double sizeScore;
    int generation;
};

// Operating mode
enum class ObfuscationMode {
    SIZE_CONSERVATIVE,
    MAXIMUM_SECURITY
};

// ATIE Core Engine
class AdaptiveTransformationEngine {
public:
    AdaptiveTransformationEngine(ObfuscationMode mode);
    ~AdaptiveTransformationEngine();
    
    // Analysis
    ComplexityMetrics analyzeCode(const std::string& inputFile);
    void identifyCriticalPaths();
    void assessVulnerabilitySurface();
    
    // ML-Driven Selection
    std::vector<TransformationPass> selectOptimalPasses(const ComplexityMetrics& metrics);
    void trainModel(const std::vector<std::pair<ComplexityMetrics, std::vector<TransformationPass>>>& history);
    
    // Resistance Assessment
    double evaluateResistance(const std::vector<TransformationPass>& passes);
    void assessAgainstTools(const std::vector<std::string>& deobfuscationTools);
    
    // Context-Aware Optimization
    void adaptToContext(const ComplexityMetrics& metrics);
    double calculateCriticalityScore(const std::string& functionName);
    
    // Getters
    ObfuscationMode getMode() const { return mode_; }
    const ComplexityMetrics& getMetrics() const { return metrics_; }
    
private:
    ObfuscationMode mode_;
    ComplexityMetrics metrics_;
    std::vector<TransformationPass> availablePasses_;
    std::map<std::string, double> criticalPathMap_;
    
    // ML Model components
    std::vector<double> mlWeights_;
    double learningRate_;
    
    // Helper methods
    void initializePassDatabase();
    double predictEffectiveness(const TransformationPass& pass, const ComplexityMetrics& metrics);
    void updateMLModel(double actualSecurity, double predictedSecurity);
};

// Genetic Algorithm Optimizer
class GeneticAlgorithmOptimizer {
public:
    GeneticAlgorithmOptimizer(ObfuscationMode mode);
    ~GeneticAlgorithmOptimizer();
    
    // Configuration
    void setPopulationSize(int size);
    void setGenerations(int generations);
    void setTournamentSize(int size);
    void setCrossoverRate(double rate);
    void setMutationRate(double rate);
    
    // Evolution
    std::vector<TransformationPass> evolveOptimalSequence(
        const std::vector<TransformationPass>& availablePasses,
        const ComplexityMetrics& metrics
    );
    
    // Fitness evaluation
    double calculateFitness(const Chromosome& chromosome, ObfuscationMode mode);
    
    // Operators
    Chromosome crossover(const Chromosome& parent1, const Chromosome& parent2);
    void mutate(Chromosome& chromosome);
    Chromosome tournamentSelection(const std::vector<Chromosome>& population);
    
    // Statistics
    struct EvolutionStats {
        double bestFitness;
        double averageFitness;
        double diversityScore;
        int convergenceGeneration;
        std::vector<double> fitnessHistory;
    };
    
    const EvolutionStats& getStats() const { return stats_; }
    
private:
    ObfuscationMode mode_;
    int populationSize_;
    int generations_;
    int tournamentSize_;
    double crossoverRate_;
    double mutationRate_;
    
    EvolutionStats stats_;
    std::vector<Chromosome> population_;
    
    // Fitness function components
    double calculateSecurityFitness(const Chromosome& chromosome);
    double calculatePerformanceFitness(const Chromosome& chromosome);
    double calculateSizeFitness(const Chromosome& chromosome);
    
    // Population management
    void initializePopulation(const std::vector<TransformationPass>& availablePasses);
    void evolveGeneration();
    bool checkConvergence();
};

// Real-time Resistance Assessor
class ResistanceAssessor {
public:
    ResistanceAssessor();
    ~ResistanceAssessor();
    
    // Tool-specific assessment
    struct ToolResistance {
        std::string toolName;
        double resistanceScore;
        std::vector<std::string> vulnerableAreas;
        std::vector<std::string> strongAreas;
    };
    
    // Assessment methods
    std::vector<ToolResistance> assessAgainstKnownTools(const std::string& binary);
    double calculateOverallResistance(const std::vector<ToolResistance>& results);
    void generateResistanceReport(const std::vector<ToolResistance>& results);
    
    // Continuous monitoring
    void startMonitoring();
    void stopMonitoring();
    bool isUnderAnalysis();
    
private:
    bool monitoring_;
    std::vector<std::string> knownTools_;
    
    // Tool signatures
    struct ToolSignature {
        std::string name;
        std::vector<std::string> patterns;
        double detectionThreshold;
    };
    
    std::vector<ToolSignature> toolSignatures_;
    
    void loadToolSignatures();
    double testAgainstIDA(const std::string& binary);
    double testAgainstGhidra(const std::string& binary);
    double testAgainstRadare2(const std::string& binary);
    double testAgainstBinaryNinja(const std::string& binary);
};

} // namespace MAOS

#endif // ATIE_H
