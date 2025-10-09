/**
 * @file ReportGenerator.cpp
 * @brief Implementation of ReportGenerator
 * @version 1.0.0
 * @date 2025-10-09
 */

#include "ReportGenerator.h"
#include "FileUtils.h"
#include "Logger.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <ctime>

namespace obfuscator {

std::string getCurrentTimestamp() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

ReportGenerator::ReportGenerator(const ObfuscationConfig& config)
    : config_(config) {
}

ReportGenerator::~ReportGenerator() {
}

void ReportGenerator::setMetricsCollector(std::shared_ptr<MetricsCollector> metrics) {
    metrics_ = metrics;
}

bool ReportGenerator::generateReport(const std::string& outputPath) {
    if (!metrics_) {
        Logger::getInstance().error("No metrics collector set for report generation");
        return false;
    }

    bool success = true;
    
    if (config_.reportFormat == "json" || config_.reportFormat == "both") {
        success &= generateJSONReport(outputPath + ".json");
    }
    
    if (config_.reportFormat == "html" || config_.reportFormat == "both") {
        success &= generateHTMLReport(outputPath + ".html");
    }
    
    return success;
}

bool ReportGenerator::generateJSONReport(const std::string& outputPath) {
    const auto& metrics = metrics_->getMetrics();
    
    std::ostringstream json;
    json << "{\n";
    json << "  \"obfuscation_report\": {\n";
    json << "    \"generated_at\": \"" << getCurrentTimestamp() << "\",\n";
    json << "    \"version\": \"1.0.0\",\n\n";
    
    // Input parameters
    json << "    \"input_parameters\": {\n";
    json << "      \"obfuscation_level\": \"" 
         << (config_.level == ObfuscationLevel::LOW ? "LOW" :
             config_.level == ObfuscationLevel::MEDIUM ? "MEDIUM" : "HIGH") << "\",\n";
    json << "      \"obfuscation_cycles\": " << config_.obfuscationCycles << ",\n";
    json << "      \"seed\": " << config_.seed << ",\n";
    json << "      \"target_platform\": \"" 
         << (config_.targetPlatform == TargetPlatform::LINUX_X86_64 ? "Linux x86_64" :
             config_.targetPlatform == TargetPlatform::WINDOWS_X86_64 ? "Windows x86_64" : "Other") << "\",\n";
    json << "      \"enabled_passes\": {\n";
    json << "        \"control_flow_flattening\": " 
         << (config_.enableControlFlowFlattening ? "true" : "false") << ",\n";
    json << "        \"opaque_predicates\": " 
         << (config_.enableOpaquePredicates ? "true" : "false") << ",\n";
    json << "        \"bogus_control_flow\": " 
         << (config_.enableBogusControlFlow ? "true" : "false") << ",\n";
    json << "        \"instruction_substitution\": " 
         << (config_.enableInstructionSubstitution ? "true" : "false") << ",\n";
    json << "        \"dead_code_injection\": " 
         << (config_.enableDeadCodeInjection ? "true" : "false") << ",\n";
    json << "        \"string_encryption\": " 
         << (config_.enableStringEncryption ? "true" : "false") << ",\n";
    json << "        \"constant_obfuscation\": " 
         << (config_.enableConstantObfuscation ? "true" : "false") << ",\n";
    json << "        \"function_virtualization\": " 
         << (config_.enableFunctionVirtualization ? "true" : "false") << ",\n";
    json << "        \"anti_debug\": " 
         << (config_.enableAntiDebug ? "true" : "false") << "\n";
    json << "      }\n";
    json << "    },\n\n";
    
    // File metrics
    json << "    \"file_metrics\": {\n";
    json << "      \"original_size_bytes\": " << metrics.originalFileSize << ",\n";
    json << "      \"obfuscated_size_bytes\": " << metrics.obfuscatedFileSize << ",\n";
    json << "      \"size_increase_percentage\": " << std::fixed << std::setprecision(2) 
         << metrics.sizeIncreasePercentage << "\n";
    json << "    },\n\n";
    
    // Code metrics
    json << "    \"code_metrics\": {\n";
    json << "      \"original_instructions\": " << metrics.originalInstructionCount << ",\n";
    json << "      \"obfuscated_instructions\": " << metrics.obfuscatedInstructionCount << ",\n";
    json << "      \"original_basic_blocks\": " << metrics.originalBasicBlockCount << ",\n";
    json << "      \"obfuscated_basic_blocks\": " << metrics.obfuscatedBasicBlockCount << ",\n";
    json << "      \"instruction_increase_percentage\": " << std::fixed << std::setprecision(2)
         << ((metrics.originalInstructionCount > 0) ?
             ((static_cast<double>(metrics.obfuscatedInstructionCount) - 
               static_cast<double>(metrics.originalInstructionCount)) / 
              static_cast<double>(metrics.originalInstructionCount) * 100.0) : 0.0) << "\n";
    json << "    },\n\n";
    
    // Transformation metrics
    json << "    \"transformation_metrics\": {\n";
    json << "      \"total_obfuscation_cycles\": " << config_.obfuscationCycles << ",\n";
    json << "      \"control_flow_transformations\": " << metrics.controlFlowTransformations << ",\n";
    json << "      \"instruction_substitutions\": " << metrics.instructionSubstitutions << ",\n";
    json << "      \"bogus_blocks_added\": " << metrics.bogusBlocksAdded << ",\n";
    json << "      \"opaque_predicates_added\": " << metrics.opaquePredicatesAdded << ",\n";
    json << "      \"dead_code_instructions_added\": " << metrics.deadCodeInstructionsAdded << ",\n";
    json << "      \"fake_loops_inserted\": " << metrics.fakeLoopsInserted << "\n";
    json << "    },\n\n";
    
    // String obfuscation
    json << "    \"string_obfuscation\": {\n";
    json << "      \"strings_encrypted\": " << metrics.stringsEncrypted << ",\n";
    json << "      \"original_strings_size_bytes\": " << metrics.stringsOriginalSize << ",\n";
    json << "      \"encrypted_strings_size_bytes\": " << metrics.stringsEncryptedSize << ",\n";
    json << "      \"encryption_algorithm\": \"" << config_.stringEncryptionAlgorithm << "\"\n";
    json << "    },\n\n";
    
    // Advanced obfuscation
    json << "    \"advanced_obfuscation\": {\n";
    json << "      \"functions_virtualized\": " << metrics.functionsVirtualized << ",\n";
    json << "      \"call_graph_transformations\": " << metrics.callGraphTransformations << ",\n";
    json << "      \"constants_obfuscated\": " << metrics.constantsObfuscated << ",\n";
    json << "      \"anti_debug_checks_added\": " << metrics.antiDebugChecksAdded << "\n";
    json << "    },\n\n";
    
    // Timing information
    json << "    \"timing_milliseconds\": {\n";
    json << "      \"compilation_time\": " << metrics.compilationTime.count() << ",\n";
    json << "      \"obfuscation_time\": " << metrics.obfuscationTime.count() << ",\n";
    json << "      \"linking_time\": " << metrics.linkingTime.count() << ",\n";
    json << "      \"total_time\": " << metrics.totalTime.count() << "\n";
    json << "    }\n";
    
    json << "  }\n";
    json << "}\n";
    
    return FileUtils::writeFile(outputPath, json.str());
}

bool ReportGenerator::generateHTMLReport(const std::string& outputPath) {
    const auto& metrics = metrics_->getMetrics();
    
    std::ostringstream html;
    html << "<!DOCTYPE html>\n";
    html << "<html lang=\"en\">\n";
    html << "<head>\n";
    html << "  <meta charset=\"UTF-8\">\n";
    html << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    html << "  <title>LLVM Obfuscation Report</title>\n";
    html << "  <style>\n";
    html << "    body { font-family: Arial, sans-serif; margin: 40px; background: #f5f5f5; }\n";
    html << "    .container { max-width: 1200px; margin: 0 auto; background: white; padding: 30px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }\n";
    html << "    h1 { color: #333; border-bottom: 3px solid #4CAF50; padding-bottom: 10px; }\n";
    html << "    h2 { color: #555; margin-top: 30px; border-bottom: 2px solid #ddd; padding-bottom: 8px; }\n";
    html << "    .metric-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(250px, 1fr)); gap: 20px; margin: 20px 0; }\n";
    html << "    .metric-card { background: #f9f9f9; padding: 15px; border-left: 4px solid #4CAF50; }\n";
    html << "    .metric-label { font-weight: bold; color: #666; font-size: 14px; }\n";
    html << "    .metric-value { font-size: 24px; color: #333; margin-top: 5px; }\n";
    html << "    .timestamp { color: #999; font-size: 14px; }\n";
    html << "    table { width: 100%; border-collapse: collapse; margin: 20px 0; }\n";
    html << "    th, td { padding: 12px; text-align: left; border-bottom: 1px solid #ddd; }\n";
    html << "    th { background: #4CAF50; color: white; }\n";
    html << "  </style>\n";
    html << "</head>\n";
    html << "<body>\n";
    html << "  <div class=\"container\">\n";
    html << "    <h1>LLVM Code Obfuscation Report</h1>\n";
    html << "    <p class=\"timestamp\">Generated: " << getCurrentTimestamp() << "</p>\n";
    
    html << "    <h2>File Metrics</h2>\n";
    html << "    <div class=\"metric-grid\">\n";
    html << "      <div class=\"metric-card\">\n";
    html << "        <div class=\"metric-label\">Original Size</div>\n";
    html << "        <div class=\"metric-value\">" << metrics.originalFileSize << " bytes</div>\n";
    html << "      </div>\n";
    html << "      <div class=\"metric-card\">\n";
    html << "        <div class=\"metric-label\">Obfuscated Size</div>\n";
    html << "        <div class=\"metric-value\">" << metrics.obfuscatedFileSize << " bytes</div>\n";
    html << "      </div>\n";
    html << "      <div class=\"metric-card\">\n";
    html << "        <div class=\"metric-label\">Size Increase</div>\n";
    html << "        <div class=\"metric-value\">" << std::fixed << std::setprecision(2) 
         << metrics.sizeIncreasePercentage << "%</div>\n";
    html << "      </div>\n";
    html << "    </div>\n";
    
    html << "    <h2>Transformation Summary</h2>\n";
    html << "    <div class=\"metric-grid\">\n";
    html << "      <div class=\"metric-card\">\n";
    html << "        <div class=\"metric-label\">Obfuscation Cycles</div>\n";
    html << "        <div class=\"metric-value\">" << config_.obfuscationCycles << "</div>\n";
    html << "      </div>\n";
    html << "      <div class=\"metric-card\">\n";
    html << "        <div class=\"metric-label\">Strings Encrypted</div>\n";
    html << "        <div class=\"metric-value\">" << metrics.stringsEncrypted << "</div>\n";
    html << "      </div>\n";
    html << "      <div class=\"metric-card\">\n";
    html << "        <div class=\"metric-label\">Bogus Blocks Added</div>\n";
    html << "        <div class=\"metric-value\">" << metrics.bogusBlocksAdded << "</div>\n";
    html << "      </div>\n";
    html << "      <div class=\"metric-card\">\n";
    html << "        <div class=\"metric-label\">Dead Code Instructions</div>\n";
    html << "        <div class=\"metric-value\">" << metrics.deadCodeInstructionsAdded << "</div>\n";
    html << "      </div>\n";
    html << "      <div class=\"metric-card\">\n";
    html << "        <div class=\"metric-label\">Opaque Predicates</div>\n";
    html << "        <div class=\"metric-value\">" << metrics.opaquePredicatesAdded << "</div>\n";
    html << "      </div>\n";
    html << "      <div class=\"metric-card\">\n";
    html << "        <div class=\"metric-label\">Fake Loops Inserted</div>\n";
    html << "        <div class=\"metric-value\">" << metrics.fakeLoopsInserted << "</div>\n";
    html << "      </div>\n";
    html << "    </div>\n";
    
    html << "    <h2>Performance Metrics</h2>\n";
    html << "    <table>\n";
    html << "      <tr><th>Phase</th><th>Time (ms)</th></tr>\n";
    html << "      <tr><td>Compilation</td><td>" << metrics.compilationTime.count() << "</td></tr>\n";
    html << "      <tr><td>Obfuscation</td><td>" << metrics.obfuscationTime.count() << "</td></tr>\n";
    html << "      <tr><td>Linking</td><td>" << metrics.linkingTime.count() << "</td></tr>\n";
    html << "      <tr><td><strong>Total</strong></td><td><strong>" << metrics.totalTime.count() << "</strong></td></tr>\n";
    html << "    </table>\n";
    
    html << "  </div>\n";
    html << "</body>\n";
    html << "</html>\n";
    
    return FileUtils::writeFile(outputPath, html.str());
}

void ReportGenerator::printSummary() const {
    if (!metrics_) {
        return;
    }
    
    const auto& metrics = metrics_->getMetrics();
    
    std::cout << "\n=== Obfuscation Summary ===\n";
    std::cout << "Original size: " << metrics.originalFileSize << " bytes\n";
    std::cout << "Obfuscated size: " << metrics.obfuscatedFileSize << " bytes\n";
    std::cout << "Size increase: " << std::fixed << std::setprecision(2) 
              << metrics.sizeIncreasePercentage << "%\n";
    std::cout << "Strings encrypted: " << metrics.stringsEncrypted << "\n";
    std::cout << "Obfuscation cycles: " << config_.obfuscationCycles << "\n";
    std::cout << "Total time: " << metrics.totalTime.count() << " ms\n";
    std::cout << "===========================\n\n";
}

std::string ReportGenerator::generateHeader() const {
    return "LLVM Code Obfuscator Report v1.0.0";
}

std::string ReportGenerator::generateConfigSection() const {
    return "Configuration section";
}

std::string ReportGenerator::generateMetricsSection() const {
    return "Metrics section";
}

} // namespace obfuscator
