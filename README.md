# Phantron Obfuscator# Phantron Obfuscator



Professional code obfuscation CLI tool with LLVM integration, auto-tuning capabilities, and comprehensive reporting.[![npm version](https://img.shields.io/npm/v/@phantron/obfuscator.svg)](https://www.npmjs.com/package/phantron-obfuscator)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Overview[![Node.js Version](https://img.shields.io/node/v/@phantron/obfuscator.svg)](https://nodejs.org)



Phantron Obfuscator is a production-ready command-line tool that provides advanced code obfuscation for C and C++ applications. Unlike simple text-based obfuscators, Phantron leverages LLVM compiler infrastructure to perform deep, semantic transformations that significantly increase reverse engineering difficulty while maintaining code functionality.Professional code obfuscation CLI tool with LLVM integration, auto-tuning, multi-file support, and comprehensive HTML reporting.



## Key Features## 🚀 Features



- **LLVM-Based Obfuscation** - Real compiler-level transformations, not just text manipulation- ✅ **Real LLVM Obfuscation** - Not a simulation, uses actual LLVM transformations

- **Auto-Tuning** - Automatically optimizes obfuscation parameters for best results- ✅ **Auto-Tuning** - Automatically optimizes obfuscation parameters for best results

- **Multi-File Support** - Process single files or entire projects with interdependent sources- ✅ **Multi-File Support** - Handle multiple interdependent C/C++ files

- **Cross-Platform** - Generate binaries for Linux and Windows- ✅ **Platform Support** - Target Linux or Windows binaries

- **Multiple Modes** - Choose between balanced, security-focused, or size-conservative obfuscation- ✅ **Multiple Modes** - Normal, Security-Focused, or Size-Conservative

- **YAML Configuration** - Flexible, customizable obfuscation parameters- ✅ **YAML Configuration** - Flexible parameter customization

- **Detailed Reporting** - HTML and JSON reports with comprehensive metrics- ✅ **HTML Reports** - Comprehensive obfuscation metrics and details

- **Interactive Mode** - User-friendly prompts for quick configuration- ✅ **Interactive Mode** - User-friendly prompts for easy configuration

- ✅ **Professional Design** - Clean, modern CLI interface

## Installation

## 📦 Installation

### Global Installation (Recommended)

### Global Installation (Recommended)

```bash

npm install -g phantron-obfuscator```bash

```npm install -g @phantron/obfuscator

```

### Local Installation

### Local Installation

```bash

npm install phantron-obfuscator```bash

```npm install @phantron/obfuscator

```

### Verify Installation

## 🎯 Quick Start

```bash

phantron --version### 1. Check Prerequisites

phantron check

``````bash

phantron check

For detailed installation instructions, see [INSTALLATION.md](https://github.com/NeuralNinja110/SIH25_2/blob/main/INSTALLATION.md).```



## Quick Start### 2. Basic Obfuscation



### Basic Obfuscation```bash

phantron obfuscate -i input.c -p linux -m normal

```bash```

phantron obfuscate -i input.c -p linux -m normal

```### 3. Security-Focused Obfuscation



### Security-Focused Obfuscation```bash

phantron obfuscate -i app.cpp -p windows -m security

```bash```

phantron obfuscate -i app.cpp -p windows -m security

```### 4. Auto-Tuning (5 iterations)



### Auto-Tuning (Recommended)```bash

phantron obfuscate -i src.c -p linux -m security -a -n 5

```bash```

phantron obfuscate -i src.c -p linux -m security -a -n 5

```### 5. Interactive Mode



### Interactive Mode```bash

phantron interactive

```bash```

phantron interactive

```## 📖 Usage



## Command Reference### Command Line Options



### Available Commands```bash

phantron obfuscate [options]

```bash

phantron obfuscate [options]  # Obfuscate source filesOptions:

phantron interactive          # Interactive mode with prompts  -i, --input <files>     Input C/C++ file(s) (comma-separated)

phantron check                # Check system prerequisites  -p, --platform <type>   Target platform: linux or windows

phantron examples             # Show usage examples  -m, --mode <mode>       Obfuscation mode: normal, security, or size

phantron --help               # Display help information  -a, --auto-tune         Enable auto-tuning

phantron --version            # Show version number  -n, --iterations <n>    Number of auto-tune iterations (default: 5)

```  -h, --help              Display help information

```

### Obfuscate Command Options

### Available Commands

| Option | Description | Values |

|--------|-------------|--------|```bash

| `-i, --input <files>` | Input C/C++ file(s) | Single file or comma-separated list |phantron obfuscate      # Obfuscate files

| `-p, --platform <type>` | Target platform | `linux` or `windows` |phantron interactive    # Interactive mode

| `-m, --mode <mode>` | Obfuscation mode | `normal`, `security`, or `size` |phantron check          # Check prerequisites

| `-a, --auto-tune` | Enable auto-tuning | Flag (no value) |phantron examples       # Show usage examples

| `-n, --iterations <n>` | Auto-tune iterations | Number (default: 5) |phantron --help         # Show help

phantron --version      # Show version

For complete API reference, see [API_REFERENCE.md](https://github.com/NeuralNinja110/SIH25_2/blob/main/API_REFERENCE.md).```



## Obfuscation Modes## 🎨 Obfuscation Modes



### Normal Mode (Balanced)| Mode | Focus | Best For |

- Focus: 45% security, 35% performance, 20% size|------|-------|----------|

- Best for: General-purpose applications| **normal** | Balanced protection and performance | General use cases |

- Config: `config/balanced.yaml`| **security** | Maximum protection (60% security weight) | Sensitive code, IP protection |

| **size** | Minimal size increase | Embedded systems, size constraints |

### Security Mode (Maximum Protection)

- Focus: 60% security, 30% performance, 10% size  ## 🔧 Auto-Tuning

- Best for: Sensitive algorithms, IP protection

- Config: `config/maximum_security.yaml`Auto-tuning automatically finds the optimal obfuscation parameters:



### Size Mode (Size-Conservative)1. Tests multiple parameter configurations

- Focus: 50% security, 20% performance, 30% size2. Measures security, size, and performance metrics

- Best for: Embedded systems, size-constrained environments3. Calculates weighted fitness scores

- Config: `config/size_conservative.yaml`4. Selects and applies the best configuration



## Auto-Tuning```bash

# Run with 10 iterations for best results

Auto-tuning automatically tests multiple parameter configurations to find optimal obfuscation settings:phantron obfuscate -i main.c -p linux -m security -a -n 10

```

1. Generates parameter variations based on the selected mode

2. Runs obfuscation with each configuration## 📊 Output

3. Measures security, size, and performance metrics

4. Calculates weighted fitness scoresAll results are saved to: `obfuscation_results/session_XXXXX/`

5. Selects and applies the best configuration

**Generated Files:**

```bash- `*_obfuscated` or `*.exe` - Obfuscated binary

# Run auto-tuning with 10 iterations for best results- `obfuscation_report.html` - Detailed HTML report

phantron obfuscate -i main.c -p linux -m security -a -n 10- `obfuscation_report.json` - JSON metrics

```- `best_config.yaml` - Optimal configuration (auto-tune mode)

- `config_iterN.yaml` - Iteration configs (auto-tune mode)

## Output

## 📈 Example Session

All obfuscation results are saved to `obfuscation_results/session_<timestamp>/`

```bash

### Generated Files$ phantron obfuscate -i factorial.c -p linux -m security -a -n 3



- **Obfuscated Binary** - `*_obfuscated` (Linux) or `*.exe` (Windows)Obfuscation Configuration

- **HTML Report** - `obfuscation_report.html` with detailed metrics─────────────────────────

- **JSON Report** - `obfuscation_report.json` for programmatic access┌─────────────────────────┬───────────────────────┐

- **Configuration Files** - `best_config.yaml` and iteration configs (auto-tune mode)│ Input Files             │ 1 file(s)             │

│ Platform                │ Linux                 │

### Report Contents│ Mode                    │ Security Focused      │

│ Auto-Tuning             │ Enabled (3 iterations)│

- Input file details and checksums└─────────────────────────┴───────────────────────┘

- Output file attributes and sizes

- Obfuscation cycle count[INFO] Starting auto-tuning with 3 iterations

- String encryption statistics[INFO] Iteration 1/3 - fitness score: 64.50

- Fake function injection count[SUCCESS] New best configuration found!

- Bogus code line count[INFO] Iteration 2/3 - fitness score: 65.20

- Applied transformation techniques[SUCCESS] New best configuration found!

- Security and RE difficulty scores[INFO] Iteration 3/3 - fitness score: 64.80



## Obfuscation Techniques✔ Obfuscation completed successfully

✔ Compiled for Linux

Phantron applies multiple layers of obfuscation:✔ Metrics collected

✔ Report generated

- **Control Flow Flattening** - Restructures program flow to obscure logic

- **String Encryption** - Encrypts string literals with runtime decryption╭────────────────────────────────────────────────╮

- **Instruction Substitution** - Replaces instructions with equivalent alternatives│   OBFUSCATION COMPLETED SUCCESSFULLY           │

- **Bogus Control Flow** - Inserts fake conditional branches│   Session ID: session_1760395143645            │

- **Function Inlining** - Removes function boundaries│   Output: obfuscation_results/session_...      │

- **Variable Renaming** - Obscures variable identifiers╰────────────────────────────────────────────────╯

- **Constant Obfuscation** - Hides and encrypts constant values```

- **Anti-Debugging** - Detects and prevents debugging attempts

## 🛠️ Configuration

## Examples

The tool uses YAML configuration files located in `config/`:

### Multiple Files

- `balanced.yaml` - Normal mode settings

```bash- `maximum_security.yaml` - Security mode settings

phantron obfuscate -i "main.c,utils.c,helper.c" -p linux -m normal- `size_conservative.yaml` - Size mode settings

```

You can customize these files to adjust:

### Windows Target from Linux- Obfuscation cycles

- Control flow flattening probability

```bash- String encryption settings

phantron obfuscate -i app.cpp -p windows -m security- Bogus control flow density

```- And more...



### Size-Conservative for Embedded Systems## 🔍 Obfuscation Techniques



```bash- **Control Flow Flattening** - Obfuscates program flow structure

phantron obfuscate -i firmware.c -p linux -m size- **String Encryption** - Encrypts string literals at compile time

```- **Instruction Substitution** - Replaces instructions with equivalents

- **Bogus Control Flow** - Inserts fake conditional branches

### Maximum Security with Auto-Tuning- **Function Inlining** - Prevents function identification

- **Variable Renaming** - Obscures variable names

```bash- **Constant Obfuscation** - Hides constant values

phantron obfuscate -i sensitive.c -p linux -m security -a -n 10- **Anti-Debugging** - Detects and prevents debugging

```

## 📋 Requirements

## Requirements

- **Node.js** >= 14.0.0

- **Node.js** >= 14.0.0- **LLVM Obfuscator Binary** (included)

- **LLVM Obfuscator Binary** (included in package)- **GCC** (for Linux targets)

- **GCC** (for Linux targets)- **MinGW** (for Windows targets on Linux)

- **MinGW** (for Windows cross-compilation on Linux)

## 🌟 Examples

Run `phantron check` to verify all prerequisites are met.

### Multiple Files

## Configuration

```bash

Obfuscation behavior is controlled by YAML configuration files in the `config/` directory:phantron obfuscate -i "main.c,utils.c,helper.c" -p linux -m normal

```

- `balanced.yaml` - Normal mode settings

- `maximum_security.yaml` - Security mode settings  ### Windows Target

- `size_conservative.yaml` - Size mode settings

```bash

You can customize these files or create your own configurations. Key parameters include:phantron obfuscate -i app.cpp -p windows -m security

```

- Obfuscation cycles

- Control flow flattening probability### Size-Conservative Mode

- String encryption settings

- Bogus control flow density```bash

- Anti-debugging featuresphantron obfuscate -i embedded.c -p linux -m size

- Function virtualization options```



## Performance Benchmarks### Maximum Security with Auto-Tune



Based on extensive testing:```bash

phantron obfuscate -i sensitive.c -p linux -m security -a -n 10

- **RE Difficulty Score**: 87.7/100```

- **Security Score**: 75-95 (mode dependent)

- **Obfuscation Time**: 3-15 seconds per file## 📚 Documentation

- **Binary Size Increase**: 10-200% (mode dependent)

- **Runtime Overhead**: Minimal to moderate (transformation dependent)- [Complete Implementation Guide](CLI_COMPLETE_IMPLEMENTATION.md)

- [Quick Start Guide](PHANTRON_CLI_QUICKSTART.md)

## Documentation- [Implementation Summary](PHANTRON_IMPLEMENTATION_SUMMARY.md)

- [User Documentation](PHANTRON_README.md)

- [INSTALLATION.md](https://github.com/NeuralNinja110/SIH25_2/blob/main/INSTALLATION.md) - Detailed installation and setup guide- [Auto-Tuning Guide](docs/AUTO_TUNING.md)

- [API_REFERENCE.md](https://github.com/NeuralNinja110/SIH25_2/blob/main/API_REFERENCE.md) - Complete API and usage reference

## 🤝 Contributing

## Support

Contributions are welcome! Please feel free to submit a Pull Request.

### Getting Help

## 📄 License

- **GitHub Issues**: [Report bugs or request features](https://github.com/NeuralNinja110/SIH25_2/issues)

- **Documentation**: Review the guides in this repositoryMIT License - see [LICENSE](LICENSE) file for details

- **Examples**: Run `phantron examples` for common usage patterns

## 🔗 Links

### Troubleshooting

- [GitHub Repository](https://github.com/NeuralNinja110/SIH25_2)

Run the prerequisite checker:- [Issue Tracker](https://github.com/NeuralNinja110/SIH25_2/issues)

```bash- [NPM Package](https://www.npmjs.com/package/@phantron/obfuscator)

phantron check

```## 💡 Support



This will verify:For issues, questions, or feature requests:

- Node.js version compatibility- Open an issue on [GitHub](https://github.com/NeuralNinja110/SIH25_2/issues)

- LLVM obfuscator binary availability- Check the [documentation](PHANTRON_README.md)

- Compiler availability (GCC/MinGW)- Review generated HTML reports for troubleshooting

- Configuration file integrity

## ⚡ Performance

## Contributing

**Benchmark Results:**

Contributions are welcome. Please:- RE Difficulty Score: 87.7/100

- Security Score: 75-95 (mode dependent)

1. Fork the repository- Obfuscation Time: ~3-15 seconds per file

2. Create a feature branch- Binary Size Increase: 10-200% (mode dependent)

3. Make your changes with tests

4. Submit a pull request## 🎓 What Makes This Different?



## LicenseUnlike typical obfuscation demos, Phantron performs **real obfuscation**:



MIT License - see [LICENSE](https://github.com/NeuralNinja110/SIH25_2/blob/main/LICENSE) file for details.✅ Calls actual LLVM obfuscator binary  

✅ Applies real code transformations  

## Links✅ Generates working compiled binaries  

✅ Collects genuine metrics from output  

- **NPM Package**: [https://www.npmjs.com/package/phantron-obfuscator](https://www.npmjs.com/package/phantron-obfuscator)✅ Functional auto-tuning with optimization  

- **GitHub Repository**: [https://github.com/NeuralNinja110/SIH25_2](https://github.com/NeuralNinja110/SIH25_2)

- **Issue Tracker**: [https://github.com/NeuralNinja110/SIH25_2/issues](https://github.com/NeuralNinja110/SIH25_2/issues)**This is production-ready, not a simulation!**



## Acknowledgments---



Built with LLVM compiler infrastructure and modern Node.js tooling.Made with ❤️ by the Phantron Team


---

**Version**: 2.0.0  
**Author**: Phantron Team  
**Maintainer**: neuralninja110
