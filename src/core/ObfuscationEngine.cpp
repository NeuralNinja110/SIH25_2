/**
 * @file ObfuscationEngine.cpp
 * @brief Implementation of ObfuscationEngine
 * @version 1.0.0
 * @date 2025-10-09
 */

#include "ObfuscationEngine.h"
#include "PassManager.h"
#include "Logger.h"
#include "FileUtils.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/Verifier.h"
#include <chrono>
#include <cstdlib>
#include <sstream>

namespace obfuscator {

ObfuscationEngine::ObfuscationEngine(const ObfuscationConfig& config)
    : config_(config) {
    context_ = std::make_unique<llvm::LLVMContext>();
    passManager_ = std::make_unique<PassManager>(config_);
    reportGenerator_ = std::make_shared<ReportGenerator>(config_);
    
    Logger::getInstance().setVerbose(config_.verbose);
    Logger::getInstance().info("Obfuscation engine initialized");
}

ObfuscationEngine::~ObfuscationEngine() {
}

bool ObfuscationEngine::processFile(const std::string& inputFile, 
                                    const std::string& outputFile) {
    Logger::getInstance().info("Processing file: " + inputFile);
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Generate temporary IR file
    std::string irFile = inputFile + ".bc";
    std::string objectFile = outputFile + ".o";
    
    // Step 1: Compile to IR
    auto compileStart = std::chrono::high_resolution_clock::now();
    if (!compileToIR(inputFile, irFile)) {
        Logger::getInstance().error("Failed to compile source to IR");
        return false;
    }
    auto compileEnd = std::chrono::high_resolution_clock::now();
    
    // Step 2: Load module
    auto module = loadModule(irFile);
    if (!module) {
        Logger::getInstance().error("Failed to load LLVM module");
        return false;
    }
    
    // Step 3: Apply obfuscation
    auto obfStart = std::chrono::high_resolution_clock::now();
    if (!applyObfuscation(*module)) {
        Logger::getInstance().error("Failed to apply obfuscation");
        return false;
    }
    auto obfEnd = std::chrono::high_resolution_clock::now();
    
    // Step 4: Compile to object file
    if (!compileToObject(*module, objectFile)) {
        Logger::getInstance().error("Failed to compile to object file");
        return false;
    }
    
    // Step 5: Link to final binary
    auto linkStart = std::chrono::high_resolution_clock::now();
    if (!linkToBinary(objectFile, outputFile)) {
        Logger::getInstance().error("Failed to link binary");
        return false;
    }
    auto linkEnd = std::chrono::high_resolution_clock::now();
    
    auto endTime = std::chrono::high_resolution_clock::now();
    
    // Record timing metrics
    auto metrics = std::make_shared<MetricsCollector>();
    reportGenerator_->setMetricsCollector(metrics);
    if (metrics) {
        metrics->getMetricsMutable().compilationTime = 
            std::chrono::duration_cast<std::chrono::milliseconds>(compileEnd - compileStart);
        metrics->getMetricsMutable().obfuscationTime = 
            std::chrono::duration_cast<std::chrono::milliseconds>(obfEnd - obfStart);
        metrics->getMetricsMutable().linkingTime = 
            std::chrono::duration_cast<std::chrono::milliseconds>(linkEnd - linkStart);
        metrics->getMetricsMutable().totalTime = 
            std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        // Record file sizes
        metrics->recordFileSizes(
            FileUtils::getFileSize(inputFile),
            FileUtils::getFileSize(outputFile)
        );
    }
    
    // Clean up temporary files
    FileUtils::deleteFile(irFile);
    FileUtils::deleteFile(objectFile);
    
    Logger::getInstance().info("Obfuscation completed successfully");
    return true;
}

bool ObfuscationEngine::compileToIR(const std::string& sourceFile, 
                                    const std::string& irFile) {
    Logger::getInstance().info("Compiling source to LLVM IR");
    
    std::ostringstream cmd;
    
    // Determine if C or C++
    std::string ext = FileUtils::getFileExtension(sourceFile);
    bool isCpp = (ext == ".cpp" || ext == ".cxx" || ext == ".cc");
    
    // Build clang command
    cmd << (isCpp ? "clang++ " : "clang ");
    cmd << "-emit-llvm -c -O1 -fPIC ";
    cmd << "\"" << sourceFile << "\" ";
    cmd << "-o \"" << irFile << "\"";
    
    int result = std::system(cmd.str().c_str());
    return result == 0;
}

std::unique_ptr<llvm::Module> ObfuscationEngine::loadModule(const std::string& irFile) {
    Logger::getInstance().info("Loading LLVM module");
    
    llvm::SMDiagnostic err;
    auto module = llvm::parseIRFile(irFile, err, *context_);
    
    if (!module) {
        std::string errMsg = "Failed to load IR: " + err.getMessage().str();
        Logger::getInstance().error(errMsg);
        return nullptr;
    }
    
    return module;
}

bool ObfuscationEngine::applyObfuscation(llvm::Module& module) {
    Logger::getInstance().info("Applying obfuscation transformations");
    
    // Create metrics collector
    auto metrics = std::make_shared<MetricsCollector>();
    reportGenerator_->setMetricsCollector(metrics);
    
    // Count original code metrics
    uint32_t originalInsts = 0;
    uint32_t originalBBs = 0;
    uint32_t originalFuncs = 0;
    
    for (auto& func : module) {
        if (!func.isDeclaration()) {
            originalFuncs++;
            for (auto& bb : func) {
                originalBBs++;
                originalInsts += bb.size();
            }
        }
    }
    
    // Run obfuscation passes multiple times
    for (uint32_t cycle = 0; cycle < config_.obfuscationCycles; ++cycle) {
        Logger::getInstance().info("Running obfuscation cycle " + 
                                  std::to_string(cycle + 1) + "/" + 
                                  std::to_string(config_.obfuscationCycles));
        
        if (!passManager_->runPasses(module, *metrics)) {
            Logger::getInstance().warning("No transformations made in cycle " + 
                                        std::to_string(cycle + 1));
        }
    }
    
    metrics->getMetricsMutable().totalObfuscationCycles = config_.obfuscationCycles;
    
    // Count obfuscated code metrics
    uint32_t obfuscatedInsts = 0;
    uint32_t obfuscatedBBs = 0;
    uint32_t obfuscatedFuncs = 0;
    
    for (auto& func : module) {
        if (!func.isDeclaration()) {
            obfuscatedFuncs++;
            for (auto& bb : func) {
                obfuscatedBBs++;
                obfuscatedInsts += bb.size();
            }
        }
    }
    
    metrics->recordCodeMetrics(originalInsts, obfuscatedInsts, originalBBs, obfuscatedBBs);
    metrics->getMetricsMutable().originalFunctionCount = originalFuncs;
    metrics->getMetricsMutable().obfuscatedFunctionCount = obfuscatedFuncs;
    
    // Verify module integrity
    std::string errorMsg;
    llvm::raw_string_ostream errorStream(errorMsg);
    if (llvm::verifyModule(module, &errorStream)) {
        Logger::getInstance().error("Module verification failed: " + errorMsg);
        return false;
    }
    
    return true;
}

bool ObfuscationEngine::compileToObject(llvm::Module& module, 
                                       const std::string& objectFile) {
    Logger::getInstance().info("Compiling IR to object file");
    
    // Write module to temporary IR file
    std::string tempIR = objectFile + ".tmp.bc";
    std::error_code ec;
    llvm::raw_fd_ostream dest(tempIR, ec, llvm::sys::fs::OF_None);
    
    if (ec) {
        Logger::getInstance().error("Failed to open temporary file: " + ec.message());
        return false;
    }
    
    llvm::WriteBitcodeToFile(module, dest);
    dest.flush();
    
    // Compile IR to object file using llc
    std::ostringstream cmd;
    cmd << "llc -filetype=obj ";
    cmd << "\"" << tempIR << "\" ";
    cmd << "-o \"" << objectFile << "\"";
    
    int result = std::system(cmd.str().c_str());
    FileUtils::deleteFile(tempIR);
    
    return result == 0;
}

bool ObfuscationEngine::linkToBinary(const std::string& objectFile, 
                                     const std::string& binaryFile) {
    Logger::getInstance().info("Linking object file to binary");
    
    std::ostringstream cmd;
    
#ifdef _WIN32
    cmd << "link.exe ";
    cmd << "\"" << objectFile << "\" ";
    cmd << "/OUT:\"" << binaryFile << "\"";
#else
    cmd << "clang ";
    cmd << "-no-pie ";  // Disable PIE to avoid relocation issues
    cmd << "\"" << objectFile << "\" ";
    cmd << "-lm ";  // Link math library
    cmd << "-o \"" << binaryFile << "\"";
#endif
    
    int result = std::system(cmd.str().c_str());
    return result == 0;
}

std::shared_ptr<ReportGenerator> ObfuscationEngine::getReportGenerator() const {
    return reportGenerator_;
}

} // namespace obfuscator
