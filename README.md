# Phantron Obfuscator

[![npm version](https://img.shields.io/npm/v/phantron-obfuscator.svg)](https://www.npmjs.com/package/phantron-obfuscator)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Node.js Version](https://img.shields.io/node/v/phantron-obfuscator.svg)](https://nodejs.org)

Professional code obfuscation CLI tool with LLVM integration, auto-tuning capabilities, and comprehensive reporting.

## Overview

Phantron Obfuscator is a production-ready command-line tool that provides advanced code obfuscation for C and C++ applications. Unlike simple text-based obfuscators, Phantron leverages LLVM compiler infrastructure to perform deep, semantic transformations that significantly increase reverse engineering difficulty while maintaining code functionality.

## Key Features

- **LLVM-Based Obfuscation** - Real compiler-level transformations, not just text manipulation
- **Auto-Tuning** - Automatically optimizes obfuscation parameters for best results
- **Multi-File Support** - Process single files or entire projects with interdependent sources
- **Cross-Platform** - Generate binaries for Linux and Windows
- **Multiple Modes** - Choose between balanced, security-focused, or size-conservative obfuscation
- **YAML Configuration** - Flexible, customizable obfuscation parameters
- **Detailed Reporting** - HTML and JSON reports with comprehensive metrics
- **Interactive Mode** - User-friendly prompts for quick configuration

## Installation

### Global Installation (Recommended)

```bash
npm install -g phantron-obfuscator
```

### Local Installation

```bash
npm install phantron-obfuscator
```

### Verify Installation

```bash
phantron --version
phantron check
```

For detailed installation instructions, see [INSTALLATION.md](https://github.com/NeuralNinja110/SIH25_2/blob/main/INSTALLATION.md).

## Quick Start

### Basic Obfuscation

```bash
phantron obfuscate -i input.c -p linux -m normal
```

### Security-Focused Obfuscation

```bash
phantron obfuscate -i app.cpp -p windows -m security
```

### Auto-Tuning (Recommended)

```bash
phantron obfuscate -i src.c -p linux -m security -a -n 5
```

### Interactive Mode

```bash
phantron interactive
```

## Command Reference

### Available Commands

```bash
phantron obfuscate [options]  # Obfuscate source files
phantron interactive          # Interactive mode with prompts
phantron check                # Check system prerequisites
phantron examples             # Show usage examples
phantron --help               # Display help information
phantron --version            # Show version number
```

### Obfuscate Command Options

| Option | Description | Values |
|--------|-------------|--------|
| `-i, --input <files>` | Input C/C++ file(s) | Single file or comma-separated list |
| `-p, --platform <type>` | Target platform | `linux` or `windows` |
| `-m, --mode <mode>` | Obfuscation mode | `normal`, `security`, or `size` |
| `-a, --auto-tune` | Enable auto-tuning | Flag (no value) |
| `-n, --iterations <n>` | Auto-tune iterations | Number (default: 5) |

For complete API reference, see [API_REFERENCE.md](https://github.com/NeuralNinja110/SIH25_2/blob/main/API_REFERENCE.md).

## Obfuscation Modes

### Normal Mode (Balanced)
- Focus: 45% security, 35% performance, 20% size
- Best for: General-purpose applications
- Config: `config/balanced.yaml`

### Security Mode (Maximum Protection)
- Focus: 60% security, 30% performance, 10% size  
- Best for: Sensitive algorithms, IP protection
- Config: `config/maximum_security.yaml`

### Size Mode (Size-Conservative)
- Focus: 50% security, 20% performance, 30% size
- Best for: Embedded systems, size-constrained environments
- Config: `config/size_conservative.yaml`

## Auto-Tuning

Auto-tuning automatically tests multiple parameter configurations to find optimal obfuscation settings:

1. Generates parameter variations based on the selected mode
2. Runs obfuscation with each configuration
3. Measures security, size, and performance metrics
4. Calculates weighted fitness scores
5. Selects and applies the best configuration

```bash
# Run auto-tuning with 10 iterations for best results
phantron obfuscate -i main.c -p linux -m security -a -n 10
```

## Output

All obfuscation results are saved to `obfuscation_results/session_<timestamp>/`

### Generated Files

- **Obfuscated Binary** - `*_obfuscated` (Linux) or `*.exe` (Windows)
- **HTML Report** - `obfuscation_report.html` with detailed metrics
- **JSON Report** - `obfuscation_report.json` for programmatic access
- **Configuration Files** - `best_config.yaml` and iteration configs (auto-tune mode)

### Report Contents

- Input file details and checksums
- Output file attributes and sizes
- Obfuscation cycle count
- String encryption statistics
- Fake function injection count
- Bogus code line count
- Applied transformation techniques
- Security and RE difficulty scores

## Obfuscation Techniques

Phantron applies multiple layers of obfuscation:

- **Control Flow Flattening** - Restructures program flow to obscure logic
- **String Encryption** - Encrypts string literals with runtime decryption
- **Instruction Substitution** - Replaces instructions with equivalent alternatives
- **Bogus Control Flow** - Inserts fake conditional branches
- **Function Inlining** - Removes function boundaries
- **Variable Renaming** - Obscures variable identifiers
- **Constant Obfuscation** - Hides and encrypts constant values
- **Anti-Debugging** - Detects and prevents debugging attempts

## Examples

### Multiple Files

```bash
phantron obfuscate -i "main.c,utils.c,helper.c" -p linux -m normal
```

### Windows Target from Linux

```bash
phantron obfuscate -i app.cpp -p windows -m security
```

### Size-Conservative for Embedded Systems

```bash
phantron obfuscate -i firmware.c -p linux -m size
```

### Maximum Security with Auto-Tuning

```bash
phantron obfuscate -i sensitive.c -p linux -m security -a -n 10
```

## Requirements

- **Node.js** >= 14.0.0
- **LLVM Obfuscator Binary** (included in package)
- **GCC** (for Linux targets)
- **MinGW** (for Windows cross-compilation on Linux)

Run `phantron check` to verify all prerequisites are met.

## Configuration

Obfuscation behavior is controlled by YAML configuration files in the `config/` directory:

- `balanced.yaml` - Normal mode settings
- `maximum_security.yaml` - Security mode settings
- `size_conservative.yaml` - Size mode settings

You can customize these files or create your own configurations. Key parameters include:

- Obfuscation cycles
- Control flow flattening probability
- String encryption settings
- Bogus control flow density
- Anti-debugging features
- Function virtualization options

## Performance Benchmarks

Based on extensive testing:

- **RE Difficulty Score**: 87.7/100
- **Security Score**: 75-95 (mode dependent)
- **Obfuscation Time**: 3-15 seconds per file
- **Binary Size Increase**: 10-200% (mode dependent)
- **Runtime Overhead**: Minimal to moderate (transformation dependent)

## Documentation

- [INSTALLATION.md](https://github.com/NeuralNinja110/SIH25_2/blob/main/INSTALLATION.md) - Detailed installation and setup guide
- [API_REFERENCE.md](https://github.com/NeuralNinja110/SIH25_2/blob/main/API_REFERENCE.md) - Complete API and usage reference

## Support

### Getting Help

- **GitHub Issues**: [Report bugs or request features](https://github.com/NeuralNinja110/SIH25_2/issues)
- **Documentation**: Review the guides in this repository
- **Examples**: Run `phantron examples` for common usage patterns

### Troubleshooting

Run the prerequisite checker:

```bash
phantron check
```

This will verify:
- Node.js version compatibility
- LLVM obfuscator binary availability
- Compiler availability (GCC/MinGW)
- Configuration file integrity

## Contributing

Contributions are welcome. Please:

1. Fork the repository
2. Create a feature branch
3. Make your changes with tests
4. Submit a pull request

## License

MIT License - see [LICENSE](https://github.com/NeuralNinja110/SIH25_2/blob/main/LICENSE) file for details.

## Links

- **NPM Package**: [https://www.npmjs.com/package/phantron-obfuscator](https://www.npmjs.com/package/phantron-obfuscator)
- **GitHub Repository**: [https://github.com/NeuralNinja110/SIH25_2](https://github.com/NeuralNinja110/SIH25_2)
- **Issue Tracker**: [https://github.com/NeuralNinja110/SIH25_2/issues](https://github.com/NeuralNinja110/SIH25_2/issues)

## Acknowledgments

Built with LLVM compiler infrastructure and modern Node.js tooling.

---

**Version**: 2.0.1  
**Author**: Phantron Team  
**Maintainer**: neuralninja110
