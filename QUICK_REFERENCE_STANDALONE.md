# 🚀 Quick Reference: Standalone Distributions

## File Overview

```
dist/
├── phantron-win-x64.exe        (35.83 MB) - Windows Intel/AMD 64-bit
├── phantron-win-arm64.exe      (25.83 MB) - Windows ARM64
├── phantron-linux-x64          (44.06 MB) - Linux Intel/AMD 64-bit
├── phantron-linux-arm64        (42.66 MB) - Linux ARM64
├── phantron-llvm-obfuscator             (12.36 MB) - Phantron LLVM Obfuscator (Linux)
├── install.bat                          - Windows installer
├── install.sh                           - Linux installer
├── README.txt                           - User guide
└── config/                              - Configuration files
    ├── balanced.yaml
    ├── maximum_security.yaml
    └── size_conservative.yaml
```

## Quick Install

### Windows
```cmd
# As Administrator
.\install.bat
```

### Linux
```bash
sudo ./install.sh
```

## Quick Usage

```bash
# Help
phantron --help

# Basic obfuscation
phantron obfuscate input.c -o output.c

# Interactive mode
phantron interactive

# Auto-tune
phantron auto-tune input.c
```

## Documentation

- **`STANDALONE_BUILDS_COMPLETE.md`** - Full summary
- **`DISTRIBUTION.md`** - Distribution guide  
- **`BUILD_COMPLETE.md`** - Build details
- **`dist/README.txt`** - User guide

## Links

- **GitHub:** https://github.com/NeuralNinja110/SIH25_2
- **NPM:** https://www.npmjs.com/package/phantron-obfuscator
- **Issues:** https://github.com/NeuralNinja110/SIH25_2/issues

---

**Team ELEVATE_1** | MIT License | v2.0.5
