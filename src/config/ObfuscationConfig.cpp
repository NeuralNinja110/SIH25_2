/**
 * @file ObfuscationConfig.cpp
 * @brief Implementation of ObfuscationConfig
 * @version 1.0.0
 * @date 2025-10-09
 */

#include "ObfuscationConfig.h"
#include <ctime>

namespace obfuscator {

ObfuscationConfig::ObfuscationConfig()
    : level(ObfuscationLevel::MEDIUM),
      targetPlatform(TargetPlatform::LINUX_X86_64),
      obfuscationCycles(3),
      seed(static_cast<uint32_t>(std::time(nullptr))),
      verbose(false),
      enableControlFlowFlattening(true),
      flatteningComplexity(50),
      enableOpaquePredicates(true),
      opaquePredicateCount(10),
      enableBogusControlFlow(true),
      bogusBlockProbability(30),
      enableInstructionSubstitution(true),
      substitutionProbability(50),
      enableDeadCodeInjection(true),
      deadCodeRatio(20),
      enableStringEncryption(true),
      stringEncryptionAlgorithm("xor"),
      enableConstantObfuscation(true),
      constantObfuscationComplexity(50),
      enableFunctionVirtualization(false),
      virtualizationThreshold(50),
      enableCallGraphObfuscation(false),
      enableAntiDebug(false),
      enableAntiTamper(false),
      reportFormat("json"),
      reportPath("obfuscation_report"),
      generateMetrics(true) {
}

void ObfuscationConfig::applyPreset(ObfuscationLevel preset) {
    level = preset;
    
    switch (preset) {
        case ObfuscationLevel::LOW:
            obfuscationCycles = 1;
            enableControlFlowFlattening = false;
            flatteningComplexity = 20;
            enableOpaquePredicates = false;  // Temporarily disabled
            opaquePredicateCount = 5;
            enableBogusControlFlow = false;
            bogusBlockProbability = 10;
            enableInstructionSubstitution = true;
            substitutionProbability = 30;
            enableDeadCodeInjection = false;
            deadCodeRatio = 10;
            enableStringEncryption = true;
            enableConstantObfuscation = false;
            enableFunctionVirtualization = false;
            enableCallGraphObfuscation = false;
            enableAntiDebug = false;
            break;
            
        case ObfuscationLevel::MEDIUM:
            obfuscationCycles = 2;  // Reduced from 3
            enableControlFlowFlattening = false;  // Temporarily disabled
            flatteningComplexity = 50;
            enableOpaquePredicates = false;  // Temporarily disabled
            opaquePredicateCount = 10;
            enableBogusControlFlow = false;  // Temporarily disabled
            bogusBlockProbability = 30;
            enableInstructionSubstitution = true;
            substitutionProbability = 50;
            enableDeadCodeInjection = true;
            deadCodeRatio = 20;
            enableStringEncryption = true;
            enableConstantObfuscation = true;
            constantObfuscationComplexity = 50;
            enableFunctionVirtualization = false;
            enableCallGraphObfuscation = false;
            enableAntiDebug = false;
            break;
            
        case ObfuscationLevel::HIGH:
            obfuscationCycles = 5;
            enableControlFlowFlattening = true;
            flatteningComplexity = 80;
            enableOpaquePredicates = true;
            opaquePredicateCount = 20;
            enableBogusControlFlow = true;
            bogusBlockProbability = 50;
            enableInstructionSubstitution = true;
            substitutionProbability = 70;
            enableDeadCodeInjection = true;
            deadCodeRatio = 40;
            enableStringEncryption = true;
            enableConstantObfuscation = true;
            constantObfuscationComplexity = 80;
            enableFunctionVirtualization = true;
            virtualizationThreshold = 30;
            enableCallGraphObfuscation = true;
            enableAntiDebug = true;
            break;
    }
}

bool ObfuscationConfig::validate() const {
    if (obfuscationCycles == 0 || obfuscationCycles > 10) {
        return false;
    }
    
    if (flatteningComplexity > 100) {
        return false;
    }
    
    if (bogusBlockProbability > 100 || substitutionProbability > 100 || 
        deadCodeRatio > 100) {
        return false;
    }
    
    if (stringEncryptionAlgorithm != "xor" && 
        stringEncryptionAlgorithm != "aes" && 
        stringEncryptionAlgorithm != "custom") {
        return false;
    }
    
    if (reportFormat != "json" && reportFormat != "html" && reportFormat != "both") {
        return false;
    }
    
    return true;
}

} // namespace obfuscator
