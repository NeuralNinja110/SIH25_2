/**
 * @file main.cpp
 * @brief Main entry point for LLVM obfuscator
 * @version 2.0.0
 * @date 2025-10-13
 */

#include "ObfuscationEngine.h"
#include "CLIParser.h"
#include "AutoTuner.h"
#include "Logger.h"
#include <iostream>
#include <exception>

using namespace obfuscator;

int main(int argc, char* argv[]) {
    try {
        // Parse command line arguments
        CLIParser parser;
        if (!parser.parse(argc, argv)) {
            if (!parser.shouldShowHelp() && !parser.shouldShowVersion()) {
                std::cerr << "Failed to parse arguments. Use --help for usage information.\n";
                return 1;
            }
        }
        
        if (parser.shouldShowHelp()) {
            parser.printHelp();
            return 0;
        }
        
        if (parser.shouldShowVersion()) {
            parser.printVersion();
            return 0;
        }
        
        // Get configuration
        const auto& config = parser.getConfig();
        const auto& inputFile = parser.getInputFile();
        const auto& outputFile = parser.getOutputFile();
        
        // Initialize logger
        Logger::getInstance().setVerbose(config.verbose);
        Logger::getInstance().info("Starting LLVM Code Obfuscator v2.0");
        Logger::getInstance().info("Input: " + inputFile);
        Logger::getInstance().info("Output: " + outputFile);
        
        // Check if auto-tuning is enabled
        if (parser.isAutoTuneEnabled()) {
            Logger::getInstance().info("\n========================================");
            Logger::getInstance().info("     AUTO-TUNING MODE ACTIVATED");
            Logger::getInstance().info("========================================");
            Logger::getInstance().info("Iterations: " + std::to_string(parser.getAutoTuneIterations()));
            Logger::getInstance().info("Goal: " + parser.getAutoTuneGoal());
            Logger::getInstance().info("This will take some time...\n");
            
            // Create auto-tuner
            std::string baseConfigPath = "config/maximum_security.yaml"; // Use max security as base
            std::string outputDir = "auto_tune_results";
            
            AutoTuner tuner(baseConfigPath, inputFile, outputDir);
            tuner.setOptimizationGoal(parser.getAutoTuneGoal());
            
            // Run optimization
            auto bestCandidate = tuner.optimize(parser.getAutoTuneIterations());
            
            // Generate report
            tuner.generateReport(outputDir + "/optimization_report.md");
            
            Logger::getInstance().info("\n========================================");
            Logger::getInstance().info("     AUTO-TUNING COMPLETED!");
            Logger::getInstance().info("========================================");
            Logger::getInstance().info("Best RE Difficulty Score: " + 
                std::to_string(bestCandidate.metrics.reDifficultyScore) + "/100");
            Logger::getInstance().info("Best Security Score: " + 
                std::to_string(bestCandidate.metrics.securityScore) + "/100");
            Logger::getInstance().info("Optimized config saved to: " + 
                outputDir + "/final_optimized_config.yaml");
            Logger::getInstance().info("Optimized binary saved to: " + 
                outputDir + "/final_optimized_binary");
            Logger::getInstance().info("Full report: " + 
                outputDir + "/optimization_report.md");
            
            return 0;
        }
        
        // Normal obfuscation mode
        Logger::getInstance().info("Obfuscation Level: " + 
            std::string(config.level == ObfuscationLevel::LOW ? "LOW" :
                       config.level == ObfuscationLevel::MEDIUM ? "MEDIUM" : "HIGH"));
        
        // Create obfuscation engine
        ObfuscationEngine engine(config);
        
        // Process file
        if (!engine.processFile(inputFile, outputFile)) {
            Logger::getInstance().error("Obfuscation failed");
            return 1;
        }
        
        // Generate report
        auto reportGen = engine.getReportGenerator();
        if (config.generateMetrics) {
            std::string reportPath = config.reportPath;
            if (reportGen->generateReport(reportPath)) {
                Logger::getInstance().info("Report generated: " + reportPath);
            } else {
                Logger::getInstance().warning("Failed to generate report");
            }
            
            // Print summary
            reportGen->printSummary();
        }
        
        Logger::getInstance().info("Obfuscation completed successfully");
        std::cout << "Output written to: " << outputFile << "\n";
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred\n";
        return 1;
    }
}
