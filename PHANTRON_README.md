# Phantron Obfuscator

Professional code protection and obfuscation tool with intelligent auto-tuning, multi-file support, and comprehensive HTML reporting.

## Features

- **Multi-File Support**: Process single or multiple interdependent C/C++ files
- **Cross-Platform**: Generate obfuscated binaries for Linux and Windows
- **Multiple Modes**: Normal, Security Focused, and Size Conservative
- **Auto-Tuning**: Intelligent parameter optimization with iterative refinement
- **HTML Reports**: Comprehensive obfuscation reports with detailed metrics
- **Interactive Mode**: Guided setup for easy configuration
- **Professional CLI**: Clean, user-friendly command-line interface

## Installation

### Global Installation (Recommended)

```bash
npm install -g @phantron/obfuscator
```

### Local Installation

```bash
npm install @phantron/obfuscator
```

### From Source

```bash
git clone <repository>
cd SIH25_2
npm install
npm link
```

## Quick Start

### Interactive Mode (Easiest)

```bash
phantron interactive
```

### Command Line Mode

```bash
# Basic obfuscation
phantron obfuscate -i program.c -p linux -m normal

# Multiple files
phantron obfuscate -i "main.c,utils.c,crypto.c" -p windows -m security

# With auto-tuning
phantron obfuscate -i app.cpp -p linux -m security -a -n 10
```

## Usage

### Commands

#### `phantron obfuscate`

Obfuscate source code files with specified parameters.

**Options:**
- `-i, --input <files>` - Input file(s), comma-separated for multiple
- `-p, --platform <platform>` - Target platform: `linux` or `windows` (default: linux)
- `-m, --mode <mode>` - Obfuscation mode: `normal`, `security`, or `size` (default: normal)
- `-a, --auto-tune` - Enable auto-tuning (default: false)
- `-n, --iterations <number>` - Auto-tuning iterations, 1-50 (default: 5)

**Examples:**

```bash
# Single file, Linux, normal mode
phantron obfuscate -i program.c -p linux -m normal

# Multiple files, Windows, security mode
phantron obfuscate -i "main.c,utils.c,crypto.c" -p windows -m security

# Auto-tuning with 10 iterations
phantron obfuscate -i app.cpp -p linux -m security -a -n 10

# Size-optimized for embedded
phantron obfuscate -i firmware.c -p linux -m size
```

#### `phantron interactive`

Launch interactive mode with guided setup. The CLI will prompt you for:
- Input files
- Target platform
- Obfuscation mode
- Auto-tuning preference
- Number of iterations (if auto-tuning)

```bash
phantron interactive
```

#### `phantron check`

Check system prerequisites.

```bash
phantron check
```

#### `phantron examples`

Show usage examples.

```bash
phantron examples
```

## Obfuscation Modes

### Normal Mode
- **Focus**: Balanced obfuscation with moderate security
- **Best For**: General production use
- **Security Level**: 70-80/100
- **Performance Impact**: Moderate
- **Size Increase**: ~150%

### Security Focused Mode
- **Focus**: Maximum protection with aggressive obfuscation
- **Best For**: IP protection, commercial software
- **Security Level**: 85-95/100
- **Performance Impact**: Significant
- **Size Increase**: ~300%

### Size Conservative Mode
- **Focus**: Minimal size increase
- **Best For**: Embedded systems, IoT devices
- **Security Level**: 60-75/100
- **Performance Impact**: Low
- **Size Increase**: ~80-120%

## Target Platforms

### Linux
- Compiler: GCC
- Output: ELF binary
- Architecture: x64, ARM64

### Windows
- Compiler: MinGW-w64 GCC
- Output: PE executable (.exe)
- Architecture: x64

## Auto-Tuning

Auto-tuning uses iterative optimization to find the best obfuscation parameters for your code.

**How it works:**
1. Start with default configuration
2. Run obfuscation and measure metrics
3. Adjust parameters based on results
4. Repeat for N iterations
5. Select best configuration
6. Generate final obfuscated binary

**Metrics Optimized:**
- RE Difficulty Score (primary)
- Security Score
- Resilience Score
- Code size
- Performance overhead

**Recommended Iterations:**
- Quick test: 5 iterations
- Production: 10-15 iterations
- Maximum quality: 20-30 iterations

## HTML Report

Each obfuscation session generates a comprehensive HTML report including:

### Configuration Parameters
- Mode and platform
- Auto-tuning settings
- Config file used

### Input Files
- File paths
- Sizes
- Types

### Output Files
- Generated files
- Sizes
- Size increase percentage

### Obfuscation Details
- **Obfuscation Cycles**: Number of transformation passes
- **String Obfuscations**: Count of encrypted strings
- **Fake Functions**: Number of bogus functions inserted
- **Bogus Code Lines**: Lines of decoy code added

### Applied Techniques
- Control Flow Flattening
- String Encryption
- Instruction Substitution
- Bogus Control Flow
- Function Inlining
- Variable Renaming
- And more...

### Security Metrics
- RE Difficulty Score (0-100)
- Security Score (0-100)
- Resilience Score (0-100)
- Overall Fitness (0-100)

### Execution Summary
- Duration
- Status
- Files processed
- Warnings

## Output Structure

```
obfuscation_results/
└── session_<timestamp>/
    ├── obfuscation_report.html      # Comprehensive HTML report
    ├── <filename>_obfuscated.c      # Obfuscated source file(s)
    ├── <filename>_obfuscated         # Linux binary
    └── <filename>_obfuscated.exe    # Windows binary (if platform=windows)
```

## Prerequisites

- Node.js >= 14.0.0
- GCC compiler
- MinGW-w64 (for Windows cross-compilation)
- LLVM Obfuscator binary (built from source)

Check prerequisites with:
```bash
phantron check
```

## Examples

### Example 1: Basic Obfuscation

```bash
phantron obfuscate -i hello.c -p linux -m normal
```

Output:
- Obfuscated source: `obfuscation_results/session_<id>/hello_obfuscated.c`
- Linux binary: `obfuscation_results/session_<id>/hello_obfuscated`
- HTML report: `obfuscation_results/session_<id>/obfuscation_report.html`

### Example 2: Multi-File Project

```bash
phantron obfuscate -i "main.c,utils.c,crypto.c" -p windows -m security
```

Output:
- 3 obfuscated source files
- 3 Windows executables (.exe)
- Comprehensive HTML report

### Example 3: Auto-Tuned Maximum Security

```bash
phantron obfuscate -i sensitive.cpp -p linux -m security -a -n 15
```

Process:
1. Runs 15 optimization iterations
2. Tests different parameter combinations
3. Measures RE difficulty and security scores
4. Selects best configuration
5. Generates final obfuscated binary
6. Creates detailed HTML report with iteration history

### Example 4: Embedded System (Size-Optimized)

```bash
phantron obfuscate -i firmware.c -p linux -m size
```

Optimizes for:
- Minimal size increase
- Reasonable security level
- Low performance overhead

## Troubleshooting

### "File not found" error
Ensure input files exist and paths are correct. Use absolute paths if needed.

### "LLVM Obfuscator Binary not found"
Run `./build.sh` to build the obfuscator binary first.

### "GCC compiler not found"
Install GCC: `sudo apt-get install gcc` (Linux)

### "MinGW not found" (Windows cross-compilation)
Install MinGW: `sudo apt-get install mingw-w64` (Linux)

### Auto-tuning taking too long
Reduce iteration count with `-n <lower_number>`

### Report not generated
Check output directory permissions and disk space

## Configuration Files

Located in `config/` directory:

- **balanced.yaml**: Normal mode configuration
- **maximum_security.yaml**: Security focused mode
- **size_conservative.yaml**: Size conservative mode

You can customize these files to adjust obfuscation parameters.

## API (Programmatic Usage)

```javascript
const { ObfuscationEngine } = require('@phantron/obfuscator');

const options = {
  inputFiles: ['main.c', 'utils.c'],
  platform: 'linux',
  mode: 'security',
  autoTune: true,
  iterations: 10,
  configFile: 'config/maximum_security.yaml'
};

const engine = new ObfuscationEngine(options);
const result = await engine.execute();

console.log('Session ID:', result.sessionId);
console.log('Output directory:', result.outputDir);
console.log('Report:', result.reportPath);
```

## Performance Considerations

### Normal Mode
- Build time: ~10-30 seconds per file
- Runtime overhead: 10-30%
- Memory overhead: 15-25%

### Security Focused Mode
- Build time: ~30-60 seconds per file
- Runtime overhead: 30-100%
- Memory overhead: 30-50%

### Size Conservative Mode
- Build time: ~5-15 seconds per file
- Runtime overhead: 5-15%
- Memory overhead: 10-20%

### Auto-Tuning
- Additional time: 5-30 minutes depending on iterations
- Recommended for production builds only

## Security Considerations

- Always test obfuscated binaries thoroughly
- Verify functionality is preserved
- Measure performance impact on your use case
- Use security mode for sensitive code
- Consider auto-tuning for optimal protection
- Keep obfuscation keys/configs secure
- Don't rely solely on obfuscation for security

## Support

- Documentation: See `docs/` directory
- Issues: GitHub Issues
- Email: support@phantron.dev

## License

MIT License - see LICENSE file

## Credits

Developed by Phantron Team, 2025

---

**Phantron Obfuscator** - Professional Code Protection, Simplified.
