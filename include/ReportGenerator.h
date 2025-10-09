/**
 * @file ReportGenerator.h
 * @brief Report generation for obfuscation metrics and statistics
 * @version 1.0.0
 * @date 2025-10-09
 */

#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include <string>
#include <memory>
#include "ObfuscationConfig.h"
#include "MetricsCollector.h"

namespace obfuscator {

/**
 * @class ReportGenerator
 * @brief Generates comprehensive reports of obfuscation process
 */
class ReportGenerator {
public:
    /**
     * @brief Construct a new Report Generator
     * @param config Obfuscation configuration
     */
    explicit ReportGenerator(const ObfuscationConfig& config);

    /**
     * @brief Destroy the Report Generator
     */
    ~ReportGenerator();

    /**
     * @brief Set the metrics collector
     * @param metrics Shared pointer to metrics collector
     */
    void setMetricsCollector(std::shared_ptr<MetricsCollector> metrics);

    /**
     * @brief Generate report in specified format
     * @param outputPath Path to output report file
     * @return true if report generation successful
     */
    bool generateReport(const std::string& outputPath);

    /**
     * @brief Generate JSON format report
     * @param outputPath Path to output JSON file
     * @return true if generation successful
     */
    bool generateJSONReport(const std::string& outputPath);

    /**
     * @brief Generate HTML format report
     * @param outputPath Path to output HTML file
     * @return true if generation successful
     */
    bool generateHTMLReport(const std::string& outputPath);

    /**
     * @brief Print summary to console
     */
    void printSummary() const;

private:
    ObfuscationConfig config_;
    std::shared_ptr<MetricsCollector> metrics_;

    /**
     * @brief Generate report header section
     * @return Header string
     */
    std::string generateHeader() const;

    /**
     * @brief Generate configuration section
     * @return Configuration string
     */
    std::string generateConfigSection() const;

    /**
     * @brief Generate metrics section
     * @return Metrics string
     */
    std::string generateMetricsSection() const;
};

} // namespace obfuscator

#endif // REPORT_GENERATOR_H
