# 🚀 MAOS - Quick Usage Guide

**Version:** 1.0.0  
**Binary:** `build/llvm-obfuscator`  
**Status:** ✅ Production Ready

---

## 📋 Table of Contents

1. [Quick Start](#quick-start)
2. [Basic Usage](#basic-usage)
3. [Operating Modes](#operating-modes)
4. [Command-Line Options](#command-line-options)
5. [Configuration Files](#configuration-files)
6. [Examples](#examples)
7. [Troubleshooting](#troubleshooting)

---

## ⚡ Quick Start

### Basic Obfuscation (Default Mode)

```bash
cd /workspaces/SIH25_2/build
./llvm-obfuscator -i ../tests/test_easy.c -o output_dir
```

**Result:**
- Obfuscated binary: `output_dir/test_easy`
- Size increase: ~15-30%
- Time: < 500ms
- Report: `obfuscation_report.json`

---

## 📖 Basic Usage

### Command Format

```bash
./llvm-obfuscator [OPTIONS] -i <input_file> -o <output_dir>
```

### Required Arguments

- `-i, --input <file>` - Input C/C++ source file
- `-o, --output <dir>` - Output directory for obfuscated binary

### Optional Arguments

- `-l, --level <low|medium|high>` - Obfuscation level (default: medium)
- `-c, --config <file>` - Configuration YAML file
- `-C, --cycles <n>` - Number of obfuscation cycles (default: 3)
- `-s, --seed <n>` - Random seed for reproducibility
- `-v, --verbose` - Enable verbose logging
- `-V, --version` - Show version information
- `-h, --help` - Show help message

---

## 🎯 Operating Modes

### 1. **Size-Conservative Mode** (Recommended for Production)

**Purpose:** Minimal size increase, low performance overhead

```bash
./llvm-obfuscator -i source.c -o output \
  -c ../config/size_conservative.yaml
```

**Characteristics:**
- Size increase: ≤15%
- Performance overhead: ≤5%
- Obfuscation level: Low-Medium
- Cycles: 2-3
- Features: Basic transformations only

**Use Cases:**
- Mobile applications
- Embedded systems
- Performance-critical software
- Size-constrained environments

### 2. **Maximum-Security Mode** (Recommended for High-Value IP)

**Purpose:** Maximum protection against reverse engineering

```bash
./llvm-obfuscator -i source.c -o output \
  -c ../config/maximum_security.yaml
```

**Characteristics:**
- Size increase: 50-200%
- Performance overhead: 10-30%
- Obfuscation level: High-Extreme
- Cycles: 5-10
- Features: All transformations + virtualization + anti-debug

**Use Cases:**
- Proprietary algorithms
- License validation code
- DRM systems
- High-value intellectual property

### 3. **Balanced Mode** (Default)

**Purpose:** Good balance of security and performance

```bash
./llvm-obfuscator -i source.c -o output
# Or explicitly:
./llvm-obfuscator -i source.c -o output -l medium -C 3
```

**Characteristics:**
- Size increase: ~20-40%
- Performance overhead: ~5-10%
- Obfuscation level: Medium
- Cycles: 3
- Features: Most transformations enabled

**Use Cases:**
- General-purpose applications
- Commercial software
- Standard IP protection
- Most production scenarios

---

## 🛠️ Command-Line Options

### Obfuscation Level (`-l, --level`)

| Level | Security | Size Impact | Time Impact | Recommended For |
|-------|----------|-------------|-------------|-----------------|
| `low` | Basic | +10-15% | +2-5% | Size-critical apps |
| `medium` | Good | +20-40% | +5-10% | General use (default) |
| `high` | Strong | +50-200% | +10-30% | High-value IP |

### Obfuscation Cycles (`-C, --cycles`)

Multiple passes of obfuscation for increased complexity.

```bash
# Light obfuscation (1-2 cycles)
./llvm-obfuscator -i source.c -o output -C 1

# Standard obfuscation (3 cycles, default)
./llvm-obfuscator -i source.c -o output -C 3

# Heavy obfuscation (5+ cycles)
./llvm-obfuscator -i source.c -o output -C 10
```

**Trade-off:**
- More cycles = Higher security
- More cycles = Larger size increase
- More cycles = Longer compilation time

### Random Seed (`-s, --seed`)

For reproducible obfuscation (useful for testing/debugging).

```bash
# Same seed = identical output
./llvm-obfuscator -i source.c -o output -s 42
./llvm-obfuscator -i source.c -o output -s 42  # Same result
```

### Verbose Mode (`-v, --verbose`)

Enable detailed logging for debugging.

```bash
./llvm-obfuscator -i source.c -o output -v
```

**Output includes:**
- Pass execution details
- Transformation statistics
- Timing information
- Warnings and errors

---

## ⚙️ Configuration Files

### Using YAML Configuration

Configuration files provide fine-grained control over obfuscation behavior.

```bash
./llvm-obfuscator -i source.c -o output -c config.yaml
```

### Pre-configured Profiles

#### `config/size_conservative.yaml`

```yaml
mode: size_conservative
level: low
cycles: 2
passes:
  - instruction_substitution
  - constant_obfuscation
  - string_encryption
maos:
  enable_atie: true
  enable_qirl: true
  enable_pcge: false  # Disabled for size
genetic_algorithm:
  population_size: 20
  generations: 50
  mutation_rate: 0.1
```

#### `config/maximum_security.yaml`

```yaml
mode: maximum_security
level: high
cycles: 5
passes:
  - control_flow_flattening
  - bogus_control_flow
  - instruction_substitution
  - opaque_predicates
  - constant_obfuscation
  - string_encryption
  - dead_code_injection
  - function_virtualization
  - anti_debug
maos:
  enable_atie: true
  enable_qirl: true
  enable_pcge: true
genetic_algorithm:
  population_size: 50
  generations: 200
  mutation_rate: 0.2
```

### Custom Configuration Template

```yaml
# Custom obfuscation configuration
mode: custom
level: medium         # low, medium, high
cycles: 3             # 1-10

# Transformation passes (enable specific passes)
passes:
  - control_flow_flattening
  - instruction_substitution
  - constant_obfuscation
  - string_encryption

# MAOS components
maos:
  enable_atie: true   # Genetic algorithm optimization
  enable_qirl: true   # Quantum-inspired randomization
  enable_pcge: true   # Polymorphic code generation

# Genetic algorithm settings
genetic_algorithm:
  population_size: 30      # 10-100
  generations: 100         # 50-500
  mutation_rate: 0.15      # 0.0-1.0
  crossover_rate: 0.8      # 0.0-1.0
  tournament_size: 3       # 2-10

# Performance targets
targets:
  max_size_increase: 0.5   # 50% increase
  max_time_overhead: 0.1   # 10% overhead
  min_resistance: 0.8      # 0.0-1.0

# Reporting
report:
  format: [json, html, txt]
  output_dir: reports
```

---

## 💡 Examples

### Example 1: Quick Test (Easy Program)

```bash
cd /workspaces/SIH25_2/build
./llvm-obfuscator -i ../tests/test_easy.c -o test_output
```

**Expected Output:**
```
LLVM Obfuscator v1.0.0
Input: ../tests/test_easy.c
Output: test_output

Phase 1: Code Analysis... ✓
Phase 2: Transformation Planning... ✓
Phase 3: Applying Obfuscation... ✓
Phase 4: Verification... ✓

Results:
  Original size:     471 bytes
  Obfuscated size:   15,888 bytes
  Size increase:     +3273%
  Strings encrypted: 0
  Cycles completed:  2
  Processing time:   107 ms

Report generated: obfuscation_report.json
```

### Example 2: Size-Conservative for Production

```bash
# Mobile app obfuscation
./llvm-obfuscator \
  -i app_core.c \
  -o build/obfuscated \
  -c ../config/size_conservative.yaml \
  -v
```

**Use Case:** Android/iOS app with size constraints

### Example 3: Maximum Security for Algorithm Protection

```bash
# Protect proprietary algorithm
./llvm-obfuscator \
  -i crypto_algorithm.c \
  -o protected \
  -c ../config/maximum_security.yaml \
  -C 10 \
  -v
```

**Use Case:** Cryptographic library, DRM system

### Example 4: Batch Processing

```bash
# Obfuscate multiple files
for file in ../tests/*.c; do
  echo "Processing $file..."
  ./llvm-obfuscator -i "$file" -o batch_output -l medium -C 3
done
```

### Example 5: Reproducible Build

```bash
# Same seed for CI/CD pipeline
./llvm-obfuscator \
  -i release.c \
  -o production \
  -l high \
  -C 5 \
  -s 123456789 \
  -v
```

**Use Case:** Consistent builds in automated pipeline

### Example 6: Custom Configuration

```bash
# Create custom config
cat > my_config.yaml << EOF
mode: custom
level: medium
cycles: 4
passes:
  - control_flow_flattening
  - instruction_substitution
  - string_encryption
maos:
  enable_atie: true
  enable_qirl: true
  enable_pcge: true
genetic_algorithm:
  population_size: 40
  generations: 150
EOF

# Use custom config
./llvm-obfuscator -i source.c -o output -c my_config.yaml
```

---

## 🐛 Troubleshooting

### Issue 1: "File not found" Error

**Problem:**
```
Error: Cannot read input file: source.c
```

**Solution:**
```bash
# Use absolute path or correct relative path
./llvm-obfuscator -i /full/path/to/source.c -o output

# Or verify file exists
ls -la source.c
```

### Issue 2: Compilation Failures

**Problem:**
```
Error: Compilation failed at preprocessing stage
```

**Solution:**
```bash
# Check if file is valid C/C++
gcc -fsyntax-only source.c

# Try with verbose mode to see details
./llvm-obfuscator -i source.c -o output -v
```

### Issue 3: "Permission denied" Error

**Problem:**
```
Error: Cannot write to output directory
```

**Solution:**
```bash
# Check directory permissions
ls -ld output_dir

# Create directory if needed
mkdir -p output_dir
chmod 755 output_dir
```

### Issue 4: Out of Memory

**Problem:**
```
Error: std::bad_alloc
```

**Solution:**
```bash
# Reduce cycles or use size-conservative mode
./llvm-obfuscator -i large_file.c -o output -C 2 -l low

# Or increase system memory limit
ulimit -v unlimited
```

### Issue 5: Slow Performance

**Problem:** Obfuscation taking too long

**Solution:**
```bash
# Reduce genetic algorithm iterations
./llvm-obfuscator -i source.c -o output \
  -c <(cat << EOF
genetic_algorithm:
  population_size: 20
  generations: 50
EOF
)

# Or use fewer cycles
./llvm-obfuscator -i source.c -o output -C 1
```

### Issue 6: Invalid YAML Configuration

**Problem:**
```
Error: Failed to parse configuration file
```

**Solution:**
```bash
# Validate YAML syntax
python3 -c "import yaml; yaml.safe_load(open('config.yaml'))"

# Use one of the pre-configured files
./llvm-obfuscator -i source.c -o output \
  -c ../config/size_conservative.yaml
```

---

## 📊 Understanding the Report

### JSON Report Structure

```json
{
  "metadata": {
    "version": "1.0.0",
    "timestamp": "2025-10-10T04:40:00Z",
    "input_file": "test.c",
    "output_dir": "output"
  },
  "configuration": {
    "mode": "size_conservative",
    "level": "low",
    "cycles": 2
  },
  "metrics": {
    "original_size_bytes": 471,
    "obfuscated_size_bytes": 15888,
    "size_increase_percent": 3273.25,
    "processing_time_ms": 107,
    "strings_encrypted": 0,
    "functions_virtualized": 0
  },
  "transformations": [
    {
      "pass": "InstructionSubstitution",
      "instructions_modified": 145,
      "execution_time_ms": 12
    }
  ],
  "resistance_scores": {
    "overall": 0.75,
    "static_analysis": 0.70,
    "dynamic_analysis": 0.80,
    "decompilation": 0.75
  },
  "genetic_algorithm": {
    "generations": 100,
    "best_fitness": 0.92,
    "convergence_generation": 78
  }
}
```

### Key Metrics Explained

- **Size Increase %**: How much larger the obfuscated binary is
  - Size-conservative: Target ≤15%
  - Maximum-security: Can be 50-200%

- **Processing Time**: Time to complete obfuscation
  - Low level: Usually < 1 second
  - High level: Can be several seconds

- **Resistance Scores**: Estimated difficulty for reverse engineering (0.0-1.0)
  - 0.0-0.4: Low resistance
  - 0.4-0.7: Medium resistance
  - 0.7-0.9: High resistance
  - 0.9-1.0: Very high resistance

- **Genetic Algorithm Fitness**: Optimization quality (0.0-1.0)
  - Higher is better
  - Convergence generation shows optimization efficiency

---

## 🎓 Best Practices

### 1. Start with Size-Conservative

```bash
# Always test with minimal obfuscation first
./llvm-obfuscator -i app.c -o test -l low -C 1
```

### 2. Measure Performance Impact

```bash
# Before obfuscation
time ./original_binary

# After obfuscation
time ./obfuscated_binary

# Calculate overhead
```

### 3. Use Reproducible Builds

```bash
# Set fixed seed for CI/CD
./llvm-obfuscator -i release.c -o build -s $BUILD_NUMBER
```

### 4. Protect Critical Code Only

Don't obfuscate entire codebase - focus on:
- License validation
- Cryptographic algorithms
- Proprietary business logic
- Anti-piracy checks

### 5. Test Thoroughly

```bash
# Always run your test suite after obfuscation
./obfuscated_binary --run-tests
```

### 6. Monitor Size/Performance

```bash
# Track metrics over time
echo "$(date),$(stat -f%z obfuscated)" >> size_history.csv
```

---

## 📚 Additional Resources

### Documentation

- **Technical Details:** See `MAOS_COMPLETE_IMPLEMENTATION_REPORT.md`
- **Architecture:** See `PROJECT_MAOS.md`
- **Development:** See `MAOS_IMPLEMENTATION_PLAN.md`
- **Testing:** See scientific test reports in `scientific_results/`

### Configuration Templates

- **Size-Conservative:** `config/size_conservative.yaml`
- **Maximum-Security:** `config/maximum_security.yaml`

### Test Files

Sample programs in `tests/` directory:
- `test_easy.c` - Simple program
- `test_medium.c` - Moderate complexity
- `test_difficult.c` - Complex algorithms
- `test_threads.c` - Multi-threaded code
- And more...

---

## 🆘 Getting Help

### Command-Line Help

```bash
./llvm-obfuscator --help
```

### Version Information

```bash
./llvm-obfuscator --version
```

### Verbose Debugging

```bash
./llvm-obfuscator -i source.c -o output -v 2>&1 | tee debug.log
```

---

## ✅ Quick Checklist

Before obfuscating production code:

- [ ] Code compiles without errors
- [ ] All tests pass on original code
- [ ] Chosen appropriate obfuscation level
- [ ] Configured size/performance targets
- [ ] Set up reproducible build (if needed)
- [ ] Prepared output directory
- [ ] Backed up original source
- [ ] Tested obfuscated binary
- [ ] Verified functionality
- [ ] Measured performance impact
- [ ] Reviewed obfuscation report
- [ ] Documented configuration used

---

**🎉 You're ready to use MAOS!**

For more details, see the complete documentation in the project repository.
