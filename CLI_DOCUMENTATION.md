# Phantron Obfuscator - Complete CLI Documentation

Version 2.0.3

---

## Table of Contents

- [Overview](#overview)
- [Installation](#installation)
- [Command-Line Interface](#command-line-interface)
- [Commands](#commands)
- [Options and Flags](#options-and-flags)
- [Obfuscation Modes](#obfuscation-modes)
- [Auto-Tuning](#auto-tuning)
- [Configuration Files](#configuration-files)
- [Input and Output](#input-and-output)
- [Obfuscation Techniques](#obfuscation-techniques)
- [Reporting](#reporting)
- [Usage Examples](#usage-examples)
- [Performance and Benchmarks](#performance-and-benchmarks)
- [Troubleshooting](#troubleshooting)
- [Advanced Usage](#advanced-usage)
- [Exit Codes](#exit-codes)
- [Environment Variables](#environment-variables)

---

## Overview

Phantron Obfuscator is a professional command-line tool for advanced code obfuscation of C and C++ projects. It leverages LLVM compiler infrastructure to perform deep, semantic transformations that make reverse engineering significantly more difficult while preserving program correctness and functionality.

### Key Features

- LLVM-based code obfuscation with real compiler transformations
- Multi-file and interdependent source support
- Cross-platform binary generation (Linux and Windows)
- Multiple obfuscation modes (normal, security-focused, size-conservative)
- Intelligent auto-tuning for optimal parameter selection
- Comprehensive HTML and JSON reporting
- Interactive and scriptable command-line interface
- YAML-based configuration system

---

## Installation

### Prerequisites

Before installing Phantron Obfuscator, ensure you have:

- **Node.js** version 14.0.0 or higher
- **npm** (comes with Node.js)
- **GCC** (for Linux target compilation)
- **MinGW** (for Windows target cross-compilation on Linux)

### Global Installation (Recommended)

Install Phantron globally to use it from anywhere:

```bash
npm install -g phantron-obfuscator
```

### Local Installation

Install in your project directory:

```bash
npm install phantron-obfuscator
```

Then use via npx:

```bash
npx phantron [command] [options]
```

### Verify Installation

Check that Phantron is installed correctly:

```bash
phantron --version
phantron check
```

The `check` command verifies all prerequisites are met.

---

## Command-Line Interface

Phantron provides a clean, intuitive command-line interface with multiple commands and options.

### General Syntax

```bash
phantron [command] [options]
```

### Global Options

Available with any command:

- `-V, --version` - Display version number
- `-h, --help` - Display help information

---

## Commands

### obfuscate

Obfuscate C/C++ source files with specified parameters.

**Syntax:**
```bash
phantron obfuscate [options]
```

**Required Options:**
- `-i, --input <files>` - Input file(s)
- `-p, --platform <type>` - Target platform
- `-m, --mode <mode>` - Obfuscation mode

**Optional Options:**
- `-a, --auto-tune` - Enable auto-tuning
- `-n, --iterations <n>` - Number of auto-tune iterations (default: 5)
- `-c, --config <path>` - Custom configuration file path

**Example:**
```bash
phantron obfuscate -i main.c -p linux -m security
```

---

### interactive

Launch interactive mode with guided prompts for all options.

**Syntax:**
```bash
phantron interactive
```

**Features:**
- Step-by-step configuration
- Input validation
- File browsing assistance
- Configuration preview before execution
- Helpful descriptions for each option

**Example:**
```bash
phantron interactive
```

This will start an interactive session where you'll be prompted for:
1. Input file(s)
2. Target platform
3. Obfuscation mode
4. Auto-tuning preference
5. Number of iterations (if auto-tuning enabled)

---

### check

Verify system prerequisites and environment setup.

**Syntax:**
```bash
phantron check
```

**Checks Performed:**
- Node.js version compatibility
- LLVM obfuscator binary availability
- GCC compiler availability (for Linux targets)
- MinGW compiler availability (for Windows targets)
- Configuration file integrity
- File system permissions

**Output:**
- Detailed status for each prerequisite
- PASS/FAIL indicators
- Suggestions for fixing issues

**Example:**
```bash
phantron check
```

---

### examples

Display comprehensive usage examples.

**Syntax:**
```bash
phantron examples
```

**Shows:**
- Basic obfuscation examples
- Multi-file processing
- Auto-tuning examples
- Platform-specific examples
- Advanced use cases

**Example:**
```bash
phantron examples
```

---

## Options and Flags

### Input Options

#### -i, --input <files>

Specify input C/C++ source file(s).

**Single File:**
```bash
phantron obfuscate -i main.c -p linux -m normal
```

**Multiple Files (comma-separated):**
```bash
phantron obfuscate -i "main.c,utils.c,crypto.cpp" -p linux -m normal
```

**Supported File Types:**
- `.c` - C source files
- `.cpp`, `.cc`, `.cxx` - C++ source files
- `.h`, `.hpp` - Header files (when part of compilation unit)

---

### Platform Options

#### -p, --platform <type>

Specify target platform for binary generation.

**Values:**
- `linux` - Generate ELF binaries for Linux
- `windows` - Generate PE executables (.exe) for Windows

**Examples:**
```bash
# Linux target
phantron obfuscate -i app.c -p linux -m normal

# Windows target
phantron obfuscate -i app.c -p windows -m normal
```

**Note:** Windows targets can be built on Linux using MinGW cross-compilation.

---

### Mode Options

#### -m, --mode <mode>

Select obfuscation mode (security profile).

**Values:**

1. **normal** - Balanced mode
   - Focus: 45% security, 35% performance, 20% size
   - Best for: General-purpose applications
   - Config: `config/balanced.yaml`

2. **security** - Maximum protection
   - Focus: 60% security, 30% performance, 10% size
   - Best for: Sensitive algorithms, IP protection
   - Config: `config/maximum_security.yaml`

3. **size** - Size-conservative
   - Focus: 50% security, 20% performance, 30% size
   - Best for: Embedded systems, size constraints
   - Config: `config/size_conservative.yaml`

**Examples:**
```bash
# Normal mode
phantron obfuscate -i app.c -p linux -m normal

# Security mode
phantron obfuscate -i sensitive.c -p linux -m security

# Size mode
phantron obfuscate -i embedded.c -p linux -m size
```

---

### Auto-Tuning Options

#### -a, --auto-tune

Enable automatic parameter optimization.

**Functionality:**
- Tests multiple parameter configurations
- Measures security, size, and performance metrics
- Calculates weighted fitness scores
- Selects optimal configuration
- Applies best settings to final obfuscation

**Example:**
```bash
phantron obfuscate -i app.c -p linux -m security -a
```

---

#### -n, --iterations <number>

Specify number of auto-tuning iterations.

**Default:** 5

**Range:** 1-20 (recommended: 5-10)

**Examples:**
```bash
# Quick auto-tune (3 iterations)
phantron obfuscate -i app.c -p linux -m security -a -n 3

# Thorough auto-tune (10 iterations)
phantron obfuscate -i app.c -p linux -m security -a -n 10
```

**Note:** More iterations increase processing time but may yield better results.

---

### Configuration Options

#### -c, --config <path>

Use custom configuration file instead of mode defaults.

**Example:**
```bash
phantron obfuscate -i app.c -p linux -c /path/to/custom.yaml
```

**Configuration File Format:** YAML (see Configuration Files section)

---

## Obfuscation Modes

### Normal Mode (balanced.yaml)

Balanced approach for general-purpose applications.

**Configuration Highlights:**
- Obfuscation cycles: 3
- Control flow flattening: 70% probability
- String encryption: 80% probability
- Bogus control flow: 50% probability
- Anti-debugging: Basic

**Use Cases:**
- Desktop applications
- Server software
- General commercial software

---

### Security Mode (maximum_security.yaml)

Maximum protection for sensitive code.

**Configuration Highlights:**
- Obfuscation cycles: 5-7
- Control flow flattening: 90% probability
- String encryption: 95% probability
- Bogus control flow: 80% probability
- Anti-debugging: Advanced
- Function virtualization: Enabled

**Use Cases:**
- Cryptographic implementations
- Proprietary algorithms
- DRM systems
- Security-critical software

---

### Size Mode (size_conservative.yaml)

Minimal size increase for constrained environments.

**Configuration Highlights:**
- Obfuscation cycles: 2
- Control flow flattening: 50% probability
- String encryption: 60% probability
- Bogus control flow: 30% probability
- Anti-debugging: Minimal
- Focus on compact transformations

**Use Cases:**
- Embedded systems
- IoT devices
- Mobile applications
- Size-constrained deployments

---

## Auto-Tuning

Auto-tuning is an intelligent optimization system that finds the best obfuscation parameters for your specific code.

### How It Works

1. **Parameter Variation Generation**
   - Creates variations of base configuration
   - Adjusts cycles, probabilities, and thresholds
   - Covers parameter space systematically

2. **Iteration Testing**
   - Runs obfuscation with each configuration
   - Compiles and measures resulting binaries
   - Collects metrics (size, security scores)

3. **Fitness Calculation**
   - Applies mode-specific weights
   - Security mode: 60% security, 10% size, 30% performance
   - Size mode: 40% security, 40% size, 20% performance
   - Normal mode: 40% security, 20% size, 40% performance

4. **Best Selection**
   - Ranks all configurations by fitness score
   - Selects highest-scoring configuration
   - Applies to final obfuscation run

### Auto-Tuning Output

During auto-tuning, you'll see:
- Progress for each iteration
- Fitness scores
- Best configuration updates
- Final optimized parameters

Generated files:
- `config_iter1.yaml` through `config_iterN.yaml` - Each iteration's config
- `best_config.yaml` - Optimal configuration selected
- `temp_iter1_*` through `temp_iterN_*` - Test binaries
- Final obfuscated binary with best configuration

---

## Configuration Files

Configuration files control obfuscation behavior using YAML format.

### File Location

Default configurations are in the `config/` directory:
- `balanced.yaml`
- `maximum_security.yaml`
- `size_conservative.yaml`

### Configuration Structure

```yaml
obfuscation:
  level: medium           # low, medium, high
  cycles: 3               # Number of obfuscation passes
  seed: 12345             # Random seed (optional, for reproducibility)
  
  control_flow:
    flattening:
      enabled: true
      probability: 70     # Percentage (0-100)
    bogus:
      enabled: true
      probability: 50
  
  string_encryption:
    enabled: true
    probability: 80
    algorithm: aes        # aes, xor, custom
  
  constant_obfuscation:
    enabled: true
    probability: 60
  
  instruction_substitution:
    enabled: true
    probability: 70
  
  anti_debugging:
    enabled: true
    level: basic          # basic, advanced
  
  virtualization:
    enabled: false        # CPU-intensive
    function_threshold: 5 # Min instructions for virtualization

metrics:
  generate: true
  format: both            # json, html, both
  output_path: obfuscation_report
```

### Custom Configuration

Create your own configuration file:

```bash
# Copy an existing config
cp config/balanced.yaml my_config.yaml

# Edit as needed
nano my_config.yaml

# Use custom config
phantron obfuscate -i app.c -p linux -c my_config.yaml
```

---

## Input and Output

### Input Files

**Supported Formats:**
- C source files (`.c`)
- C++ source files (`.cpp`, `.cc`, `.cxx`)
- Header files (`.h`, `.hpp`) when included in compilation

**Single File:**
```bash
phantron obfuscate -i main.c -p linux -m normal
```

**Multiple Files:**
```bash
phantron obfuscate -i "main.c,utils.c,crypto.c" -p linux -m normal
```

**File Dependencies:**
- Include all interdependent files
- Phantron handles compilation order
- Header files are processed automatically

---

### Output Structure

All output is saved to: `obfuscation_results/session_<timestamp>/`

**Generated Files:**

1. **Obfuscated Binary**
   - Linux: `<basename>_obfuscated`
   - Windows: `<basename>_obfuscated.exe`
   - Executable, ready to run

2. **HTML Report**
   - `obfuscation_report.html`
   - Comprehensive metrics and visualizations
   - Open in any web browser

3. **JSON Report**
   - `obfuscation_report.json`
   - Machine-readable metrics
   - For programmatic analysis

4. **Configuration Files** (auto-tune mode)
   - `config_iter1.yaml` through `config_iterN.yaml`
   - `best_config.yaml` - Optimal configuration
   - Reusable for future obfuscations

5. **Test Binaries** (auto-tune mode)
   - `temp_iter1_<basename>` through `temp_iterN_<basename>`
   - Intermediate binaries from each iteration

---

## Obfuscation Techniques

Phantron applies multiple layers of obfuscation at the LLVM IR level.

### Control Flow Flattening

**Description:** Restructures program control flow into a dispatch-based structure.

**Effect:**
- Linear code flow becomes state machine
- Jump targets obscured
- Makes static analysis extremely difficult

**Impact:**
- High security benefit
- Moderate performance overhead
- Moderate size increase

---

### String Encryption

**Description:** Encrypts string literals with runtime decryption.

**Methods:**
- AES encryption
- XOR encoding
- Custom algorithms

**Effect:**
- Strings hidden in binary
- Decrypted only when needed
- Prevents simple string extraction

**Impact:**
- High security benefit
- Low performance overhead
- Small size increase

---

### Instruction Substitution

**Description:** Replaces instructions with functionally equivalent but more complex sequences.

**Examples:**
- `x = a + b` becomes `x = (a ^ b) + 2 * (a & b)`
- Simple operations become complex expressions

**Effect:**
- Obscures arithmetic and logic
- Increases instruction count
- Complicates pattern recognition

**Impact:**
- Medium security benefit
- Low performance overhead
- Medium size increase

---

### Bogus Control Flow

**Description:** Inserts fake conditional branches that never execute.

**Effect:**
- Creates unreachable code paths
- Increases control flow complexity
- Misleads disassemblers and decompilers

**Impact:**
- Medium security benefit
- Minimal performance overhead
- Medium size increase

---

### Function Inlining

**Description:** Removes function boundaries by inlining function bodies.

**Effect:**
- Eliminates function call patterns
- Merges function code into callers
- Prevents function identification

**Impact:**
- Medium security benefit
- Potential performance improvement
- Large size increase

---

### Variable Renaming

**Description:** Renames variables to non-meaningful identifiers.

**Effect:**
- Removes semantic information
- Makes decompiled code harder to understand

**Impact:**
- Low security benefit (mainly for source-level analysis)
- No performance overhead
- No size increase

---

### Constant Obfuscation

**Description:** Hides and encrypts constant values.

**Methods:**
- Mathematical expressions
- Encryption
- Runtime computation

**Effect:**
- Constants not visible in binary
- Harder to identify magic numbers

**Impact:**
- Medium security benefit
- Low performance overhead
- Small size increase

---

### Anti-Debugging

**Description:** Detects and prevents debugging attempts.

**Techniques:**
- Debugger detection
- Timing checks
- Integrity verification

**Levels:**
- **Basic:** Simple debugger checks
- **Advanced:** Multi-layered anti-debugging

**Effect:**
- Prevents dynamic analysis
- Terminates if debugger detected

**Impact:**
- High security benefit
- Minimal performance overhead
- Small size increase

---

### Function Virtualization

**Description:** Converts functions to custom bytecode executed by a virtual machine.

**Effect:**
- Custom instruction set
- Extremely difficult to reverse
- High protection for critical functions

**Impact:**
- Very high security benefit
- Significant performance overhead
- Large size increase

**Note:** Use selectively on critical functions only.

---

## Reporting

Phantron generates comprehensive reports in multiple formats.

### HTML Report

Interactive HTML report with:

**Sections:**
1. **Configuration Parameters**
   - Input parameters
   - Platform and mode
   - Auto-tuning status

2. **Input Files**
   - File paths
   - Sizes
   - File types

3. **Output Files**
   - Generated binaries
   - Sizes and size increases
   - Locations

4. **Obfuscation Details**
   - Transformation statistics
   - Applied techniques
   - Cycle counts

5. **Metrics**
   - Security scores
   - RE difficulty scores
   - Resilience metrics

**Opening Report:**
```bash
# Linux
xdg-open obfuscation_results/session_*/obfuscation_report.html

# MacOS
open obfuscation_results/session_*/obfuscation_report.html
```

---

### JSON Report

Machine-readable report with same data as HTML.

**Structure:**
```json
{
  "session_id": "session_1234567890",
  "timestamp": "2025-10-14T10:30:00Z",
  "configuration": {
    "mode": "security",
    "platform": "linux",
    "auto_tune": true,
    "iterations": 5
  },
  "inputs": [
    {
      "path": "/path/to/input.c",
      "size": 1024
    }
  ],
  "outputs": [
    {
      "path": "/path/to/output_obfuscated",
      "size": 16384,
      "size_increase_percent": 1500.0
    }
  ],
  "metrics": {
    "re_difficulty_score": 87,
    "security_score": 92,
    "resilience_score": 85
  },
  "transformations": {
    "obfuscation_cycles": 5,
    "string_obfuscations": 23,
    "fake_functions": 12,
    "bogus_code_lines": 145
  }
}
```

**Using JSON Report:**
```bash
# Parse with jq
cat obfuscation_report.json | jq '.metrics.security_score'

# In Python
import json
with open('obfuscation_report.json') as f:
    report = json.load(f)
    print(f"Security Score: {report['metrics']['security_score']}")
```

---

## Usage Examples

### Basic Examples

**Simple obfuscation:**
```bash
phantron obfuscate -i hello.c -p linux -m normal
```

**Security-focused obfuscation:**
```bash
phantron obfuscate -i crypto.c -p linux -m security
```

**Size-conservative obfuscation:**
```bash
phantron obfuscate -i embedded.c -p linux -m size
```

---

### Multi-File Examples

**Multiple source files:**
```bash
phantron obfuscate -i "main.c,utils.c,network.c" -p linux -m normal
```

**C++ project:**
```bash
phantron obfuscate -i "app.cpp,crypto.cpp,ui.cpp" -p linux -m security
```

---

### Cross-Platform Examples

**Build Windows executable on Linux:**
```bash
phantron obfuscate -i app.c -p windows -m normal
```

**Build for both platforms:**
```bash
# Linux
phantron obfuscate -i app.c -p linux -m security

# Windows
phantron obfuscate -i app.c -p windows -m security
```

---

### Auto-Tuning Examples

**Quick auto-tune (3 iterations):**
```bash
phantron obfuscate -i app.c -p linux -m security -a -n 3
```

**Thorough auto-tune (10 iterations):**
```bash
phantron obfuscate -i critical.c -p linux -m security -a -n 10
```

**Auto-tune with custom config as base:**
```bash
phantron obfuscate -i app.c -p linux -c custom.yaml -a -n 5
```

---

### Advanced Examples

**Custom configuration:**
```bash
phantron obfuscate -i app.c -p linux -c /path/to/custom.yaml
```

**Batch processing:**
```bash
for file in src/*.c; do
  phantron obfuscate -i "$file" -p linux -m security
done
```

**With specific output directory:**
```bash
# Note: Output directory is auto-generated in obfuscation_results/
# To organize, move after obfuscation:
phantron obfuscate -i app.c -p linux -m security
mv obfuscation_results/session_* my_output/
```

---

## Performance and Benchmarks

### Obfuscation Time

| File Size | Mode | Time (approx) |
|-----------|------|---------------|
| < 100 KB | Normal | 3-5 seconds |
| < 100 KB | Security | 5-8 seconds |
| 100-500 KB | Normal | 5-10 seconds |
| 100-500 KB | Security | 10-15 seconds |
| > 500 KB | Normal | 15-30 seconds |
| > 500 KB | Security | 30-60 seconds |

**Auto-Tuning:** Multiply by number of iterations.

---

### Binary Size Impact

| Mode | Size Increase |
|------|---------------|
| Normal | 20-50% |
| Security | 100-200% |
| Size | 10-30% |

**Factors affecting size:**
- Number of functions
- String count
- Code complexity
- Obfuscation cycles

---

### Runtime Performance

| Technique | Performance Impact |
|-----------|-------------------|
| Control Flow Flattening | 10-30% slowdown |
| String Encryption | < 5% slowdown |
| Instruction Substitution | 5-15% slowdown |
| Bogus Control Flow | < 5% slowdown |
| Function Virtualization | 50-200% slowdown |

**Note:** Actual impact depends on code structure and hot paths.

---

### Security Scores

**RE Difficulty Score:** 70-95 (higher is better)
- Measures reverse engineering difficulty
- Based on control flow complexity, encryption, and anti-debugging

**Security Score:** 75-95 (higher is better)
- Overall security metric
- Combines multiple obfuscation technique effectiveness

**Resilience Score:** 70-90 (higher is better)
- Resistance to automated deobfuscation
- Measures against common analysis tools

---

## Troubleshooting

### Common Issues

**Issue: "Command not found: phantron"**

Solution:
```bash
# Check if installed globally
npm list -g phantron-obfuscator

# If not found, reinstall
npm install -g phantron-obfuscator

# Or use npx
npx phantron --version
```

---

**Issue: "LLVM obfuscator binary not found"**

Solution:
```bash
# Run prerequisite check
phantron check

# Verify binary exists
ls -la /usr/local/lib/node_modules/phantron-obfuscator/build/

# Reinstall if missing
npm uninstall -g phantron-obfuscator
npm install -g phantron-obfuscator
```

---

**Issue: "GCC not found"**

Solution:
```bash
# Ubuntu/Debian
sudo apt-get install build-essential

# CentOS/RHEL
sudo yum groupinstall "Development Tools"

# Verify installation
gcc --version
```

---

**Issue: "MinGW not found" (for Windows targets)**

Solution:
```bash
# Ubuntu/Debian
sudo apt-get install mingw-w64

# Verify installation
x86_64-w64-mingw32-gcc --version
```

---

**Issue: "Permission denied" when writing output**

Solution:
```bash
# Check directory permissions
ls -la obfuscation_results/

# Create directory if needed
mkdir -p obfuscation_results

# Set permissions
chmod 755 obfuscation_results
```

---

**Issue: "Configuration file not found"**

Solution:
```bash
# Check config directory
ls -la config/

# Verify config files exist
ls config/*.yaml

# Reinstall if missing
npm uninstall -g phantron-obfuscator
npm install -g phantron-obfuscator
```

---

**Issue: "Obfuscation failed" error**

Solution:
1. Check input file syntax: `gcc -fsyntax-only input.c`
2. Verify file exists: `ls -la input.c`
3. Check file permissions: `chmod 644 input.c`
4. Try with verbose output (if available)
5. Check report for specific errors

---

**Issue: Auto-tuning takes too long**

Solution:
- Reduce iterations: `-n 3` instead of `-n 10`
- Use faster mode: `-m normal` instead of `-m security`
- Split into smaller files

---

### Debug Mode

Check prerequisites:
```bash
phantron check
```

Verify installation:
```bash
which phantron
phantron --version
npm list -g phantron-obfuscator
```

---

## Advanced Usage

### Scripting and Automation

**Bash Script:**
```bash
#!/bin/bash

FILES="main.c utils.c crypto.c"
MODE="security"
PLATFORM="linux"

for file in $FILES; do
  echo "Obfuscating $file..."
  phantron obfuscate -i "$file" -p "$PLATFORM" -m "$MODE"
done

echo "All files obfuscated!"
```

---

**Python Script:**
```python
import subprocess
import os

files = ['main.c', 'utils.c', 'crypto.c']
mode = 'security'
platform = 'linux'

for file in files:
    if os.path.exists(file):
        print(f"Obfuscating {file}...")
        subprocess.run([
            'phantron', 'obfuscate',
            '-i', file,
            '-p', platform,
            '-m', mode
        ])
```

---

### CI/CD Integration

**GitHub Actions:**
```yaml
name: Obfuscate Release

on:
  release:
    types: [created]

jobs:
  obfuscate:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      
      - name: Setup Node.js
        uses: actions/setup-node@v2
        with:
          node-version: '14'
      
      - name: Install Phantron
        run: npm install -g phantron-obfuscator
      
      - name: Obfuscate source
        run: |
          phantron obfuscate -i src/main.c -p linux -m security
          phantron obfuscate -i src/main.c -p windows -m security
      
      - name: Upload artifacts
        uses: actions/upload-artifact@v2
        with:
          name: obfuscated-binaries
          path: obfuscation_results/
```

---

**GitLab CI:**
```yaml
obfuscate:
  stage: build
  image: node:14
  script:
    - npm install -g phantron-obfuscator
    - phantron obfuscate -i src/main.c -p linux -m security
  artifacts:
    paths:
      - obfuscation_results/
```

---

### Configuration Management

**Version Control:**
```bash
# Track custom configurations
git add config/my_custom.yaml
git commit -m "Add custom obfuscation config"

# Don't track results
echo "obfuscation_results/" >> .gitignore
```

---

**Configuration Templates:**
```bash
# Create template directory
mkdir -p config/templates

# Copy base configs
cp config/balanced.yaml config/templates/project_A.yaml
cp config/security.yaml config/templates/project_B.yaml

# Use templates
phantron obfuscate -i app.c -p linux -c config/templates/project_A.yaml
```

---

## Exit Codes

Phantron uses standard exit codes:

| Code | Meaning |
|------|---------|
| 0 | Success |
| 1 | General error |
| 2 | Invalid arguments |
| 3 | File not found |
| 4 | Permission denied |
| 5 | Obfuscation failed |
| 6 | Compilation failed |
| 7 | Configuration error |

**Usage in Scripts:**
```bash
phantron obfuscate -i app.c -p linux -m security
if [ $? -eq 0 ]; then
  echo "Success!"
else
  echo "Failed with exit code $?"
  exit 1
fi
```

---

## Environment Variables

### NODE_ENV

Controls Node.js environment mode.

```bash
export NODE_ENV=production
phantron obfuscate -i app.c -p linux -m security
```

---

### PHANTRON_CONFIG_DIR

Override default config directory.

```bash
export PHANTRON_CONFIG_DIR=/path/to/configs
phantron obfuscate -i app.c -p linux -m security
```

---

### PHANTRON_OUTPUT_DIR

Override default output directory.

```bash
export PHANTRON_OUTPUT_DIR=/path/to/output
phantron obfuscate -i app.c -p linux -m security
```

---

## Conclusion

Phantron Obfuscator provides a comprehensive, professional solution for C/C++ code obfuscation. This documentation covers all aspects of the CLI interface, from basic usage to advanced automation and configuration.

For additional support:
- Run `phantron --help` for quick reference
- Run `phantron examples` for usage examples
- Check generated HTML reports for detailed metrics

---

**Version:** 2.0.3  
**Last Updated:** October 14, 2025  
**License:** MIT
