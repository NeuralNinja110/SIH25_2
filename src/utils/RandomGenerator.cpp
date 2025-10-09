/**
 * @file RandomGenerator.cpp
 * @brief Implementation of RandomGenerator
 * @version 1.0.0
 * @date 2025-10-09
 */

#include "RandomGenerator.h"
#include <ctime>

namespace obfuscator {

RandomGenerator::RandomGenerator() {
    generator_.seed(static_cast<uint32_t>(std::time(nullptr)));
}

RandomGenerator& RandomGenerator::getInstance() {
    static RandomGenerator instance;
    return instance;
}

void RandomGenerator::seed(uint32_t seed) {
    generator_.seed(seed);
}

uint32_t RandomGenerator::getUInt32() {
    return generator_();
}

uint32_t RandomGenerator::getUInt32(uint32_t min, uint32_t max) {
    std::uniform_int_distribution<uint32_t> dist(min, max);
    return dist(generator_);
}

uint64_t RandomGenerator::getUInt64() {
    uint64_t high = static_cast<uint64_t>(generator_()) << 32;
    uint64_t low = static_cast<uint64_t>(generator_());
    return high | low;
}

bool RandomGenerator::getBool(uint32_t probability) {
    return getUInt32(0, 99) < probability;
}

double RandomGenerator::getDouble() {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(generator_);
}

} // namespace obfuscator
