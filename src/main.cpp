/**
 * @file main.cpp
 * @brief Main entry point for LLVM obfuscator
 * @version 1.0.0
 * @date 2025-10-09
 */

#include "ObfuscationEngine.h"
#include "CLIParser.h"
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
        Logger::getInstance().info("Starting LLVM Code Obfuscator");
        Logger::getInstance().info("Input: " + inputFile);
        Logger::getInstance().info("Output: " + outputFile);
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
