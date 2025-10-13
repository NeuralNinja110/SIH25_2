/**
 * @file AutoTuner.h
 * @brief Auto-tuning system for obfuscation parameter optimization
 * @version 2.0.0
 * @date 2025-10-13
 */

#ifndef AUTO_TUNER_H
#define AUTO_TUNER_H

#include "ObfuscationConfig.h"
#include <string>
#include <vector>
#include <map>

namespace obfuscator {

/**
 * @struct EvaluationMetrics
 * @brief Metrics from RE benchmark evaluation
 */
struct EvaluationMetrics {
    // Primary scores
    double reDifficultyScore;      // RE Difficulty Score (0-100)
    double securityScore;          // Security Score (0-100)
    double resilienceScore;        // Resilience Score (0-100)
    
    // Component scores
    double complexityScore;
    double symbolScore;
    double stringScore;
    double disasmScore;
    double cfgScore;
    double dataflowScore;
    double antiAnalysisScore;
    double decompilationScore;
    
    // Performance metrics
    double compilationTime;        // Seconds
    double analysisTime;           // Seconds
    double binarySize;             // Bytes
    double sizeOverhead;           // Percentage increase
    
    // Correctness
    bool functionalityPreserved;   // Did it execute correctly?
    
    // Overall fitness (weighted combination)
    double fitnessScore;
    
    EvaluationMetrics() 
        : reDifficultyScore(0.0), securityScore(0.0), resilienceScore(0.0),
          complexityScore(0.0), symbolScore(0.0), stringScore(0.0),
          disasmScore(0.0), cfgScore(0.0), dataflowScore(0.0),
          antiAnalysisScore(0.0), decompilationScore(0.0),
          compilationTime(0.0), analysisTime(0.0), binarySize(0.0),
          sizeOverhead(0.0), functionalityPreserved(false), fitnessScore(0.0) {}
};

/**
 * @struct ConfigCandidate
 * @brief Configuration candidate with evaluation metrics
 */
struct ConfigCandidate {
    ObfuscationConfig config;
    EvaluationMetrics metrics;
    int iteration;
    std::string configPath;
    
    ConfigCandidate() : iteration(0) {}
};

/**
 * @class AutoTuner
 * @brief Automatic parameter tuning using iterative optimization
 */
class AutoTuner {
public:
    /**
     * @brief Constructor
     * @param baseConfigPath Path to base configuration YAML
     * @param inputFile Input source file to obfuscate
     * @param outputDir Directory for outputs
     */
    AutoTuner(const std::string& baseConfigPath, 
              const std::string& inputFile,
              const std::string& outputDir);
    
    ~AutoTuner();
    
    /**
     * @brief Run auto-tuning optimization
     * @param iterations Number of optimization iterations
     * @return Best configuration found
     */
    ConfigCandidate optimize(int iterations);
    
    /**
     * @brief Set optimization goal
     * @param goal "security" (maximize RE difficulty), 
     *             "balanced" (balance security and performance),
     *             "size" (minimize size with good security)
     */
    void setOptimizationGoal(const std::string& goal);
    
    /**
     * @brief Get best configuration
     * @return Best config candidate
     */
    const ConfigCandidate& getBestCandidate() const { return bestCandidate_; }
    
    /**
     * @brief Generate optimization report
     * @param reportPath Output path for report
     */
    void generateReport(const std::string& reportPath) const;
    
private:
    std::string baseConfigPath_;
    std::string inputFile_;
    std::string outputDir_;
    std::string optimizationGoal_;
    
    ConfigCandidate bestCandidate_;
    std::vector<ConfigCandidate> candidateHistory_;
    
    // Optimization parameters
    struct {
        double securityWeight = 0.7;
        double performanceWeight = 0.2;
        double sizeWeight = 0.1;
    } weights_;
    
    /**
     * @brief Load base configuration from YAML
     */
    bool loadBaseConfig(ObfuscationConfig& config);
    
    /**
     * @brief Generate parameter variations for iteration
     */
    std::vector<ObfuscationConfig> generateCandidates(int iteration);
    
    /**
     * @brief Run obfuscation with given config
     */
    bool runObfuscation(const ObfuscationConfig& config, 
                        const std::string& outputFile,
                        double& compilationTime);
    
    /**
     * @brief Run RE benchmark evaluation
     */
    bool runREBenchmark(const std::string& binaryFile,
                        EvaluationMetrics& metrics);
    
    /**
     * @brief Calculate fitness score
     */
    double calculateFitness(const EvaluationMetrics& metrics);
    
    /**
     * @brief Mutate configuration based on previous results
     */
    ObfuscationConfig mutateConfig(const ObfuscationConfig& base, 
                                   int iteration,
                                   double mutationRate);
    
    /**
     * @brief Adapt parameters based on observed trends
     */
    void adaptParameters(int iteration);
    
    /**
     * @brief Save configuration to YAML file
     */
    bool saveConfigToYAML(const ObfuscationConfig& config,
                          const std::string& path);
    
    /**
     * @brief Parse RE benchmark JSON output
     */
    bool parseREResults(const std::string& jsonPath,
                        EvaluationMetrics& metrics);
};

} // namespace obfuscator

#endif // AUTO_TUNER_H
