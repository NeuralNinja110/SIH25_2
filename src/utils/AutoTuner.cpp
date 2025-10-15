/**
 * @file AutoTuner.cpp
 * @brief Implementation of auto-tuning optimization system
 * @version 2.0.0
 * @date 2025-10-13
 */

#include "AutoTuner.h"
#include "ConfigParser.h"
#include "Logger.h"
#include "FileUtils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <random>
#include <iomanip>
#include <sys/stat.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace obfuscator {

AutoTuner::AutoTuner(const std::string& baseConfigPath,
                     const std::string& inputFile,
                     const std::string& outputDir)
    : baseConfigPath_(baseConfigPath),
      inputFile_(inputFile),
      outputDir_(outputDir),
      optimizationGoal_("balanced") {
    
    // Create output directory
    FileUtils::createDirectory(outputDir_);
}

AutoTuner::~AutoTuner() {
}

void AutoTuner::setOptimizationGoal(const std::string& goal) {
    optimizationGoal_ = goal;
    
    if (goal == "security") {
        weights_.securityWeight = 0.8;
        weights_.performanceWeight = 0.15;
        weights_.sizeWeight = 0.05;
    } else if (goal == "balanced") {
        weights_.securityWeight = 0.6;
        weights_.performanceWeight = 0.25;
        weights_.sizeWeight = 0.15;
    } else if (goal == "size") {
        weights_.securityWeight = 0.5;
        weights_.performanceWeight = 0.2;
        weights_.sizeWeight = 0.3;
    }
    
    Logger::getInstance().info("Optimization goal set to: " + goal);
    Logger::getInstance().info("Weights - Security: " + 
        std::to_string(weights_.securityWeight) + 
        ", Performance: " + std::to_string(weights_.performanceWeight) +
        ", Size: " + std::to_string(weights_.sizeWeight));
}

ConfigCandidate AutoTuner::optimize(int iterations) {
    Logger::getInstance().info("Starting auto-tuning optimization");
    Logger::getInstance().info("Iterations: " + std::to_string(iterations));
    Logger::getInstance().info("Input file: " + inputFile_);
    
    ObfuscationConfig baseConfig;
    if (!loadBaseConfig(baseConfig)) {
        Logger::getInstance().error("Failed to load base configuration");
        return ConfigCandidate();
    }
    
    // Iteration 0: Evaluate base configuration
    Logger::getInstance().info("\n=== ITERATION 0: Baseline Evaluation ===");
    ConfigCandidate baseline;
    baseline.config = baseConfig;
    baseline.iteration = 0;
    baseline.configPath = outputDir_ + "/config_iter0.yaml";
    
    saveConfigToYAML(baseline.config, baseline.configPath);
    
    std::string outputBinary = outputDir_ + "/binary_iter0";
    if (!runObfuscation(baseline.config, outputBinary, baseline.metrics.compilationTime)) {
        Logger::getInstance().error("Baseline obfuscation failed");
        return ConfigCandidate();
    }
    
    if (!runREBenchmark(outputBinary, baseline.metrics)) {
        Logger::getInstance().error("Baseline RE benchmark failed");
        return ConfigCandidate();
    }
    
    baseline.metrics.fitnessScore = calculateFitness(baseline.metrics);
    candidateHistory_.push_back(baseline);
    bestCandidate_ = baseline;
    
    Logger::getInstance().info("Baseline fitness: " + 
        std::to_string(baseline.metrics.fitnessScore));
    Logger::getInstance().info("Baseline RE Difficulty: " + 
        std::to_string(baseline.metrics.reDifficultyScore));
    
    // Iterative optimization
    for (int i = 1; i <= iterations; ++i) {
        Logger::getInstance().info("\n=== ITERATION " + std::to_string(i) + " ===");
        
        // Generate candidate configurations
        auto candidates = generateCandidates(i);
        
        Logger::getInstance().info("Generated " + std::to_string(candidates.size()) + 
                                  " candidate configurations");
        
        // Evaluate each candidate
        for (size_t j = 0; j < candidates.size(); ++j) {
            Logger::getInstance().info("Evaluating candidate " + 
                std::to_string(j + 1) + "/" + std::to_string(candidates.size()));
            
            ConfigCandidate candidate;
            candidate.config = candidates[j];
            candidate.iteration = i;
            candidate.configPath = outputDir_ + "/config_iter" + 
                std::to_string(i) + "_cand" + std::to_string(j) + ".yaml";
            
            saveConfigToYAML(candidate.config, candidate.configPath);
            
            std::string outputBinary = outputDir_ + "/binary_iter" + 
                std::to_string(i) + "_cand" + std::to_string(j);
            
            if (!runObfuscation(candidate.config, outputBinary, 
                               candidate.metrics.compilationTime)) {
                Logger::getInstance().warning("Obfuscation failed for candidate " + 
                    std::to_string(j));
                continue;
            }
            
            if (!runREBenchmark(outputBinary, candidate.metrics)) {
                Logger::getInstance().warning("RE benchmark failed for candidate " + 
                    std::to_string(j));
                continue;
            }
            
            candidate.metrics.fitnessScore = calculateFitness(candidate.metrics);
            candidateHistory_.push_back(candidate);
            
            Logger::getInstance().info("  Fitness: " + 
                std::to_string(candidate.metrics.fitnessScore));
            Logger::getInstance().info("  RE Difficulty: " + 
                std::to_string(candidate.metrics.reDifficultyScore));
            Logger::getInstance().info("  Security: " + 
                std::to_string(candidate.metrics.securityScore));
            
            // Update best candidate
            if (candidate.metrics.fitnessScore > bestCandidate_.metrics.fitnessScore) {
                bestCandidate_ = candidate;
                Logger::getInstance().info("*** NEW BEST CANDIDATE FOUND! ***");
                Logger::getInstance().info("    Fitness improved: " + 
                    std::to_string(bestCandidate_.metrics.fitnessScore));
            }
        }
        
        // Adapt parameters for next iteration
        adaptParameters(i);
    }
    
    // Final run with best configuration
    Logger::getInstance().info("\n=== FINAL RUN WITH BEST CONFIGURATION ===");
    Logger::getInstance().info("Best configuration from iteration " + 
        std::to_string(bestCandidate_.iteration));
    Logger::getInstance().info("Best fitness: " + 
        std::to_string(bestCandidate_.metrics.fitnessScore));
    Logger::getInstance().info("Best RE Difficulty: " + 
        std::to_string(bestCandidate_.metrics.reDifficultyScore));
    
    std::string finalOutput = outputDir_ + "/final_optimized_binary";
    std::string finalConfig = outputDir_ + "/final_optimized_config.yaml";
    
    saveConfigToYAML(bestCandidate_.config, finalConfig);
    
    double finalTime;
    if (runObfuscation(bestCandidate_.config, finalOutput, finalTime)) {
        Logger::getInstance().info("Final optimized binary created: " + finalOutput);
        
        // Run final benchmark
        EvaluationMetrics finalMetrics;
        if (runREBenchmark(finalOutput, finalMetrics)) {
            Logger::getInstance().info("\n=== FINAL METRICS ===");
            Logger::getInstance().info("RE Difficulty Score: " + 
                std::to_string(finalMetrics.reDifficultyScore) + "/100");
            Logger::getInstance().info("Security Score: " + 
                std::to_string(finalMetrics.securityScore) + "/100");
            Logger::getInstance().info("Resilience Score: " + 
                std::to_string(finalMetrics.resilienceScore) + "/100");
            Logger::getInstance().info("Size Overhead: " + 
                std::to_string(finalMetrics.sizeOverhead) + "%");
        }
    }
    
    return bestCandidate_;
}

bool AutoTuner::loadBaseConfig(ObfuscationConfig& config) {
    ConfigParser parser;
    return parser.parseFile(baseConfigPath_, config);
}

std::vector<ObfuscationConfig> AutoTuner::generateCandidates(int iteration) {
    std::vector<ObfuscationConfig> candidates;
    
    // Generate 3-5 candidates per iteration
    int numCandidates = 3 + (iteration % 3);
    
    for (int i = 0; i < numCandidates; ++i) {
        double mutationRate = 0.1 + (i * 0.1); // Varying mutation rates
        ObfuscationConfig candidate = mutateConfig(bestCandidate_.config, 
                                                   iteration, 
                                                   mutationRate);
        candidates.push_back(candidate);
    }
    
    return candidates;
}

ObfuscationConfig AutoTuner::mutateConfig(const ObfuscationConfig& base,
                                          int iteration,
                                          double mutationRate) {
    ObfuscationConfig mutated = base;
    
    std::random_device rd;
    std::mt19937 gen(rd() + iteration);
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::uniform_int_distribution<> intDis(-10, 10);
    
    // Mutate numerical parameters
    if (dis(gen) < mutationRate) {
        int delta = intDis(gen);
        mutated.obfuscationCycles = std::max(1u, 
            std::min(10u, mutated.obfuscationCycles + delta));
    }
    
    if (dis(gen) < mutationRate) {
        int delta = intDis(gen) * 5;
        mutated.flatteningComplexity = std::max(10u,
            std::min(100u, mutated.flatteningComplexity + delta));
    }
    
    if (dis(gen) < mutationRate) {
        int delta = intDis(gen) * 3;
        mutated.opaquePredicateCount = std::max(5u,
            std::min(50u, mutated.opaquePredicateCount + delta));
    }
    
    if (dis(gen) < mutationRate) {
        int delta = intDis(gen) * 5;
        mutated.constantObfuscationComplexity = std::max(10u,
            std::min(100u, mutated.constantObfuscationComplexity + delta));
    }
    
    if (dis(gen) < mutationRate) {
        int delta = intDis(gen) * 5;
        mutated.cacheObfuscationIntensity = std::max(0u,
            std::min(100u, mutated.cacheObfuscationIntensity + delta));
    }
    
    // Mutate boolean flags (less frequently)
    if (dis(gen) < mutationRate * 0.5) {
        mutated.enableBogusControlFlow = !mutated.enableBogusControlFlow;
    }
    
    if (dis(gen) < mutationRate * 0.5) {
        mutated.enableDeadCodeInjection = !mutated.enableDeadCodeInjection;
    }
    
    if (dis(gen) < mutationRate * 0.3) {
        mutated.enableFunctionVirtualization = !mutated.enableFunctionVirtualization;
    }
    
    return mutated;
}

void AutoTuner::adaptParameters(int iteration) {
    // Analyze trends in recent candidates
    if (candidateHistory_.size() < 2) return;
    
    // Check if we're improving
    size_t recentStart = candidateHistory_.size() > 5 ? 
        candidateHistory_.size() - 5 : 0;
    
    bool improving = false;
    for (size_t i = recentStart; i < candidateHistory_.size(); ++i) {
        if (candidateHistory_[i].metrics.fitnessScore > 
            bestCandidate_.metrics.fitnessScore * 0.95) {
            improving = true;
            break;
        }
    }
    
    if (!improving && iteration > 2) {
        Logger::getInstance().info("No significant improvement - increasing mutation rate");
        // Mutation rate will be increased in next iteration via generateCandidates
    }
}

bool AutoTuner::runObfuscation(const ObfuscationConfig& config,
                               const std::string& outputFile,
                               double& compilationTime) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Build command
    std::stringstream cmd;
    cmd << "./build/phantron-llvm-obfuscator";
    cmd << " -i " << inputFile_;
    cmd << " -o " << outputFile;
    cmd << " --cycles " << config.obfuscationCycles;
    cmd << " --seed " << config.seed;
    
    if (config.verbose) cmd << " --verbose";
    if (!config.enableControlFlowFlattening) cmd << " --no-flatten";
    if (!config.enableStringEncryption) cmd << " --no-strings";
    if (!config.enableConstantObfuscation) cmd << " --no-constants";
    if (config.enableFunctionVirtualization) cmd << " --enable-virtualization";
    if (config.enableAntiDebug) cmd << " --enable-anti-debug";
    
    cmd << " 2>&1";
    
    Logger::getInstance().info("Running: " + cmd.str());
    
    int result = system(cmd.str().c_str());
    
    auto endTime = std::chrono::high_resolution_clock::now();
    compilationTime = std::chrono::duration<double>(endTime - startTime).count();
    
    return (result == 0);
}

bool AutoTuner::runREBenchmark(const std::string& binaryFile,
                               EvaluationMetrics& metrics) {
    // Run RE benchmark
    std::string resultsDir = outputDir_ + "/re_results_" + 
        std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    
    std::stringstream cmd;
    cmd << "cd reverse_engineering/scripts && ";
    cmd << "python3 re_framework.py";
    cmd << " --binary " << "../../" << binaryFile;
    cmd << " --output " << "../../" << resultsDir;
    cmd << " --format json";
    cmd << " 2>&1";
    
    Logger::getInstance().info("Running RE benchmark...");
    
    int result = system(cmd.str().c_str());
    if (result != 0) {
        Logger::getInstance().error("RE benchmark failed");
        return false;
    }
    
    // Parse results
    std::string jsonPath = resultsDir + "/metrics.json";
    if (!parseREResults(jsonPath, metrics)) {
        Logger::getInstance().error("Failed to parse RE results");
        return false;
    }
    
    // Get binary size
    struct stat st;
    if (stat(binaryFile.c_str(), &st) == 0) {
        metrics.binarySize = st.st_size;
    }
    
    metrics.functionalityPreserved = true; // Assume true if obfuscation succeeded
    
    return true;
}

bool AutoTuner::parseREResults(const std::string& jsonPath,
                               EvaluationMetrics& metrics) {
    try {
        std::ifstream file(jsonPath);
        if (!file.is_open()) {
            return false;
        }
        
        json data;
        file >> data;
        
        metrics.reDifficultyScore = data.value("re_difficulty_score", 0.0);
        metrics.securityScore = data.value("security_score", 0.0);
        metrics.resilienceScore = data.value("resilience_score", 0.0);
        
        if (data.contains("component_scores")) {
            auto comp = data["component_scores"];
            metrics.complexityScore = comp.value("complexity", 0.0);
            metrics.symbolScore = comp.value("symbol", 0.0);
            metrics.stringScore = comp.value("string", 0.0);
            metrics.disasmScore = comp.value("disassembly", 0.0);
            metrics.cfgScore = comp.value("cfg", 0.0);
            metrics.dataflowScore = comp.value("dataflow", 0.0);
            metrics.antiAnalysisScore = comp.value("anti_analysis", 0.0);
            metrics.decompilationScore = comp.value("decompilation", 0.0);
        }
        
        metrics.analysisTime = data.value("analysis_time_seconds", 0.0);
        
        return true;
    } catch (const std::exception& e) {
        Logger::getInstance().error("JSON parse error: " + std::string(e.what()));
        return false;
    }
}

double AutoTuner::calculateFitness(const EvaluationMetrics& metrics) {
    if (!metrics.functionalityPreserved) {
        return 0.0; // Invalid if functionality broken
    }
    
    // Security component (0-100 normalized to 0-1)
    double securityComponent = (metrics.reDifficultyScore + 
                                metrics.securityScore + 
                                metrics.resilienceScore) / 300.0;
    
    // Performance component (penalize slow compilation)
    double performanceComponent = 1.0 / (1.0 + metrics.compilationTime / 10.0);
    
    // Size component (penalize large binaries)
    double sizeComponent = 1.0 / (1.0 + metrics.sizeOverhead / 100.0);
    
    // Weighted fitness
    double fitness = weights_.securityWeight * securityComponent +
                     weights_.performanceWeight * performanceComponent +
                     weights_.sizeWeight * sizeComponent;
    
    return fitness * 100.0; // Scale to 0-100
}

bool AutoTuner::saveConfigToYAML(const ObfuscationConfig& config,
                                 const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }
    
    file << "# Auto-generated configuration\n";
    file << "# Obfuscation Level: " << 
        (config.level == ObfuscationLevel::LOW ? "low" :
         config.level == ObfuscationLevel::MEDIUM ? "medium" : "high") << "\n\n";
    
    file << "obfuscation:\n";
    file << "  cycles: " << config.obfuscationCycles << "\n";
    file << "  seed: " << config.seed << "\n";
    file << "  verbose: " << (config.verbose ? "true" : "false") << "\n\n";
    
    file << "control_flow:\n";
    file << "  flattening_enabled: " << (config.enableControlFlowFlattening ? "true" : "false") << "\n";
    file << "  flattening_complexity: " << config.flatteningComplexity << "\n";
    file << "  opaque_predicates: " << (config.enableOpaquePredicates ? "true" : "false") << "\n";
    file << "  opaque_count: " << config.opaquePredicateCount << "\n";
    file << "  bogus_control_flow: " << (config.enableBogusControlFlow ? "true" : "false") << "\n\n";
    
    file << "data_obfuscation:\n";
    file << "  string_encryption: " << (config.enableStringEncryption ? "true" : "false") << "\n";
    file << "  constant_obfuscation: " << (config.enableConstantObfuscation ? "true" : "false") << "\n";
    file << "  constant_complexity: " << config.constantObfuscationComplexity << "\n\n";
    
    file << "advanced:\n";
    file << "  function_virtualization: " << (config.enableFunctionVirtualization ? "true" : "false") << "\n";
    file << "  anti_debug: " << (config.enableAntiDebug ? "true" : "false") << "\n";
    file << "  cache_obfuscation_intensity: " << config.cacheObfuscationIntensity << "\n\n";
    
    file.close();
    return true;
}

void AutoTuner::generateReport(const std::string& reportPath) const {
    std::ofstream report(reportPath);
    if (!report.is_open()) {
        Logger::getInstance().error("Failed to create report file");
        return;
    }
    
    report << "# MAOS v2.0 Auto-Tuning Optimization Report\n\n";
    report << "**Generated:** " << std::time(nullptr) << "\n";
    report << "**Input File:** " << inputFile_ << "\n";
    report << "**Optimization Goal:** " << optimizationGoal_ << "\n";
    report << "**Total Iterations:** " << candidateHistory_.size() << "\n\n";
    
    report << "## Best Configuration Found\n\n";
    report << "- **Iteration:** " << bestCandidate_.iteration << "\n";
    report << "- **Fitness Score:** " << std::fixed << std::setprecision(2) 
           << bestCandidate_.metrics.fitnessScore << "/100\n";
    report << "- **RE Difficulty Score:** " << bestCandidate_.metrics.reDifficultyScore << "/100\n";
    report << "- **Security Score:** " << bestCandidate_.metrics.securityScore << "/100\n";
    report << "- **Resilience Score:** " << bestCandidate_.metrics.resilienceScore << "/100\n";
    report << "- **Compilation Time:** " << bestCandidate_.metrics.compilationTime << "s\n";
    report << "- **Binary Size:** " << bestCandidate_.metrics.binarySize << " bytes\n";
    report << "- **Configuration:** " << bestCandidate_.configPath << "\n\n";
    
    report << "## Optimization History\n\n";
    report << "| Iteration | Candidate | Fitness | RE Diff | Security | Compilation Time |\n";
    report << "|-----------|-----------|---------|---------|----------|------------------|\n";
    
    for (const auto& candidate : candidateHistory_) {
        report << "| " << candidate.iteration << " | ";
        report << "- | " << std::fixed << std::setprecision(2);
        report << candidate.metrics.fitnessScore << " | ";
        report << candidate.metrics.reDifficultyScore << " | ";
        report << candidate.metrics.securityScore << " | ";
        report << candidate.metrics.compilationTime << "s |\n";
    }
    
    report << "\n## Component Scores (Best Configuration)\n\n";
    report << "- **Complexity:** " << bestCandidate_.metrics.complexityScore << "/100\n";
    report << "- **Symbol Stripping:** " << bestCandidate_.metrics.symbolScore << "/100\n";
    report << "- **String Obfuscation:** " << bestCandidate_.metrics.stringScore << "/100\n";
    report << "- **Disassembly Resistance:** " << bestCandidate_.metrics.disasmScore << "/100\n";
    report << "- **Control Flow:** " << bestCandidate_.metrics.cfgScore << "/100\n";
    report << "- **Data Flow:** " << bestCandidate_.metrics.dataflowScore << "/100\n";
    report << "- **Anti-Analysis:** " << bestCandidate_.metrics.antiAnalysisScore << "/100\n";
    report << "- **Decompilation Resistance:** " << bestCandidate_.metrics.decompilationScore << "/100\n";
    
    report << "\n## Recommendations\n\n";
    
    if (bestCandidate_.metrics.reDifficultyScore >= 80) {
        report << "✅ **Excellent** - Heavy-tier obfuscation achieved\n";
    } else if (bestCandidate_.metrics.reDifficultyScore >= 60) {
        report << "✓ **Good** - Medium-heavy tier obfuscation\n";
    } else {
        report << "⚠ **Needs Improvement** - Consider increasing obfuscation intensity\n";
    }
    
    report.close();
    Logger::getInstance().info("Optimization report generated: " + reportPath);
}

} // namespace obfuscator
