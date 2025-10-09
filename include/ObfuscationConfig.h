/**
 * @file ObfuscationConfig.h
 * @brief Configuration structure for obfuscation parameters
 * @version 1.0.0
 * @date 2025-10-09
 */

#ifndef OBFUSCATION_CONFIG_H
#define OBFUSCATION_CONFIG_H

#include <string>
#include <cstdint>

namespace obfuscator {

/**
 * @enum ObfuscationLevel
 * @brief Predefined obfuscation intensity levels
 */
enum class ObfuscationLevel {
    LOW,      ///< Basic obfuscation with minimal overhead
    MEDIUM,   ///< Balanced obfuscation with moderate overhead
    HIGH      ///< Maximum obfuscation with high overhead
};

/**
 * @enum TargetPlatform
 * @brief Target platform for binary generation
 */
enum class TargetPlatform {
    LINUX_X86_64,
    LINUX_ARM64,
    WINDOWS_X86_64,
    WINDOWS_ARM64
};

/**
 * @struct ObfuscationConfig
 * @brief Configuration parameters for obfuscation process
 */
struct ObfuscationConfig {
    // General settings
    ObfuscationLevel level;
    TargetPlatform targetPlatform;
    uint32_t obfuscationCycles;
    uint32_t seed;
    bool verbose;

    // Control flow obfuscation
    bool enableControlFlowFlattening;
    uint32_t flatteningComplexity;
    
    bool enableOpaquePredicates;
    uint32_t opaquePredicateCount;
    
    bool enableBogusControlFlow;
    uint32_t bogusBlockProbability;  // Percentage (0-100)
    
    // Instruction level obfuscation
    bool enableInstructionSubstitution;
    uint32_t substitutionProbability;  // Percentage (0-100)
    
    bool enableDeadCodeInjection;
    uint32_t deadCodeRatio;  // Percentage (0-100)
    
    // Data obfuscation
    bool enableStringEncryption;
    std::string stringEncryptionAlgorithm;  // "xor", "aes", "custom"
    
    bool enableConstantObfuscation;
    uint32_t constantObfuscationComplexity;
    
    // Advanced features
    bool enableFunctionVirtualization;
    uint32_t virtualizationThreshold;  // Min instruction count for virtualization
    
    bool enableCallGraphObfuscation;
    bool enableAntiDebug;
    bool enableAntiTamper;
    
    // Output settings
    std::string reportFormat;  // "json", "html", "both"
    std::string reportPath;
    bool generateMetrics;

    /**
     * @brief Default constructor with safe defaults
     */
    ObfuscationConfig();

    /**
     * @brief Apply preset configuration based on level
     * @param preset Obfuscation level preset
     */
    void applyPreset(ObfuscationLevel preset);

    /**
     * @brief Validate configuration parameters
     * @return true if configuration is valid
     */
    bool validate() const;
};

} // namespace obfuscator

#endif // OBFUSCATION_CONFIG_H
