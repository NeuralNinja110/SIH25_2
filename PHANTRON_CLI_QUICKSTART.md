# Phantron CLI - Quick Start Guide

## Installation

```bash
cd /workspaces/SIH25_2
npm install
```

## Basic Usage

### 1. Check Prerequisites
```bash
node cli/index.js check
```

### 2. Simple Obfuscation
```bash
node cli/index.js obfuscate -i input.c -p linux -m normal
```

### 3. Security Focused
```bash
node cli/index.js obfuscate -i app.cpp -p windows -m security
```

### 4. Auto-Tuning (5 iterations)
```bash
node cli/index.js obfuscate -i src.c -p linux -m security -a -n 5
```

### 5. Multiple Files
```bash
node cli/index.js obfuscate -i "file1.c,file2.c,file3.c" -p linux -m normal
```

### 6. Interactive Mode
```bash
node cli/index.js interactive
```

## Command Options

```
-i, --input <files>     Input C/C++ file(s) (comma-separated)
-p, --platform <type>   Target platform: linux or windows
-m, --mode <mode>       Obfuscation mode: normal, security, or size
-a, --auto-tune         Enable auto-tuning
-n, --iterations <n>    Number of auto-tune iterations (default: 5)
-h, --help              Display help
```

## Modes

| Mode | Focus | Config File | Best For |
|------|-------|-------------|----------|
| **normal** | Balanced | balanced.yaml | General use |
| **security** | Maximum protection | maximum_security.yaml | Sensitive code |
| **size** | Minimal size increase | size_conservative.yaml | Embedded systems |

## Output

All results are saved to: `obfuscation_results/session_XXXXX/`

Contents:
- `*_obfuscated` or `*.exe` - Obfuscated binary
- `obfuscation_report.html` - Detailed HTML report
- `obfuscation_report.json` - JSON metrics
- `best_config.yaml` - Optimal config (auto-tune only)
- `config_iterN.yaml` - Iteration configs (auto-tune only)

## Example Session

```bash
$ node cli/index.js obfuscate -i test.c -p linux -m security -a -n 3

Obfuscation Configuration
─────────────────────────
┌─────────────────────────┬───────────────────────┐
│ Input Files             │ 1 file(s)             │
│ Platform                │ Linux                 │
│ Mode                    │ Security Focused      │
│ Auto-Tuning             │ Enabled (3 iterations)│
└─────────────────────────┴───────────────────────┘

[INFO] Starting auto-tuning with 3 iterations
[INFO] Iteration 1/3
[INFO] Executing: llvm-obfuscator
[INFO] Iteration 1 fitness score: 64.50
[SUCCESS] New best configuration found!
[INFO] Iteration 2/3
[INFO] Executing: llvm-obfuscator
[INFO] Iteration 2 fitness score: 64.50
[INFO] Iteration 3/3
[INFO] Executing: llvm-obfuscator
[INFO] Running final obfuscation with best configuration...

✔ Obfuscation completed successfully
✔ Compiled for Linux
✔ Metrics collected
✔ Report generated

╭────────────────────────────────────────────────╮
│   OBFUSCATION COMPLETED SUCCESSFULLY           │
│                                                │
│   Session ID: session_1760395143645            │
│   Output Directory: obfuscation_results/...    │
│   Report: obfuscation_report.html              │
╰────────────────────────────────────────────────╯
```

## Testing the Output

```bash
# Linux
$ ./obfuscation_results/session_XXXXX/test_obfuscated
Hello, World!

# Windows (on Windows or Wine)
$ ./obfuscation_results/session_XXXXX/test_obfuscated.exe
Hello, World!
```

## Viewing Reports

```bash
# Open HTML report in browser
$ xdg-open obfuscation_results/session_XXXXX/obfuscation_report.html

# Or view JSON metrics
$ cat obfuscation_results/session_XXXXX/obfuscation_report.json | jq
```

## Auto-Tuning Explained

Auto-tuning runs multiple iterations with different parameter configurations:

1. **Iteration 1-N**: Tests different parameter values
   - Adjusts obfuscation cycles (3 → 10)
   - Modifies control flow flattening probability
   - Tunes string encryption intensity
   - Varies bogus control flow density

2. **Fitness Calculation**: Scores each configuration
   - Security mode: 60% security, 10% size, 30% performance
   - Size mode: 40% security, 40% size, 20% performance
   - Normal mode: 40% security, 20% size, 40% performance

3. **Best Selection**: Picks highest-scoring configuration
   - Saves as `best_config.yaml`
   - Uses for final obfuscation run

4. **Output**: Generates all iteration binaries + final optimized binary

## Configuration Files

Located in `config/` directory:

- `balanced.yaml` - Normal mode (45% security, 35% performance, 20% size)
- `maximum_security.yaml` - Security mode (60% security, 30% performance, 10% size)
- `size_conservative.yaml` - Size mode (50% security, 20% performance, 30% size)

You can edit these files to customize obfuscation parameters.

## Troubleshooting

### Binary Not Found
```bash
$ node cli/index.js check
# Verify "LLVM Obfuscator Binary" shows "PASS"
```

### Compilation Failed
- Check input file is valid C/C++
- Ensure no syntax errors
- Try with verbose mode: add `--verbose` flag

### Auto-Tuning Takes Too Long
- Reduce iterations: `-n 3` instead of `-n 10`
- Use faster mode: `-m normal` instead of `-m security`

### Report Not Generated
- Check disk space: `df -h`
- Verify output directory permissions
- Look for errors in terminal output

## Advanced Usage

### Custom Config Path
```bash
node cli/index.js obfuscate -i input.c -p linux -c path/to/custom.yaml
```

### Batch Processing
```bash
for file in src/*.c; do
  node cli/index.js obfuscate -i "$file" -p linux -m security
done
```

### Scripted Auto-Tune
```bash
#!/bin/bash
for iterations in 3 5 10; do
  echo "Testing with $iterations iterations..."
  node cli/index.js obfuscate -i test.c -p linux -m security -a -n $iterations
done
```

## What's Actually Happening

Unlike typical CLI demos, Phantron performs **real obfuscation**:

1. **Reads YAML Config**: Parses obfuscation parameters
2. **Calls LLVM Binary**: Executes actual obfuscator
3. **Applies Transformations**: Real code transformations occur
4. **Compiles Binary**: Generates working executable
5. **Collects Metrics**: Parses real transformation counts
6. **Generates Report**: Creates detailed HTML documentation

This is **NOT a simulation** - it's a production-ready obfuscation tool!

## Next Steps

1. Read `CLI_COMPLETE_IMPLEMENTATION.md` for technical details
2. Review `PHANTRON_README.md` for comprehensive documentation
3. Check `CLI_UPGRADE_SUMMARY.md` for implementation history
4. Explore `config/*.yaml` files to customize obfuscation

## Support

For issues or questions:
- Check documentation in `/docs` directory
- Review generated HTML reports for details
- Examine JSON reports for raw metrics
- Test with simple programs first

---

**Ready to obfuscate your code? Start with:**
```bash
node cli/index.js interactive
```
