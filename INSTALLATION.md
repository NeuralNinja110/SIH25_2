# Installation Guide

Complete installation and setup instructions for Phantron Obfuscator.

## Table of Contents

- [System Requirements](#system-requirements)
- [Installation Methods](#installation-methods)
- [Post-Installation Setup](#post-installation-setup)
- [Verification](#verification)
- [Troubleshooting](#troubleshooting)
- [Updating](#updating)
- [Uninstallation](#uninstallation)

## System Requirements

### Minimum Requirements

- **Node.js**: Version 14.0.0 or higher
- **npm**: Version 6.0.0 or higher
- **Operating System**: Linux or macOS (Windows support via WSL)
- **RAM**: 2 GB minimum, 4 GB recommended
- **Disk Space**: 500 MB for installation and working files

### Required Compilers

#### For Linux Targets
- **GCC**: Version 7.0 or higher
- Install on Ubuntu/Debian: `sudo apt-get install build-essential`
- Install on Fedora/RHEL: `sudo dnf install gcc gcc-c++`

#### For Windows Targets (Cross-Compilation)
- **MinGW-w64**: For Windows binary generation on Linux
- Install on Ubuntu/Debian: `sudo apt-get install mingw-w64`
- Install on Fedora/RHEL: `sudo dnf install mingw64-gcc`

### Check Current Versions

```bash
# Check Node.js version
node --version

# Check npm version
npm --version

# Check GCC version
gcc --version

# Check MinGW version (if installed)
x86_64-w64-mingw32-gcc --version
```

## Installation Methods

### Method 1: Global Installation (Recommended)

Global installation makes the `phantron` command available system-wide.

```bash
npm install -g phantron-obfuscator
```

#### Benefits of Global Installation
- Command available from any directory
- No need to specify path to executable
- Ideal for regular use as a development tool

#### Installation Location
- Linux/macOS: `/usr/local/lib/node_modules/phantron-obfuscator`
- Windows: `%APPDATA%\npm\node_modules\phantron-obfuscator`

### Method 2: Local Installation

Local installation adds Phantron to a specific project.

```bash
# In your project directory
npm install phantron-obfuscator
```

#### Using Local Installation

```bash
# Via npx (recommended)
npx phantron obfuscate -i input.c -p linux -m normal

# Via package.json scripts
# Add to package.json:
{
  "scripts": {
    "obfuscate": "phantron obfuscate"
  }
}

# Then run:
npm run obfuscate -- -i input.c -p linux -m normal
```

### Method 3: From Source (Development)

For development or contributing to Phantron.

```bash
# Install from npm
npm install phantron-obfuscator

# Install dependencies
npm install

# Link globally for testing
npm link

# Now you can use phantron command globally
phantron --version
```

## Post-Installation Setup

### 1. Verify Installation

```bash
# Check installed version
phantron --version

# Run prerequisite check
phantron check
```

Expected output:
```
Prerequisites Check
-------------------
Node.js                     PASS    v14.x.x
Phantron LLVM Obfuscator    PASS    Found
Configuration Files         PASS    All found
GCC Compiler                PASS    Available
```

### 2. Test Basic Functionality

Create a test file:

```bash
# Create a simple C file
cat > test.c << 'EOF'
#include <stdio.h>
int main() {
    printf("Hello from Phantron!\n");
    return 0;
}
EOF

# Obfuscate it
phantron obfuscate -i test.c -p linux -m normal

# Check the output
ls obfuscation_results/session_*/
```

### 3. Configure Shell Completion (Optional)

For bash:
```bash
# Add to ~/.bashrc
eval "$(phantron --completion bash)"
```

For zsh:
```bash
# Add to ~/.zshrc
eval "$(phantron --completion zsh)"
```

## Verification

### Complete System Check

Run the comprehensive prerequisite checker:

```bash
phantron check
```

This verifies:
- Node.js version compatibility
- npm package installation
- Phantron LLVM Obfuscator binary availability
- Configuration files integrity
- GCC compiler availability
- MinGW cross-compiler (if needed)

### Manual Verification Steps

#### 1. Check Binary Availability

```bash
# Find where phantron is installed
which phantron

# Check if it's executable
ls -la $(which phantron)
```

#### 2. Verify Config Files

```bash
# List configuration files
ls -la $(npm root -g)/phantron-obfuscator/config/

# Expected files:
# - balanced.yaml
# - maximum_security.yaml
# - size_conservative.yaml
```

#### 3. Test Each Command

```bash
# Test help
phantron --help

# Test examples
phantron examples

# Test interactive mode
phantron interactive
# (Press Ctrl+C to exit)
```

## Troubleshooting

### Issue: Command Not Found

**Symptom**: `phantron: command not found`

**Solutions**:

1. **Check if installed globally**:
   ```bash
   npm list -g phantron-obfuscator
   ```

2. **Verify npm global path**:
   ```bash
   npm config get prefix
   ```
   Add this to your PATH if not present.

3. **Reinstall**:
   ```bash
   npm uninstall -g phantron-obfuscator
   npm install -g phantron-obfuscator
   ```

### Issue: Permission Denied

**Symptom**: `EACCES: permission denied`

**Solutions**:

1. **Use sudo (not recommended)**:
   ```bash
   sudo npm install -g phantron-obfuscator
   ```

2. **Fix npm permissions (recommended)**:
   ```bash
   mkdir ~/.npm-global
   npm config set prefix '~/.npm-global'
   echo 'export PATH=~/.npm-global/bin:$PATH' >> ~/.bashrc
   source ~/.bashrc
   npm install -g phantron-obfuscator
   ```

### Issue: LLVM Binary Not Found

**Symptom**: `Phantron LLVM Obfuscator binary not found`

**Solution**:
The binary should be included in the package. Verify:

```bash
ls $(npm root -g)/phantron-obfuscator/build/llvm-obfuscator
```

If missing, reinstall:
```bash
npm reinstall -g phantron-obfuscator
```

### Issue: GCC Not Found

**Symptom**: `GCC compiler not available`

**Solutions**:

Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install build-essential
```

Fedora/RHEL:
```bash
sudo dnf install gcc gcc-c++
```

macOS:
```bash
xcode-select --install
```

### Issue: MinGW Not Found (Windows Targets)

**Symptom**: Cannot generate Windows executables

**Solution**:

Ubuntu/Debian:
```bash
sudo apt-get install mingw-w64
```

Fedora/RHEL:
```bash
sudo dnf install mingw64-gcc mingw64-gcc-c++
```

### Issue: Node Version Too Old

**Symptom**: Requires node version >= 14.0.0

**Solutions**:

Using nvm (recommended):
```bash
# Install nvm
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.0/install.sh | bash

# Install latest LTS
nvm install --lts

# Use it
nvm use --lts

# Verify
node --version
```

Using package manager:
```bash
# Ubuntu/Debian
curl -fsSL https://deb.nodesource.com/setup_lts.x | sudo -E bash -
sudo apt-get install -y nodejs

# Verify
node --version
```

## Updating

### Check for Updates

```bash
# Check current version
phantron --version

# Check for newer versions
npm outdated -g phantron-obfuscator
```

### Update to Latest Version

```bash
# Global installation
npm update -g phantron-obfuscator

# Local installation
npm update phantron-obfuscator
```

### Update to Specific Version

```bash
npm install -g phantron-obfuscator@2.1.0
```

## Uninstallation

### Remove Global Installation

```bash
npm uninstall -g phantron-obfuscator
```

### Remove Local Installation

```bash
# In your project directory
npm uninstall phantron-obfuscator
```

### Clean Up Generated Files

```bash
# Remove obfuscation results
rm -rf obfuscation_results/

# Remove any generated configs
rm -rf auto_tune_results/
```

### Verify Removal

```bash
# Should return error if successfully removed
phantron --version

# Check npm list
npm list -g phantron-obfuscator
```

## Next Steps

After successful installation:

1. **Review Documentation**: See the comprehensive guides at the Gist links
2. **Try Examples**: Run `phantron examples` for common usage patterns
3. **Explore Modes**: Test different obfuscation modes with sample files
4. **Enable Auto-Tuning**: Experiment with auto-tuning for optimal results

## Getting Help

If you encounter issues not covered in this guide:

- **Check Prerequisites**: Run `phantron check`
- **View Examples**: Run `phantron examples`
- **NPM Package**: [https://www.npmjs.com/package/phantron-obfuscator](https://www.npmjs.com/package/phantron-obfuscator)

---

**Last Updated**: October 2025  
**Version**: 2.0.0
