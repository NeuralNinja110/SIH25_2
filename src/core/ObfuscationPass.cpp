/**
 * @file ObfuscationPass.cpp
 * @brief Implementation of ObfuscationPass base class
 * @version 1.0.0
 * @date 2025-10-09
 */

#include "ObfuscationPass.h"
#include "llvm/IR/Function.h"

namespace obfuscator {

ObfuscationPass::ObfuscationPass(const std::string& name, bool enabled)
    : name_(name), enabled_(enabled), seed_(0) {
}

bool ObfuscationPass::shouldObfuscateFunction(llvm::Function& func) const {
    // Skip declarations (no body)
    if (func.isDeclaration()) {
        return false;
    }
    
    // Skip intrinsic functions
    if (func.isIntrinsic()) {
        return false;
    }
    
    // Skip very small functions (< 3 basic blocks)
    if (func.size() < 3) {
        return false;
    }
    
    // Skip functions with specific annotations
    if (func.hasSection() && func.getSection().contains("noobf")) {
        return false;
    }
    
    return true;
}

} // namespace obfuscator
