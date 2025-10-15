# Phantron Obfuscator - Distribution Guide

Complete guide for building and distributing standalone Phantron Obfuscator CLI across multiple platforms and architectures.

## 🎯 Supported Platforms

| Platform | Architecture | Status |
|----------|--------------|--------|
| **Linux** | x64 (AMD64) | ✅ Fully Supported |
| **Linux** | ARM64 | ✅ Fully Supported |
| **Windows** | x64 (AMD64) | ✅ Fully Supported |
| **Windows** | ARM64 | ✅ Fully Supported |

## 🏗️ Build System Overview

The build system creates standalone executables that bundle:
- Node.js runtime
- CLI code
- Phantron LLVM Obfuscator binary
- Configuration files
- Installation scripts

## 📋 Prerequisites

### For Linux Builds

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y \
    cmake \
    build-essential \
    clang-14 \
    llvm-14-dev \
    nodejs \
    npm \
    git

# Install pkg globally
sudo npm install -g pkg
```

### For Windows Builds

1. **Node.js** (v18+): https://nodejs.org/
2. **CMake**: https://cmake.org/download/
3. **Visual Studio Build Tools** (optional, for native compilation)
4. **WSL with Ubuntu 24.04** (for building Linux binaries on Windows)

```powershell
# Install pkg globally
npm install -g pkg
```

## 🚀 Building Distributions

### Method 1: Using Build Scripts (Recommended)

#### On Linux:

```bash
# Make script executable
chmod +x scripts/build/build-standalone.sh

# Run build
./scripts/build/build-standalone.sh
```

#### On Windows:

```powershell
# Run in PowerShell as Administrator
powershell -ExecutionPolicy Bypass -File scripts\build\build-standalone.ps1
```

#### Using NPM Scripts:

```bash
# On Linux
npm run build:standalone

# On Windows
npm run build:standalone:win
```

### Method 2: Manual Build

```bash
# Install dependencies
npm install
npm install -g pkg

# Build Phantron LLVM Obfuscator
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release --parallel
cd ..

# Build CLI executables
pkg . --targets node18-linux-x64 --output dist/phantron-linux-x64
pkg . --targets node18-linux-arm64 --output dist/phantron-linux-arm64
pkg . --targets node18-win-x64 --output dist/phantron-win-x64.exe
pkg . --targets node18-win-arm64 --output dist/phantron-win-arm64.exe
```

## 📦 Distribution Packages

After building, you'll find these packages in the `dist/` directory:

```
dist/
├── phantron-obfuscator-linux-x64.tar.gz       # Linux x64 package
├── phantron-obfuscator-linux-arm64.tar.gz     # Linux ARM64 package
├── phantron-obfuscator-win-x64.zip            # Windows x64 package
├── phantron-obfuscator-win-arm64.zip          # Windows ARM64 package
└── SHA256SUMS.txt                             # Checksums for verification
```

### Package Contents

Each package contains:
- `phantron` or `phantron.exe` - Standalone CLI executable
- `phantron-llvm-obfuscator` - Native obfuscation engine
- `config/` - Configuration files (balanced, security, size modes)
- `install.sh` or `install.bat` - Automated installer
- `README.txt` - Quick start guide

## 📥 Installation

### Linux

```bash
# Extract
tar -xzf phantron-obfuscator-linux-x64.tar.gz
cd phantron-obfuscator-linux-x64

# Install
sudo ./install.sh

# Verify
phantron --version
```

### Windows

1. Extract the ZIP file
2. Right-click `install.bat` and select **"Run as Administrator"**
3. Restart your terminal
4. Run `phantron --version`

### Manual Installation

#### Linux:
```bash
sudo cp phantron /usr/local/bin/
sudo cp phantron-llvm-obfuscator /usr/local/bin/
sudo chmod +x /usr/local/bin/phantron*
sudo mkdir -p /usr/local/share/phantron
sudo cp -r config /usr/local/share/phantron/
```

#### Windows (PowerShell as Administrator):
```powershell
Copy-Item phantron.exe "$env:ProgramFiles\Phantron\"
Copy-Item phantron-llvm-obfuscator.exe "$env:ProgramFiles\Phantron\" -ErrorAction SilentlyContinue
Copy-Item -Recurse config "$env:ProgramFiles\Phantron\"
$env:PATH += ";$env:ProgramFiles\Phantron"
[Environment]::SetEnvironmentVariable("PATH", $env:PATH, "Machine")
```

## 🧪 Testing Distributions

After installation, test with:

```bash
# Check version
phantron --version

# View help
phantron --help

# Test obfuscation
echo 'int main() { return 0; }' > test.c
phantron obfuscate test.c -o test_obf.c

# Interactive mode
phantron interactive
```

## 🔧 Build Customization

### Custom Build Targets

Edit `package.json` to add/remove targets:

```json
{
  "pkg": {
    "targets": [
      "node18-linux-x64",
      "node18-linux-arm64",
      "node18-win-x64",
      "node18-win-arm64",
      "node18-macos-x64",     // Add macOS
      "node18-macos-arm64"    // Add macOS ARM
    ]
  }
}
```

### Cross-Compilation

For cross-architecture builds, use Docker:

```dockerfile
# Dockerfile.builder
FROM node:18-slim
RUN apt-get update && apt-get install -y cmake build-essential
WORKDIR /app
COPY . .
RUN npm install && npm run build:standalone
```

```bash
docker build -f Dockerfile.builder -t phantron-builder .
docker run --rm -v $(pwd)/dist:/app/dist phantron-builder
```

## 📊 Build Sizes

Approximate sizes for reference:

| Package | Compressed | Extracted |
|---------|-----------|-----------|
| Linux x64 | ~40 MB | ~120 MB |
| Linux ARM64 | ~40 MB | ~120 MB |
| Windows x64 | ~35 MB | ~100 MB |
| Windows ARM64 | ~35 MB | ~100 MB |

## 🐛 Troubleshooting

### Build Issues

**"pkg not found"**
```bash
npm install -g pkg
```

**"LLVM not found"**
```bash
# Linux
sudo apt-get install llvm-14-dev clang-14

# Check version
llvm-config-14 --version
```

**"Permission denied"**
```bash
chmod +x scripts/build/build-standalone.sh
```

### Distribution Issues

**"Command not found" after installation**
- Restart your terminal
- Check PATH: `echo $PATH` (Linux) or `echo $env:PATH` (Windows)
- Verify binary location: `which phantron` (Linux) or `where phantron` (Windows)

**"DLL not found" (Windows)**
- Install Visual C++ Redistributable: https://aka.ms/vs/17/release/vc_redist.x64.exe

**"Permission denied" (Linux)**
```bash
chmod +x /usr/local/bin/phantron*
```

## 🌐 Distribution Channels

### GitHub Releases

Upload packages as release assets:

```bash
# Create release
gh release create v2.0.5 \
  dist/phantron-obfuscator-*.tar.gz \
  dist/phantron-obfuscator-*.zip \
  dist/SHA256SUMS.txt \
  --title "Phantron Obfuscator v2.0.5" \
  --notes "Release notes here"
```

### NPM Registry

The CLI is also available via npm:

```bash
npm install -g phantron-obfuscator
```

### Docker Hub

```bash
docker pull phantron/obfuscator:latest
docker run --rm -v $(pwd):/workspace phantron/obfuscator obfuscate input.c
```

## 📝 Release Checklist

- [ ] Update version in `package.json`
- [ ] Update `CHANGELOG.md`
- [ ] Run full build for all platforms
- [ ] Test each distribution package
- [ ] Generate and verify checksums
- [ ] Create git tag: `git tag v2.0.5`
- [ ] Push tag: `git push origin v2.0.5`
- [ ] Create GitHub release with packages
- [ ] Publish to npm: `npm publish`
- [ ] Update documentation
- [ ] Announce release

## 🔐 Security

### Verify Downloads

```bash
# Verify checksum
sha256sum -c SHA256SUMS.txt

# Or individually
sha256sum phantron-obfuscator-linux-x64.tar.gz
```

### Code Signing (Future)

- Linux: GPG signatures
- Windows: Authenticode signing
- macOS: Apple notarization

## 📞 Support

- **Issues**: https://github.com/NeuralNinja110/SIH25_2/issues
- **Discussions**: https://github.com/NeuralNinja110/SIH25_2/discussions
- **Email**: support@phantron.dev

## 📄 License

MIT License - See LICENSE file for details.

---

**Built with ❤️ by Team ELEVATE_1**
