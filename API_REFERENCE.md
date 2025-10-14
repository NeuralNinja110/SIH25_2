# API Reference

Complete command-line reference for Phantron Obfuscator.

## Table of Contents

- [Command Overview](#command-overview)
- [Global Options](#global-options)
- [Obfuscate Command](#obfuscate-command)
- [Interactive Command](#interactive-command)
- [Check Command](#check-command)
- [Examples Command](#examples-command)
- [Configuration Files](#configuration-files)
- [Output Format](#output-format)
- [Exit Codes](#exit-codes)
- [Environment Variables](#environment-variables)

## Command Overview

```bash
phantron <command> [options]
```

### Available Commands

| Command | Description |
|---------|-------------|
| `obfuscate` | Obfuscate source code files |
| `interactive` | Launch interactive configuration mode |
| `check` | Verify system prerequisites |
| `examples` | Display usage examples |
| `help` | Show help for a command |

## Global Options

Options available for all commands:

```bash
-V, --version              Output the version number
-h, --help                 Display help information
```

### Examples

```bash
# Show version
phantron --version
phantron -V

# Show general help
phantron --help
phantron -h

# Show help for specific command
phantron obfuscate --help
phantron help obfuscate
```

## Obfuscate Command

Primary command for code obfuscation.

### Syntax

```bash
phantron obfuscate [options]
```

### Options

#### Required Options

| Option | Alias | Type | Description |
|--------|-------|------|-------------|
| `--input <files>` | `-i` | String | Input C/C++ file(s) to obfuscate |
| `--platform <type>` | `-p` | String | Target platform: linux or windows |
| `--mode <mode>` | `-m` | String | Obfuscation mode: normal, security, or size |

#### Optional Options

| Option | Alias | Type | Default | Description |
|--------|-------|------|---------|-------------|
| `--auto-tune` | `-a` | Flag | false | Enable auto-tuning |
| `--iterations <n>` | `-n` | Number | 5 | Number of auto-tune iterations |
| `--config <path>` | `-c` | String | Auto | Path to custom config file |
| `--output-dir <path>` | `-o` | String | Auto | Output directory path |
| `--verbose` | `-v` | Flag | false | Enable verbose output |

### Input Files

#### Single File

```bash
phantron obfuscate -i input.c -p linux -m normal
```

#### Multiple Files

Use comma-separated list (no spaces):

```bash
phantron obfuscate -i "main.c,utils.c,helper.c" -p linux -m normal
```

#### File Extensions

Supported file extensions:
- `.c` - C source files
- `.cpp` - C++ source files
- `.cc` - C++ source files
- `.cxx` - C++ source files

### Platform

Specifies the target platform for the obfuscated binary.

#### Linux Platform

```bash
phantron obfuscate -i input.c -p linux -m normal
```

- Output: ELF executable
- Compiler: GCC
- Binary name: `input_obfuscated`

#### Windows Platform

```bash
phantron obfuscate -i input.c -p windows -m normal
```

- Output: PE executable
- Compiler: MinGW (cross-compilation on Linux)
- Binary name: `input_obfuscated.exe`

### Obfuscation Modes

#### Normal Mode (Balanced)

```bash
phantron obfuscate -i input.c -p linux -m normal
```

**Characteristics**:
- Config file: `config/balanced.yaml`
- Security weight: 45%
- Performance weight: 35%
- Size weight: 20%
- Best for: General-purpose applications

#### Security Mode (Maximum Protection)

```bash
phantron obfuscate -i input.c -p linux -m security
```

**Characteristics**:
- Config file: `config/maximum_security.yaml`
- Security weight: 60%
- Performance weight: 30%
- Size weight: 10%
- Best for: Sensitive algorithms, intellectual property protection

#### Size Mode (Size-Conservative)

```bash
phantron obfuscate -i input.c -p linux -m size
```

**Characteristics**:
- Config file: `config/size_conservative.yaml`
- Security weight: 50%
- Performance weight: 20%
- Size weight: 30%
- Best for: Embedded systems, size-constrained environments

### Auto-Tuning

Enable automatic parameter optimization:

```bash
phantron obfuscate -i input.c -p linux -m security -a -n 10
```

**Process**:
1. Generates parameter variations based on selected mode
2. Runs obfuscation with each configuration
3. Measures security, size, and performance metrics
4. Calculates weighted fitness scores
5. Selects and applies optimal configuration

**Iteration Count**:
- Minimum: 3 (quick test)
- Default: 5 (balanced)
- Recommended: 10 (thorough)
- Maximum: Unlimited (diminishing returns after 15)

### Custom Configuration

Use a custom YAML configuration:

```bash
phantron obfuscate -i input.c -p linux -c /path/to/custom.yaml
```

**Note**: When using custom config, mode flag is ignored.

### Output Directory

Specify custom output directory:

```bash
phantron obfuscate -i input.c -p linux -m normal -o /path/to/output
```

**Default**: `obfuscation_results/session_<timestamp>/`

### Verbose Mode

Enable detailed logging:

```bash
phantron obfuscate -i input.c -p linux -m normal -v
```

**Output includes**:
- Detailed obfuscation steps
- LLVM transformation logs
- Compilation output
- Metric calculation details

### Complete Example

```bash
phantron obfuscate \
  --input "main.c,utils.c,helper.c" \
  --platform linux \
  --mode security \
  --auto-tune \
  --iterations 10 \
  --output-dir ./secured \
  --verbose
```

## Interactive Command

Launch interactive mode with guided prompts.

### Syntax

```bash
phantron interactive
```

### Interactive Prompts

1. **Input Files**: Enter file path(s)
2. **Platform Selection**: Choose Linux or Windows
3. **Mode Selection**: Choose Normal, Security, or Size
4. **Auto-Tuning**: Enable/disable auto-tuning
5. **Iterations**: Set iteration count (if auto-tuning enabled)

### Example Session

```bash
$ phantron interactive

? Select input files:
  > input.c

? Select target platform:
  > Linux

? Select obfuscation mode:
  > Security Focused

? Enable auto-tuning?
  > Yes

? Number of iterations:
  > 10

Obfuscation started...
```

### Benefits

- User-friendly for beginners
- Validates input before execution
- Provides hints and descriptions
- Allows easy configuration changes

## Check Command

Verify system prerequisites and configuration.

### Syntax

```bash
phantron check
```

### Verification Items

1. **Node.js Version**: >= 14.0.0
2. **LLVM Binary**: Availability and permissions
3. **Configuration Files**: Existence and validity
4. **GCC Compiler**: For Linux targets
5. **MinGW Compiler**: For Windows targets (optional)

### Output Format

```
Prerequisites Check
-------------------
Node.js                     PASS    v16.14.0
LLVM Obfuscator Binary      PASS    Found
Configuration Files         PASS    All found
GCC Compiler                PASS    Available
MinGW Compiler              WARN    Not found (Windows targets unavailable)
```

### Exit Codes

- `0`: All prerequisites met
- `1`: One or more prerequisites failed

## Examples Command

Display common usage examples.

### Syntax

```bash
phantron examples
```

### Output

Displays examples for:
- Basic obfuscation
- Security-focused obfuscation
- Size-conservative obfuscation
- Multi-file obfuscation
- Auto-tuning usage
- Cross-platform compilation

## Configuration Files

### File Locations

Default configurations are located in the package installation directory:

```
<npm_root>/phantron-obfuscator/config/
├── balanced.yaml
├── maximum_security.yaml
└── size_conservative.yaml
```

### Configuration Structure

```yaml
obfuscation:
  level: medium                    # low, medium, high
  cycles: 3                        # Number of obfuscation passes
  
  control_flow:
    flattening:
      enabled: true
      probability: 70              # Percentage
    bogus:
      enabled: true
      probability: 50
  
  string_encryption:
    enabled: true
    probability: 80
    algorithm: aes128              # aes128, aes256, xor
  
  constant_obfuscation:
    enabled: true
    probability: 60
  
  anti_debugging:
    enabled: true
    techniques:
      - ptrace_detection
      - timing_checks
  
  virtualization:
    enabled: false                 # Function virtualization
    functions: []
```

### Custom Configuration

Create custom YAML file with desired parameters:

```bash
# Create custom config
cat > my_config.yaml << EOF
obfuscation:
  level: high
  cycles: 5
  control_flow:
    flattening:
      enabled: true
      probability: 90
EOF

# Use it
phantron obfuscate -i input.c -p linux -c my_config.yaml
```

## Output Format

### Directory Structure

```
obfuscation_results/session_<timestamp>/
├── <filename>_obfuscated              # Obfuscated binary (Linux)
├── <filename>_obfuscated.exe          # Obfuscated binary (Windows)
├── obfuscation_report.html            # HTML report
├── obfuscation_report.json            # JSON metrics
├── best_config.yaml                   # Best config (auto-tune mode)
├── config_iter1.yaml                  # Iteration 1 config (auto-tune)
├── config_iter2.yaml                  # Iteration 2 config (auto-tune)
└── ...
```

### JSON Report Format

```json
{
  "session_id": "session_1234567890",
  "timestamp": "2025-10-13T12:34:56.789Z",
  "input_files": [
    {
      "path": "input.c",
      "size": 1234,
      "checksum": "abc123..."
    }
  ],
  "configuration": {
    "mode": "security",
    "platform": "linux",
    "auto_tune": true,
    "iterations": 10
  },
  "metrics": {
    "duration": "15.3s",
    "re_score": 87,
    "security_score": 92,
    "resilience_score": 85,
    "fitness_score": 88
  },
  "transformations": {
    "obfuscation_cycles": 5,
    "string_obfuscations": 45,
    "fake_functions": 23,
    "bogus_code_lines": 187
  },
  "output_files": [
    {
      "path": "input_obfuscated",
      "size": 16384,
      "size_increase_percent": 23.5
    }
  ]
}
```

### HTML Report Sections

1. **Configuration Parameters**: Input settings and options
2. **Input Files**: Source file details and checksums
3. **Output Files**: Generated binary details and sizes
4. **Obfuscation Details**: Transformation statistics
5. **Metrics Summary**: Security scores and RE difficulty
6. **Techniques Applied**: List of obfuscation methods used

## Exit Codes

| Code | Meaning |
|------|---------|
| `0` | Success |
| `1` | General error |
| `2` | Invalid arguments |
| `3` | File not found |
| `4` | Obfuscation failed |
| `5` | Compilation failed |
| `6` | Prerequisites not met |

### Checking Exit Code

```bash
# Bash
phantron obfuscate -i input.c -p linux -m normal
echo $?

# Use in scripts
if phantron obfuscate -i input.c -p linux -m normal; then
    echo "Success"
else
    echo "Failed with code $?"
fi
```

## Environment Variables

### PHANTRON_CONFIG_DIR

Override default configuration directory:

```bash
export PHANTRON_CONFIG_DIR=/path/to/configs
phantron obfuscate -i input.c -p linux -m normal
```

### PHANTRON_OUTPUT_DIR

Override default output directory:

```bash
export PHANTRON_OUTPUT_DIR=/path/to/output
phantron obfuscate -i input.c -p linux -m normal
```

### PHANTRON_VERBOSE

Enable verbose mode globally:

```bash
export PHANTRON_VERBOSE=1
phantron obfuscate -i input.c -p linux -m normal
```

### NODE_OPTIONS

Node.js specific options:

```bash
# Increase memory limit
export NODE_OPTIONS="--max-old-space-size=4096"
phantron obfuscate -i large_file.c -p linux -m security
```

## Advanced Usage

### Batch Processing

```bash
# Obfuscate all C files in directory
for file in *.c; do
    phantron obfuscate -i "$file" -p linux -m security
done
```

### Scripted Auto-Tuning

```bash
#!/bin/bash
# Test different iteration counts
for iterations in 3 5 10 15; do
    echo "Testing with $iterations iterations"
    phantron obfuscate \
        -i test.c \
        -p linux \
        -m security \
        -a \
        -n $iterations \
        -o results_$iterations
done
```

### Integration with CI/CD

```yaml
# GitHub Actions example
- name: Obfuscate release binaries
  run: |
    npm install -g phantron-obfuscator
    phantron obfuscate \
      -i src/main.c \
      -p linux \
      -m security \
      -o dist/
```

## See Also

- [INSTALLATION.md](https://gist.github.com/NeuralNinja110/b8be1d6452830662977d696c114d80be) - Installation guide
- [CLI_DOCUMENTATION.md](https://gist.github.com/NeuralNinja110/ef656c198bdaf17f5e7ce5a46d04cc5f) - CLI usage documentation
- [NPM Package](https://www.npmjs.com/package/phantron-obfuscator) - Package page

---

**Last Updated**: October 2025  
**Version**: 2.0.0
