/**
 * @file OpaquePredicates.h
 * @brief Opaque predicate insertion obfuscation pass
 * @version 1.0.0
 * @date 2025-10-09
 * 
 * Inserts predicates whose outcome is known at obfuscation time but
 * difficult for static analysis to determine.
 */

#ifndef OPAQUE_PREDICATES_H
#define OPAQUE_PREDICATES_H

#include "ObfuscationPass.h"
#include "llvm/IR/IRBuilder.h"
#include <cstdint>

namespace obfuscator {

/**
 * @class OpaquePredicates
 * @brief Inserts opaque predicates to confuse analysis
 */
class OpaquePredicates : public ObfuscationPass {
public:
    /**
     * @brief Construct a new Opaque Predicates pass
     * @param count Number of predicates to insert
     */
    explicit OpaquePredicates(uint32_t count = 10);

    /**
     * @brief Run opaque predicate insertion on module
     * @param module LLVM module to transform
     * @param metrics Metrics collector
     * @return true if transformations were made
     */
    bool runOnModule(llvm::Module& module, MetricsCollector& metrics) override;

private:
    uint32_t count_;

    /**
     * @brief Insert opaque predicates into function
     * @param func Function to modify
     * @return Number of predicates inserted
     */
    uint32_t insertPredicates(llvm::Function& func);

    /**
     * @brief Create an opaque predicate expression
     * @param builder IR builder for insertion
     * @return Value representing opaque predicate
     */
    llvm::Value* createOpaquePredicate(llvm::IRBuilder<>& builder);
};

} // namespace obfuscator

#endif // OPAQUE_PREDICATES_H
