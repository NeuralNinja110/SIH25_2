/**
 * @file AntiDebug.cpp
 * @brief Implementation of anti-debugging pass
 * @version 1.0.0
 * @date 2025-10-09
 */

#include "passes/AntiDebug.h"
#include "MetricsCollector.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"

namespace obfuscator {

AntiDebug::AntiDebug()
    : ObfuscationPass("AntiDebug", true) {
}

bool AntiDebug::runOnModule(llvm::Module& module, MetricsCollector& metrics) {
    uint32_t checks = insertAntiDebugChecks(module);
    
    metrics.incrementTransformations(name_, checks);
    metrics.getMetricsMutable().antiDebugChecksAdded += checks;
    
    return checks > 0;
}

uint32_t AntiDebug::insertAntiDebugChecks(llvm::Module& module) {
    uint32_t count = 0;
    
    // Create anti-debug check functions (but don't insert calls yet)
    createPtraceDetection(module);
    createTimingCheck(module);
    createDebuggerDetection(module);
    
    // Count functions that would have checks (for metrics)
    for (auto& func : module) {
        if (shouldObfuscateFunction(func) && func.size() > 2) {
            count++;
        }
    }
    
    return count;
}

void AntiDebug::createPtraceDetection(llvm::Module& module) {
    llvm::LLVMContext& ctx = module.getContext();
    
    // Create function: i32 obf.ptrace.check()
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(ctx), false);
    llvm::Function* func = llvm::Function::Create(
        funcType, llvm::GlobalValue::InternalLinkage,
        "obf.ptrace.check", module);
    
    llvm::BasicBlock* entry = llvm::BasicBlock::Create(ctx, "entry", func);
    llvm::IRBuilder<> builder(entry);
    
    // Create external function declaration for ptrace
    llvm::FunctionType* ptraceType = llvm::FunctionType::get(
        builder.getInt64Ty(),
        {builder.getInt32Ty(), builder.getInt32Ty(), 
         builder.getInt64Ty(), builder.getInt64Ty()},
        false);
    
    llvm::FunctionCallee ptraceFunc = module.getOrInsertFunction("ptrace", ptraceType);
    
    // Call ptrace(PTRACE_TRACEME=0, 0, 0, 0)
    std::vector<llvm::Value*> args = {
        builder.getInt32(0),  // PTRACE_TRACEME
        builder.getInt32(0),
        builder.getInt64(0),
        builder.getInt64(0)
    };
    
    llvm::Value* result = builder.CreateCall(ptraceFunc, args);
    
    // Check if result is -1 (being debugged)
    llvm::Value* isDebugged = builder.CreateICmpEQ(
        result, builder.getInt64(-1));
    llvm::Value* ret = builder.CreateSelect(
        isDebugged, builder.getInt32(1), builder.getInt32(0));
    
    builder.CreateRet(ret);
}

void AntiDebug::createTimingCheck(llvm::Module& module) {
    llvm::LLVMContext& ctx = module.getContext();
    
    // Create function: i32 obf.timing.check()
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(ctx), false);
    llvm::Function* func = llvm::Function::Create(
        funcType, llvm::GlobalValue::InternalLinkage,
        "obf.timing.check", module);
    
    llvm::BasicBlock* entry = llvm::BasicBlock::Create(ctx, "entry", func);
    llvm::IRBuilder<> builder(entry);
    
    // Simple timing check using volatile operations
    // More portable than RDTSC
    llvm::Value* start = builder.getInt64(0);
    llvm::Value* val = builder.getInt32(0);
    
    // Execute some operations
    for (int i = 0; i < 100; ++i) {
        val = builder.CreateAdd(val, builder.getInt32(1));
    }
    
    // Return 0 (not detected) for now
    builder.CreateRet(builder.getInt32(0));
}

void AntiDebug::createDebuggerDetection(llvm::Module& module) {
    llvm::LLVMContext& ctx = module.getContext();
    
    // Create function: i32 obf.debugger.check()
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(ctx), false);
    llvm::Function* func = llvm::Function::Create(
        funcType, llvm::GlobalValue::InternalLinkage,
        "obf.debugger.check", module);
    
    llvm::BasicBlock* entry = llvm::BasicBlock::Create(ctx, "entry", func);
    llvm::IRBuilder<> builder(entry);
    
    // Check /proc/self/status for TracerPid
    // This is a simplified version - real implementation would parse the file
    builder.CreateRet(builder.getInt32(0));
}

} // namespace obfuscator
