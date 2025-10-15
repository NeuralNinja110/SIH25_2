/**
 * @file test_obfuscation.cpp
 * @brief Unit tests for obfuscation passes
 * @version 1.0.0
 * @date 2025-10-09
 */

#include <iostream>
#include <cassert>
#include "ObfuscationConfig.h"
#include "MetricsCollector.h"
#include "RandomGenerator.h"

using namespace obfuscator;

void testObfuscationConfig() {
    std::cout << "Testing ObfuscationConfig... ";
    
    ObfuscationConfig config;
    assert(config.validate());
    
    config.applyPreset(ObfuscationLevel::LOW);
    assert(config.obfuscationCycles == 1);
    
    config.applyPreset(ObfuscationLevel::HIGH);
    assert(config.obfuscationCycles == 5);
    
    std::cout << "✓\n";
}

void testMetricsCollector() {
    std::cout << "Testing MetricsCollector... ";
    
    MetricsCollector metrics;
    
    metrics.recordFileSizes(1000, 1500);
    assert(metrics.getMetrics().originalFileSize == 1000);
    assert(metrics.getMetrics().obfuscatedFileSize == 1500);
    
    metrics.incrementTransformations("TestPass", 10);
    assert(metrics.getMetrics().passTransformations["TestPass"] == 10);
    
    std::cout << "✓\n";
}

void testRandomGenerator() {
    std::cout << "Testing RandomGenerator... ";
    
    auto& rng = RandomGenerator::getInstance();
    rng.seed(12345);
    
    uint32_t val1 = rng.getUInt32();
    rng.seed(12345);
    uint32_t val2 = rng.getUInt32();
    
    // With same seed, should get same value
    assert(val1 == val2);
    
    uint32_t ranged = rng.getUInt32(0, 100);
    assert(ranged <= 100);
    
    std::cout << "✓\n";
}

int main() {
    std::cout << "Running unit tests...\n\n";
    
    try {
        testObfuscationConfig();
        testMetricsCollector();
        testRandomGenerator();
        
        std::cout << "\n✓ All unit tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Test failed: " << e.what() << "\n";
        return 1;
    }
}
