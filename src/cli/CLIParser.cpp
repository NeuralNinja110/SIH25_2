/**
 * @file CLIParser.cpp
 * @brief Implementation of CLIParser
 * @version 1.0.0
 * @date 2025-10-09
 */

#include "CLIParser.h"
#include "ConfigParser.h"
#include <iostream>
#include <cstring>

namespace obfuscator {

CLIParser::CLIParser() : showHelp_(false), showVersion_(false), 
                         autoTuneEnabled_(false), autoTuneIterations_(5),
                         autoTuneGoal_("balanced") {
    setDefaults();
}

CLIParser::~CLIParser() {
}

void CLIParser::setDefaults() {
    config_ = ObfuscationConfig();
    config_.applyPreset(ObfuscationLevel::MEDIUM);
}

bool CLIParser::parse(int argc, char* argv[]) {
    if (argc < 2) {
        showHelp_ = true;
        return false;
    }
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            showHelp_ = true;
            return true;
        }
        
        if (arg == "-v" || arg == "--version") {
            showVersion_ = true;
            return true;
        }
        
        if (arg == "-i" || arg == "--input") {
            if (i + 1 < argc) {
                inputFile_ = argv[++i];
            }
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                outputFile_ = argv[++i];
            }
        } else if (arg == "-l" || arg == "--level") {
            if (i + 1 < argc) {
                std::string level = argv[++i];
                if (level == "0" || level == "low") {
                    config_.applyPreset(ObfuscationLevel::LOW);
                } else if (level == "1" || level == "medium") {
                    config_.applyPreset(ObfuscationLevel::MEDIUM);
                } else if (level == "2" || level == "high") {
                    config_.applyPreset(ObfuscationLevel::HIGH);
                } else {
                    int levelNum = std::stoi(level);
                    if (levelNum <= 0) {
                        config_.applyPreset(ObfuscationLevel::LOW);
                    } else if (levelNum == 1) {
                        config_.applyPreset(ObfuscationLevel::MEDIUM);
                    } else {
                        config_.applyPreset(ObfuscationLevel::HIGH);
                    }
                }
            }
        } else if (arg == "-C" || arg == "--complexity") {
            if (i + 1 < argc) {
                int complexity = std::stoi(argv[++i]);
                // Use complexity to adjust obfuscation intensity
                config_.flatteningComplexity = std::min(100, std::max(10, complexity * 30));
                config_.constantObfuscationComplexity = std::min(100, std::max(10, complexity * 30));
                config_.obfuscationCycles = std::max(1, complexity);
            }
        } else if (arg == "-c" || arg == "--config") {
            if (i + 1 < argc) {
                configFile_ = argv[++i];
                // Parse config file here
                ConfigParser parser;
                if (!parser.parseFile(configFile_, config_)) {
                    std::cerr << "Warning: Failed to parse config file, using defaults\n";
                }
            }
        } else if (arg == "--cycles") {
            if (i + 1 < argc) {
                config_.obfuscationCycles = std::stoi(argv[++i]);
            }
        } else if (arg == "--seed") {
            if (i + 1 < argc) {
                config_.seed = std::stoul(argv[++i]);
            }
        } else if (arg == "--verbose") {
            config_.verbose = true;
        } else if (arg == "--no-flatten") {
            config_.enableControlFlowFlattening = false;
        } else if (arg == "--no-strings") {
            config_.enableStringEncryption = false;
        } else if (arg == "--no-constants") {
            config_.enableConstantObfuscation = false;
        } else if (arg == "--enable-virtualization") {
            config_.enableFunctionVirtualization = true;
        } else if (arg == "--enable-anti-debug") {
            config_.enableAntiDebug = true;
        } else if (arg == "--report") {
            if (i + 1 < argc) {
                config_.reportPath = argv[++i];
            }
        } else if (arg == "--report-format") {
            if (i + 1 < argc) {
                config_.reportFormat = argv[++i];
            }
        } else if (arg == "--auto-tune") {
            autoTuneEnabled_ = true;
            if (i + 1 < argc) {
                std::string next = argv[i + 1];
                if (next == "yes" || next == "true" || next == "enable") {
                    ++i;
                }
            }
        } else if (arg == "--auto-tune-iterations") {
            if (i + 1 < argc) {
                autoTuneIterations_ = std::stoi(argv[++i]);
                if (autoTuneIterations_ < 1) autoTuneIterations_ = 1;
                if (autoTuneIterations_ > 50) autoTuneIterations_ = 50;
            }
        } else if (arg == "--auto-tune-goal") {
            if (i + 1 < argc) {
                autoTuneGoal_ = argv[++i];
                if (autoTuneGoal_ != "security" && 
                    autoTuneGoal_ != "balanced" && 
                    autoTuneGoal_ != "size") {
                    std::cerr << "Warning: Invalid auto-tune goal '" << autoTuneGoal_ 
                              << "', using 'balanced'\n";
                    autoTuneGoal_ = "balanced";
                }
            }
        } else if (inputFile_.empty()) {
            inputFile_ = arg;
        } else if (outputFile_.empty()) {
            outputFile_ = arg;
        }
    }
    
    if (inputFile_.empty()) {
        std::cerr << "Error: No input file specified\n";
        return false;
    }
    
    if (outputFile_.empty()) {
        outputFile_ = inputFile_ + ".obf";
    }
    
    return config_.validate();
}

void CLIParser::printHelp() const {
    std::cout << "LLVM Code Obfuscator v1.0.0\n\n";
    std::cout << "Usage: llvm-obfuscator [options] <input-file> [output-file]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help                 Show this help message\n";
    std::cout << "  -v, --version              Show version information\n";
    std::cout << "  -i, --input <file>         Input source file (C/C++)\n";
    std::cout << "  -o, --output <file>        Output obfuscated binary\n";
    std::cout << "  -l, --level <level>        Obfuscation level: low, medium, high (default: medium)\n";
    std::cout << "  -c, --config <file>        Load configuration from YAML file\n";
    std::cout << "  --cycles <n>               Number of obfuscation cycles (default: 3)\n";
    std::cout << "  --seed <n>                 Random seed for reproducibility\n";
    std::cout << "  --verbose                  Enable verbose output\n";
    std::cout << "\nAuto-Tuning Options:\n";
    std::cout << "  --auto-tune                Enable automatic parameter optimization\n";
    std::cout << "  --auto-tune-iterations <n> Number of optimization iterations (1-50, default: 5)\n";
    std::cout << "  --auto-tune-goal <goal>    Optimization goal:\n";
    std::cout << "                               security  - Maximize RE difficulty (80% weight)\n";
    std::cout << "                               balanced  - Balance security/performance (60/25/15%)\n";
    std::cout << "                               size      - Minimize size with good security (50/20/30%)\n";
    std::cout << "\nObfuscation Options:\n";
    std::cout << "  --no-flatten               Disable control flow flattening\n";
    std::cout << "  --no-strings               Disable string encryption\n";
    std::cout << "  --no-constants             Disable constant obfuscation\n";
    std::cout << "  --enable-virtualization    Enable function virtualization\n";
    std::cout << "  --enable-anti-debug        Enable anti-debugging features\n";
    std::cout << "\nReport Options:\n";
    std::cout << "  --report <path>            Report output path (default: obfuscation_report)\n";
    std::cout << "  --report-format <format>   Report format: json, html, both (default: json)\n";
    std::cout << "\nExamples:\n";
    std::cout << "  # Basic obfuscation\n";
    std::cout << "  llvm-obfuscator input.c output\n\n";
    std::cout << "  # High security with auto-tuning (5 iterations)\n";
    std::cout << "  llvm-obfuscator --auto-tune --auto-tune-iterations 5 input.c\n\n";
    std::cout << "  # Auto-tune for maximum security (10 iterations)\n";
    std::cout << "  llvm-obfuscator --auto-tune --auto-tune-iterations 10 \\\n";
    std::cout << "                  --auto-tune-goal security input.cpp output\n\n";
    std::cout << "  # Manual high-level obfuscation\n";
    std::cout << "  llvm-obfuscator -l high --cycles 5 input.cpp output\n\n";
    std::cout << "  # Load config and auto-tune\n";
    std::cout << "  llvm-obfuscator -c config.yaml --auto-tune --auto-tune-iterations 8 input.c\n\n";
}

void CLIParser::printVersion() const {
    std::cout << "LLVM Code Obfuscator v1.0.0\n";
    std::cout << "Advanced code obfuscation using LLVM infrastructure\n";
    std::cout << "Copyright (c) 2025\n";
}

} // namespace obfuscator
