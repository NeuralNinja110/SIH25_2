/**
 * @file RandomGenerator.h
 * @brief Random number generation utility
 * @version 1.0.0
 * @date 2025-10-09
 */

#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include <random>
#include <cstdint>

namespace obfuscator {

class RandomGenerator {
public:
    static RandomGenerator& getInstance();
    
    void seed(uint32_t seed);
    uint32_t getUInt32();
    uint32_t getUInt32(uint32_t min, uint32_t max);
    uint64_t getUInt64();
    bool getBool(uint32_t probability);
    double getDouble();

private:
    RandomGenerator();
    ~RandomGenerator() = default;
    RandomGenerator(const RandomGenerator&) = delete;
    RandomGenerator& operator=(const RandomGenerator&) = delete;

    std::mt19937 generator_;
};

} // namespace obfuscator

#endif // RANDOM_GENERATOR_H
