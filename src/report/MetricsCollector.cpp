/**
 * @file MetricsCollector.cpp
 * @brief Implementation of MetricsCollector
 * @version 1.0.0
 * @date 2025-10-09
 */

#include "MetricsCollector.h"

namespace obfuscator {

ObfuscationMetrics::ObfuscationMetrics()
    : originalFileSize(0),
      obfuscatedFileSize(0),
      sizeIncreasePercentage(0.0),
      originalInstructionCount(0),
      obfuscatedInstructionCount(0),
      originalBasicBlockCount(0),
      obfuscatedBasicBlockCount(0),
      originalFunctionCount(0),
      obfuscatedFunctionCount(0),
      totalObfuscationCycles(0),
      controlFlowTransformations(0),
      instructionSubstitutions(0),
      bogusBlocksAdded(0),
      opaquePredicatesAdded(0),
      deadCodeInstructionsAdded(0),
      stringsEncrypted(0),
      stringsOriginalSize(0),
      stringsEncryptedSize(0),
      functionsVirtualized(0),
      callGraphTransformations(0),
      constantsObfuscated(0),
      antiDebugChecksAdded(0),
      fakeLoopsInserted(0),
      compilationTime(0),
      obfuscationTime(0),
      linkingTime(0),
      totalTime(0) {
}

void ObfuscationMetrics::reset() {
    *this = ObfuscationMetrics();
}

MetricsCollector::MetricsCollector() {
}

MetricsCollector::~MetricsCollector() {
}

void MetricsCollector::recordFileSizes(uint64_t original, uint64_t obfuscated) {
    metrics_.originalFileSize = original;
    metrics_.obfuscatedFileSize = obfuscated;
    
    if (original > 0) {
        metrics_.sizeIncreasePercentage = 
            ((static_cast<double>(obfuscated) - static_cast<double>(original)) / 
             static_cast<double>(original)) * 100.0;
    }
}

void MetricsCollector::recordCodeMetrics(uint32_t originalInst, uint32_t obfuscatedInst,
                                        uint32_t originalBB, uint32_t obfuscatedBB) {
    metrics_.originalInstructionCount = originalInst;
    metrics_.obfuscatedInstructionCount = obfuscatedInst;
    metrics_.originalBasicBlockCount = originalBB;
    metrics_.obfuscatedBasicBlockCount = obfuscatedBB;
}

void MetricsCollector::incrementTransformations(const std::string& passName, uint32_t count) {
    metrics_.passTransformations[passName] += count;
}

void MetricsCollector::recordStringEncryption(uint32_t count, uint32_t originalSize, 
                                             uint32_t encryptedSize) {
    metrics_.stringsEncrypted += count;
    metrics_.stringsOriginalSize += originalSize;
    metrics_.stringsEncryptedSize += encryptedSize;
}

void MetricsCollector::recordTiming(const std::string& passName, 
                                   std::chrono::milliseconds duration) {
    metrics_.passTimings[passName] = duration;
    metrics_.totalTime += duration;
}

} // namespace obfuscator
