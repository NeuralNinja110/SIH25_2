/**
 * @file MetricsCollector.h
 * @brief Metrics collection for obfuscation process
 * @version 1.0.0
 * @date 2025-10-09
 */

#ifndef METRICS_COLLECTOR_H
#define METRICS_COLLECTOR_H

#include <cstdint>
#include <string>
#include <map>
#include <chrono>

namespace obfuscator {

/**
 * @struct ObfuscationMetrics
 * @brief Comprehensive metrics for obfuscation process
 */
struct ObfuscationMetrics {
    // File metrics
    uint64_t originalFileSize;
    uint64_t obfuscatedFileSize;
    double sizeIncreasePercentage;

    // Code metrics
    uint32_t originalInstructionCount;
    uint32_t obfuscatedInstructionCount;
    uint32_t originalBasicBlockCount;
    uint32_t obfuscatedBasicBlockCount;
    uint32_t originalFunctionCount;
    uint32_t obfuscatedFunctionCount;

    // Transformation metrics
    uint32_t totalObfuscationCycles;
    uint32_t controlFlowTransformations;
    uint32_t instructionSubstitutions;
    uint32_t bogusBlocksAdded;
    uint32_t opaquePredicatesAdded;
    uint32_t deadCodeInstructionsAdded;
    
    // String obfuscation
    uint32_t stringsEncrypted;
    uint32_t stringsOriginalSize;
    uint32_t stringsEncryptedSize;

    // Advanced metrics
    uint32_t functionsVirtualized;
    uint32_t callGraphTransformations;
    uint32_t constantsObfuscated;
    uint32_t antiDebugChecksAdded;
    uint32_t fakeLoopsInserted;

    // Timing metrics
    std::chrono::milliseconds compilationTime;
    std::chrono::milliseconds obfuscationTime;
    std::chrono::milliseconds linkingTime;
    std::chrono::milliseconds totalTime;

    // Per-pass metrics
    std::map<std::string, uint32_t> passTransformations;
    std::map<std::string, std::chrono::milliseconds> passTimings;

    /**
     * @brief Default constructor
     */
    ObfuscationMetrics();

    /**
     * @brief Reset all metrics to zero
     */
    void reset();
};

/**
 * @class MetricsCollector
 * @brief Collects and aggregates obfuscation metrics
 */
class MetricsCollector {
public:
    /**
     * @brief Construct a new Metrics Collector
     */
    MetricsCollector();

    /**
     * @brief Destroy the Metrics Collector
     */
    ~MetricsCollector();

    /**
     * @brief Record file size metrics
     * @param original Original file size
     * @param obfuscated Obfuscated file size
     */
    void recordFileSizes(uint64_t original, uint64_t obfuscated);

    /**
     * @brief Record code metrics
     * @param originalInst Original instruction count
     * @param obfuscatedInst Obfuscated instruction count
     * @param originalBB Original basic block count
     * @param obfuscatedBB Obfuscated basic block count
     */
    void recordCodeMetrics(uint32_t originalInst, uint32_t obfuscatedInst,
                          uint32_t originalBB, uint32_t obfuscatedBB);

    /**
     * @brief Increment transformation counter
     * @param passName Name of the pass
     * @param count Number of transformations
     */
    void incrementTransformations(const std::string& passName, uint32_t count);

    /**
     * @brief Record string encryption
     * @param count Number of strings encrypted
     * @param originalSize Original strings size
     * @param encryptedSize Encrypted strings size
     */
    void recordStringEncryption(uint32_t count, uint32_t originalSize, uint32_t encryptedSize);

    /**
     * @brief Record timing information
     * @param passName Name of the pass or phase
     * @param duration Duration in milliseconds
     */
    void recordTiming(const std::string& passName, std::chrono::milliseconds duration);

    /**
     * @brief Get current metrics
     * @return Reference to metrics structure
     */
    const ObfuscationMetrics& getMetrics() const { return metrics_; }

    /**
     * @brief Get mutable metrics reference
     * @return Mutable reference to metrics
     */
    ObfuscationMetrics& getMetricsMutable() { return metrics_; }

private:
    ObfuscationMetrics metrics_;
};

} // namespace obfuscator

#endif // METRICS_COLLECTOR_H
