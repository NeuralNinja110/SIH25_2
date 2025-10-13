/**
 * @file StringEncryption.h
 * @brief String encryption obfuscation pass
 * @version 1.0.0
 * @date 2025-10-09
 */

#ifndef STRING_ENCRYPTION_H
#define STRING_ENCRYPTION_H

#include "ObfuscationPass.h"
#include <string>

namespace obfuscator {

class StringEncryption : public ObfuscationPass {
public:
    explicit StringEncryption(const std::string& algorithm = "xor");
    bool runOnModule(llvm::Module& module, MetricsCollector& metrics) override;

private:
    std::string algorithm_;
    uint32_t encryptStrings(llvm::Module& module);
    std::vector<uint8_t> encryptString(const std::string& str, uint8_t key);
    llvm::GlobalVariable* createEncryptedString(llvm::Module& module, 
                                                const std::string& str, uint8_t key);
    void createDecryptionStub(llvm::Module& module, llvm::GlobalVariable* encStr,
                             uint8_t key, size_t len);
};

} // namespace obfuscator

#endif // STRING_ENCRYPTION_H
