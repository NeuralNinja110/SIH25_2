# @maos/auto-tuner

<div align="center">

![MAOS Logo](https://img.shields.io/badge/MAOS-v2.0-blue?style=for-the-badge)
[![npm version](https://img.shields.io/npm/v/@maos/auto-tuner.svg?style=for-the-badge)](https://www.npmjs.com/package/@maos/auto-tuner)
[![License](https://img.shields.io/badge/license-MIT-green?style=for-the-badge)](LICENSE)
[![Downloads](https://img.shields.io/npm/dm/@maos/auto-tuner.svg?style=for-the-badge)](https://www.npmjs.com/package/@maos/auto-tuner)

**Intelligent Auto-Tuning System for MAOS Obfuscation Parameters**

*Automatically discover optimal security configurations through iterative optimization*

[Installation](#installation) • [Quick Start](#quick-start) • [Usage](#usage) • [Examples](#examples) • [Documentation](#documentation)

</div>

---

## 🚀 Features

- **🤖 Intelligent Optimization** - Automatically discovers best obfuscation parameters
- **📊 Real Metrics** - Uses actual reverse engineering difficulty scores
- **🎯 Three Optimization Goals** - Security, Balanced, or Size-focused
- **💡 Interactive Mode** - Guided setup for beginners
- **📈 Progress Tracking** - Beautiful CLI with real-time progress
- **📁 Reusable Configs** - Save and apply optimized configurations
- **🔍 Prerequisites Check** - Validates system requirements automatically

## 📋 Prerequisites

- **Node.js** >= 14.0.0
- **LLVM/Clang** >= 14.0
- **Python** >= 3.8
- **Linux** or **macOS** (x86_64 or ARM64)

## 📦 Installation

### Global Installation (Recommended)

```bash
npm install -g @maos/auto-tuner
```

### Local Installation

```bash
npm install @maos/auto-tuner
```

### From Source

```bash
git clone https://github.com/NeuralNinja110/SIH25_2.git
cd SIH25_2
npm install
npm link
```

## ⚡ Quick Start

### Interactive Mode (Recommended for Beginners)

```bash
maos-tune interactive
```

This will guide you through:
1. Selecting input file
2. Choosing optimization goal
3. Setting number of iterations
4. Confirming configuration

### Command-Line Mode

```bash
# Basic usage (5 iterations, balanced goal)
maos-tune run -i input.c

# Security-focused optimization
maos-tune run -i input.c -n 10 -g security

# Size-optimized for embedded systems
maos-tune run -i firmware.c -n 8 -g size
```

## 📖 Usage

### Commands

```bash
maos-tune run [options]          # Run auto-tuning optimization
maos-tune interactive            # Interactive mode with guided setup
maos-tune check                  # Check system prerequisites
maos-tune status                 # Show status of previous runs
maos-tune examples               # Show usage examples
maos-tune --help                 # Show help
maos-tune --version              # Show version
```

### Options for `run` Command

| Option | Alias | Description | Default |
|--------|-------|-------------|---------|
| `--input <file>` | `-i` | Input source file (C/C++) | *Required* |
| `--iterations <n>` | `-n` | Number of optimization iterations (1-50) | 5 |
| `--goal <goal>` | `-g` | Optimization goal: `security`, `balanced`, `size` | `balanced` |
| `--output <dir>` | `-o` | Output directory | `auto_tune_results` |
| `--verbose` | `-v` | Verbose output | `false` |

### Optimization Goals

#### 🔒 Security
- **Weight**: 80% security, 15% performance, 5% size
- **Best for**: IP protection, commercial software
- **Target**: 85-95/100 RE difficulty
- **Trade-off**: Higher compilation time and size

```bash
maos-tune run -i input.c -n 10 -g security
```

#### ⚖️ Balanced
- **Weight**: 60% security, 25% performance, 15% size
- **Best for**: General production use
- **Target**: 70-85/100 RE difficulty
- **Trade-off**: Good balance of all metrics

```bash
maos-tune run -i input.c -n 8 -g balanced
```

#### 📦 Size
- **Weight**: 50% security, 20% performance, 30% size
- **Best for**: Embedded systems, IoT devices
- **Target**: 60-75/100 RE difficulty
- **Trade-off**: Smaller binary, good security

```bash
maos-tune run -i embedded.c -n 8 -g size
```

## 📚 Examples

### Example 1: First-Time User

```bash
# Check prerequisites
maos-tune check

# Run interactive mode
maos-tune interactive
```

**Output:**
```
🎯 Interactive Auto-Tuning Setup

? Input source file: tests/test_easy.c
? Optimization goal: ⚖️  Balanced - Security + Performance (60/25/15%)
? Number of iterations: 5
? Output directory: auto_tune_results
? Start optimization? Yes

✓ Prerequisites check passed
```

### Example 2: Production Optimization

```bash
# Step 1: Optimize parameters (8 iterations, security-focused)
maos-tune run -i src/main.c -n 8 -g security -o prod_config

# Step 2: View results
maos-tune status -d prod_config

# Step 3: Apply to all production files
./build/llvm-obfuscator -c prod_config/final_optimized_config.yaml \
                         -i src/app.c -o release/app.obf
```

**Expected Output:**
```
🚀 Starting Optimization

✓ Baseline: RE 42.3/100 | Security 40.1/100 | Fitness 52.4/100 (8.2s)
✓ Iteration 1: RE 65.8/100 | Security 62.4/100 | Fitness 68.9/100 (9.1s)
  ★ New best configuration found!
✓ Iteration 2: RE 74.2/100 | Security 71.8/100 | Fitness 76.3/100 (8.9s)
  ★ New best configuration found!
...
✓ Iteration 8: RE 87.5/100 | Security 84.9/100 | Fitness 89.2/100 (9.5s)
  ★ New best configuration found!

✅ Auto-Tuning Completed Successfully!

Best Configuration:
  RE Difficulty:    87.5/100
  Security Score:   84.9/100
  Improvement:      +107.1%
```

### Example 3: Embedded System

```bash
# Size-focused optimization for resource-constrained device
maos-tune run -i firmware.c -n 8 -g size -o embedded_opt --verbose
```

### Example 4: Batch Processing

```bash
#!/bin/bash
# Optimize multiple files

for file in src/*.c; do
    echo "Optimizing $file..."
    maos-tune run -i "$file" -n 5 -g balanced -o "results/$(basename $file .c)"
done
```

## 📊 Output Files

After optimization, the following files are generated in the output directory:

```
auto_tune_results/
├── final_optimized_config.yaml    # ⭐ Best configuration (use this!)
├── final_optimized_binary         # Optimized binary
├── optimization_report.md         # Comprehensive analysis
├── config_iter0.yaml              # Baseline configuration
├── config_iter1_cand0.yaml        # Iteration 1, candidate 0
├── binary_iter1_cand0             # Iteration 1, candidate 0 binary
└── ...                            # More iterations
```

### Using the Optimized Configuration

```bash
# Apply best configuration to other files
./build/llvm-obfuscator \
    -c auto_tune_results/final_optimized_config.yaml \
    -i my_code.c \
    -o my_code_obf
```

## 🎯 How It Works

### Optimization Process

```
1. BASELINE (Iteration 0)
   ├─ Load default config (maximum_security.yaml)
   ├─ Obfuscate input file
   ├─ Run RE benchmark
   └─ Establish baseline metrics

2. ITERATIONS (1 to N)
   ├─ Generate 3-5 config variants (mutation)
   ├─ For each variant:
   │  ├─ Obfuscate input
   │  ├─ Run RE benchmark
   │  ├─ Calculate fitness score
   │  └─ Track if best
   └─ Adapt mutation strategy

3. FINAL RUN
   ├─ Use best configuration found
   ├─ Generate optimized binary
   └─ Create comprehensive report
```

### Evaluation Metrics

- **Primary Scores**: RE Difficulty, Security, Resilience (0-100)
- **Component Scores**: Complexity, CFG, Data Flow, Anti-Analysis, etc.
- **Performance**: Compilation time, binary size
- **Fitness**: Weighted combination based on goal

## 📈 Expected Results

| Iterations | RE Difficulty | Improvement | Time |
|------------|---------------|-------------|------|
| Baseline | 40-45/100 | - | - |
| 3 | 70-78/100 | +60-75% | 5-10 min |
| 5 | 75-82/100 | +75-95% | 10-20 min |
| 8 | 80-88/100 | +90-110% | 20-40 min |
| 15 | 85-92/100 | +100-120% | 40-80 min |

## 🔧 Configuration

### Custom Base Configuration

Create a custom base config and use it:

```bash
# Create custom config
cp config/maximum_security.yaml config/my_base.yaml
# Edit my_base.yaml with your preferences

# Use custom base (requires modifying source)
```

### Environment Variables

```bash
export MAOS_VERBOSE=1           # Enable verbose mode
export MAOS_ITERATIONS=10       # Default iterations
export MAOS_GOAL=security       # Default goal
```

## 🐛 Troubleshooting

### Issue: "MAOS obfuscator binary not found"

**Solution:**
```bash
cd /path/to/SIH25_2
make -C build llvm-obfuscator
```

### Issue: "RE benchmark framework not found"

**Solution:**
```bash
# Ensure reverse_engineering/scripts/re_framework.py exists
ls -l reverse_engineering/scripts/re_framework.py

# Install Python dependencies
pip install -r reverse_engineering/requirements.txt
```

### Issue: "Python 3 not found"

**Solution:**
```bash
# On Ubuntu/Debian
sudo apt-get install python3 python3-pip

# On macOS
brew install python3
```

### Issue: Node.js version too old

**Solution:**
```bash
# Install nvm
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.0/install.sh | bash

# Install Node.js 14+
nvm install 14
nvm use 14
```

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- LLVM Project for the amazing compiler infrastructure
- All contributors and users of MAOS

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/NeuralNinja110/SIH25_2/issues)
- **Discussions**: [GitHub Discussions](https://github.com/NeuralNinja110/SIH25_2/discussions)
- **Email**: support@maos.dev

## 🔗 Links

- **Homepage**: [https://github.com/NeuralNinja110/SIH25_2](https://github.com/NeuralNinja110/SIH25_2)
- **NPM Package**: [https://www.npmjs.com/package/@maos/auto-tuner](https://www.npmjs.com/package/@maos/auto-tuner)
- **Documentation**: [docs/AUTO_TUNING.md](docs/AUTO_TUNING.md)

---

<div align="center">

**Made with ❤️ by the MAOS Team**

[⬆ Back to Top](#maosauto-tuner)

</div>
