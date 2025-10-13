# Phantron Obfuscator

[![npm version](https://img.shields.io/npm/v/@phantron/obfuscator.svg)](https://www.npmjs.com/package/@phantron/obfuscator)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Node.js Version](https://img.shields.io/node/v/@phantron/obfuscator.svg)](https://nodejs.org)

Professional code obfuscation CLI tool with LLVM integration, auto-tuning, multi-file support, and comprehensive HTML reporting.

## 🚀 Features

- ✅ **Real LLVM Obfuscation** - Not a simulation, uses actual LLVM transformations
- ✅ **Auto-Tuning** - Automatically optimizes obfuscation parameters for best results
- ✅ **Multi-File Support** - Handle multiple interdependent C/C++ files
- ✅ **Platform Support** - Target Linux or Windows binaries
- ✅ **Multiple Modes** - Normal, Security-Focused, or Size-Conservative
- ✅ **YAML Configuration** - Flexible parameter customization
- ✅ **HTML Reports** - Comprehensive obfuscation metrics and details
- ✅ **Interactive Mode** - User-friendly prompts for easy configuration
- ✅ **Professional Design** - Clean, modern CLI interface

## 📦 Installation

### Global Installation (Recommended)

```bash
npm install -g @phantron/obfuscator
```

### Local Installation

```bash
npm install @phantron/obfuscator
```

## 🎯 Quick Start

### 1. Check Prerequisites

```bash
phantron check
```

### 2. Basic Obfuscation

```bash
phantron obfuscate -i input.c -p linux -m normal
```

### 3. Security-Focused Obfuscation

```bash
phantron obfuscate -i app.cpp -p windows -m security
```

### 4. Auto-Tuning (5 iterations)

```bash
phantron obfuscate -i src.c -p linux -m security -a -n 5
```

### 5. Interactive Mode

```bash
phantron interactive
```

## 📖 Usage

### Command Line Options

```bash
phantron obfuscate [options]

Options:
  -i, --input <files>     Input C/C++ file(s) (comma-separated)
  -p, --platform <type>   Target platform: linux or windows
  -m, --mode <mode>       Obfuscation mode: normal, security, or size
  -a, --auto-tune         Enable auto-tuning
  -n, --iterations <n>    Number of auto-tune iterations (default: 5)
  -h, --help              Display help information
```

### Available Commands

```bash
phantron obfuscate      # Obfuscate files
phantron interactive    # Interactive mode
phantron check          # Check prerequisites
phantron examples       # Show usage examples
phantron --help         # Show help
phantron --version      # Show version
```

## 🎨 Obfuscation Modes

| Mode | Focus | Best For |
|------|-------|----------|
| **normal** | Balanced protection and performance | General use cases |
| **security** | Maximum protection (60% security weight) | Sensitive code, IP protection |
| **size** | Minimal size increase | Embedded systems, size constraints |

## 🔧 Auto-Tuning

Auto-tuning automatically finds the optimal obfuscation parameters:

1. Tests multiple parameter configurations
2. Measures security, size, and performance metrics
3. Calculates weighted fitness scores
4. Selects and applies the best configuration

```bash
# Run with 10 iterations for best results
phantron obfuscate -i main.c -p linux -m security -a -n 10
```

## 📊 Output

All results are saved to: `obfuscation_results/session_XXXXX/`

**Generated Files:**
- `*_obfuscated` or `*.exe` - Obfuscated binary
- `obfuscation_report.html` - Detailed HTML report
- `obfuscation_report.json` - JSON metrics
- `best_config.yaml` - Optimal configuration (auto-tune mode)
- `config_iterN.yaml` - Iteration configs (auto-tune mode)

## 📈 Example Session

```bash
$ phantron obfuscate -i factorial.c -p linux -m security -a -n 3

Obfuscation Configuration
─────────────────────────
┌─────────────────────────┬───────────────────────┐
│ Input Files             │ 1 file(s)             │
│ Platform                │ Linux                 │
│ Mode                    │ Security Focused      │
│ Auto-Tuning             │ Enabled (3 iterations)│
└─────────────────────────┴───────────────────────┘

[INFO] Starting auto-tuning with 3 iterations
[INFO] Iteration 1/3 - fitness score: 64.50
[SUCCESS] New best configuration found!
[INFO] Iteration 2/3 - fitness score: 65.20
[SUCCESS] New best configuration found!
[INFO] Iteration 3/3 - fitness score: 64.80

✔ Obfuscation completed successfully
✔ Compiled for Linux
✔ Metrics collected
✔ Report generated

╭────────────────────────────────────────────────╮
│   OBFUSCATION COMPLETED SUCCESSFULLY           │
│   Session ID: session_1760395143645            │
│   Output: obfuscation_results/session_...      │
╰────────────────────────────────────────────────╯
```

## 🛠️ Configuration

The tool uses YAML configuration files located in `config/`:

- `balanced.yaml` - Normal mode settings
- `maximum_security.yaml` - Security mode settings
- `size_conservative.yaml` - Size mode settings

You can customize these files to adjust:
- Obfuscation cycles
- Control flow flattening probability
- String encryption settings
- Bogus control flow density
- And more...

## 🔍 Obfuscation Techniques

- **Control Flow Flattening** - Obfuscates program flow structure
- **String Encryption** - Encrypts string literals at compile time
- **Instruction Substitution** - Replaces instructions with equivalents
- **Bogus Control Flow** - Inserts fake conditional branches
- **Function Inlining** - Prevents function identification
- **Variable Renaming** - Obscures variable names
- **Constant Obfuscation** - Hides constant values
- **Anti-Debugging** - Detects and prevents debugging

## 📋 Requirements

- **Node.js** >= 14.0.0
- **LLVM Obfuscator Binary** (included)
- **GCC** (for Linux targets)
- **MinGW** (for Windows targets on Linux)

## 🌟 Examples

### Multiple Files

```bash
phantron obfuscate -i "main.c,utils.c,helper.c" -p linux -m normal
```

### Windows Target

```bash
phantron obfuscate -i app.cpp -p windows -m security
```

### Size-Conservative Mode

```bash
phantron obfuscate -i embedded.c -p linux -m size
```

### Maximum Security with Auto-Tune

```bash
phantron obfuscate -i sensitive.c -p linux -m security -a -n 10
```

## 📚 Documentation

- [Complete Implementation Guide](CLI_COMPLETE_IMPLEMENTATION.md)
- [Quick Start Guide](PHANTRON_CLI_QUICKSTART.md)
- [Implementation Summary](PHANTRON_IMPLEMENTATION_SUMMARY.md)
- [User Documentation](PHANTRON_README.md)
- [Auto-Tuning Guide](docs/AUTO_TUNING.md)

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## 📄 License

MIT License - see [LICENSE](LICENSE) file for details

## 🔗 Links

- [GitHub Repository](https://github.com/NeuralNinja110/SIH25_2)
- [Issue Tracker](https://github.com/NeuralNinja110/SIH25_2/issues)
- [NPM Package](https://www.npmjs.com/package/@phantron/obfuscator)

## 💡 Support

For issues, questions, or feature requests:
- Open an issue on [GitHub](https://github.com/NeuralNinja110/SIH25_2/issues)
- Check the [documentation](PHANTRON_README.md)
- Review generated HTML reports for troubleshooting

## ⚡ Performance

**Benchmark Results:**
- RE Difficulty Score: 87.7/100
- Security Score: 75-95 (mode dependent)
- Obfuscation Time: ~3-15 seconds per file
- Binary Size Increase: 10-200% (mode dependent)

## 🎓 What Makes This Different?

Unlike typical obfuscation demos, Phantron performs **real obfuscation**:

✅ Calls actual LLVM obfuscator binary  
✅ Applies real code transformations  
✅ Generates working compiled binaries  
✅ Collects genuine metrics from output  
✅ Functional auto-tuning with optimization  

**This is production-ready, not a simulation!**

---

Made with ❤️ by the Phantron Team
