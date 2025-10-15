/**
 * @file test_main.cpp
 * @brief Main test runner
 * @version 1.0.0
 * @date 2025-10-09
 */

#include <iostream>
#include "ObfuscationEngine.h"
#include "FileUtils.h"

using namespace obfuscator;

bool runTest(const std::string& testName, const std::string& inputFile, ObfuscationLevel level) {
    std::cout << "\n=== Running Test: " << testName << " ===\n";
    
    ObfuscationConfig config;
    config.applyPreset(level);
    config.verbose = true;
    config.reportPath = "test_report_" + testName;
    
    ObfuscationEngine engine(config);
    
    std::string outputFile = inputFile + ".obf";
    
    if (!engine.processFile(inputFile, outputFile)) {
        std::cout << "❌ Test FAILED: " << testName << "\n";
        return false;
    }
    
    // Verify output exists
    if (!FileUtils::fileExists(outputFile)) {
        std::cout << "❌ Test FAILED: Output file not created\n";
        return false;
    }
    
    // Generate report
    auto reportGen = engine.getReportGenerator();
    reportGen->generateReport(config.reportPath);
    reportGen->printSummary();
    
    std::cout << "✓ Test PASSED: " << testName << "\n";
    return true;
}

int main() {
    std::cout << "LLVM Obfuscator Test Suite\n";
    std::cout << "===========================\n";
    
    int passed = 0;
    int total = 0;
    
    // Test 1: Easy - Simple Hello World
    total++;
    if (runTest("Easy_HelloWorld", "tests/test_easy.c", ObfuscationLevel::LOW)) {
        passed++;
    }
    
    // Test 2: Medium - Data Structures
    total++;
    if (runTest("Medium_DataStructures", "tests/test_medium.c", ObfuscationLevel::MEDIUM)) {
        passed++;
    }
    
    // Test 3: Difficult - Complex Algorithms
    total++;
    if (runTest("Difficult_Algorithms", "tests/test_difficult.c", ObfuscationLevel::HIGH)) {
        passed++;
    }
    
    std::cout << "\n=== Test Summary ===\n";
    std::cout << "Passed: " << passed << "/" << total << "\n";
    std::cout << "Failed: " << (total - passed) << "/" << total << "\n";
    
    return (passed == total) ? 0 : 1;
}
