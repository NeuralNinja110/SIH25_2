# Phantron Obfuscator v2.0.5 - Standalone Distributions

## 🎉 What's New

### Standalone CLI Distributions
Complete standalone executables with no dependencies required! Download, extract, and run immediately.

### Consistent Branding
- Renamed to **Phantron LLVM Obfuscator** throughout
- Professional, consistent naming across all components
- Clear distinction between CLI wrapper and native binary

### Repository Cleanup
- Removed 96+ unnecessary files
- Professional structure for open-source distribution
- Only essential documentation (README.md + INSTALLATION.md)

## 📦 Distribution Packages

Choose the package for your platform:

### Windows

**Windows x64 (Intel/AMD 64-bit)**
- `phantron-obfuscator-windows-x64-v2.0.5.zip` (18.22 MB)
- For: Most Windows PCs and laptops
- Installation: Extract ZIP → Run `install.bat` as Administrator

**Windows ARM64**
- `phantron-obfuscator-windows-arm64-v2.0.5.zip` (13.22 MB)
- For: Surface X, Windows on ARM devices
- Installation: Extract ZIP → Run `install.bat` as Administrator

### Linux

**Linux x64 (Intel/AMD 64-bit)**
- `phantron-obfuscator-linux-x64-v2.0.5.tar.gz` (21.46 MB)
- For: Most Linux systems (Ubuntu, Debian, Fedora, etc.)
- Installation: `tar -xzf *.tar.gz && cd phantron-* && sudo ./install.sh`

**Linux ARM64**
- `phantron-obfuscator-linux-arm64-v2.0.5.tar.gz` (21.27 MB)
- For: Raspberry Pi 4+, AWS Graviton, ARM servers
- Installation: `tar -xzf *.tar.gz && cd phantron-* && sudo ./install.sh`

## ✨ Features

### Advanced Obfuscation Techniques (14+)
- ✅ Control Flow Flattening
- ✅ Bogus Control Flow
- ✅ Opaque Predicates (Classical & Quantum-Inspired)
- ✅ Instruction Substitution
- ✅ Dead Code Injection
- ✅ String & Constant Obfuscation
- ✅ Mixed Boolean-Arithmetic (MBA)
- ✅ Function Virtualization
- ✅ Call Graph Obfuscation
- ✅ Anti-Debugging Techniques
- ✅ Hardware Cache Obfuscation
- ✅ Grammar-Based Metamorphic Transformations

### Multi-Layered Systems
- **MAOS**: Multi-Layered Adaptive Obfuscation System
- **QIRL**: Quantum-Inspired Resistance Layer
- **ATIE**: AI-Tuned Intelligent Engine
- **PCGE**: Polymorphic Code Generation Engine

### Professional Features
- ✅ Auto-tuning for optimal protection
- ✅ Comprehensive HTML reporting
- ✅ Multi-file batch processing
- ✅ Cross-platform support
- ✅ No dependencies (standalone)

## 🚀 Quick Start

### Windows
```cmd
# 1. Download phantron-obfuscator-windows-x64-v2.0.5.zip
# 2. Extract the ZIP file
# 3. Right-click install.bat and "Run as Administrator"
# 4. Restart your terminal
# 5. Run:
phantron --version
```

### Linux
```bash
# 1. Download phantron-obfuscator-linux-x64-v2.0.5.tar.gz
# 2. Extract and install:
tar -xzf phantron-obfuscator-linux-x64-v2.0.5.tar.gz
cd phantron-obfuscator-linux-x64-v2.0.5
sudo ./install.sh

# 3. Verify installation:
phantron --version
```

## 📝 Usage Examples

### Basic Obfuscation
```bash
phantron obfuscate input.c -o output.c
```

### Security Mode
```bash
phantron obfuscate input.c -o output.c --mode security
```

### Interactive Mode
```bash
phantron interactive
```

### Auto-Tune
```bash
phantron auto-tune input.c
```

### Batch Processing
```bash
phantron batch src/*.c -o obfuscated/
```

## 📊 System Requirements

**Minimum:**
- 2GB RAM
- 500MB disk space
- Windows 10/11 or Linux (kernel 4.x+)

**Recommended:**
- 4GB+ RAM
- 1GB disk space
- SSD storage

## 🔐 Verification

Verify package integrity using SHA256 checksums:

```bash
sha256sum -c SHA256SUMS.txt
```

Or check individual files against `SHA256SUMS.txt`.

## 📚 Documentation

- **Installation Guide**: See `INSTALLATION.md` in repository
- **Distribution Guide**: See `DISTRIBUTION.md` in repository
- **API Reference**: Run `phantron --help` after installation
- **GitHub Repository**: https://github.com/NeuralNinja110/SIH25_2

## 💾 Alternative Installation

### Via NPM (Requires Node.js)
```bash
npm install -g phantron-obfuscator
```

### From Source
```bash
git clone https://github.com/NeuralNinja110/SIH25_2.git
cd SIH25_2
npm install
npm run build
```

## 🐛 Known Issues

None at this time. Report issues at: https://github.com/NeuralNinja110/SIH25_2/issues

## 👥 Team

Developed by **Team ELEVATE_1**:
- Rahul Ashok - [LinkedIn](https://www.linkedin.com/in/rahul19a/)
- Pritham Devaprasad - [LinkedIn](https://www.linkedin.com/in/pritham-devaprasad)
- Anish R - [LinkedIn](https://www.linkedin.com/in/anish-raghavendr)
- Siddarth S - [LinkedIn](https://www.linkedin.com/in/siddarth-s-a572b926b/)
- Sahana Mutharasan
- Sanjana Suresh

## 📄 License

MIT License - Copyright (c) 2025 Team ELEVATE_1

## 🔗 Links

- **NPM Package**: https://www.npmjs.com/package/phantron-obfuscator
- **GitHub**: https://github.com/NeuralNinja110/SIH25_2
- **Issues**: https://github.com/NeuralNinja110/SIH25_2/issues
- **Discussions**: https://github.com/NeuralNinja110/SIH25_2/discussions

---

**Full Changelog**: https://github.com/NeuralNinja110/SIH25_2/commits/v2.0.5

**Built with ❤️ by Team ELEVATE_1**
