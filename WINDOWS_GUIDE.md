# 🪟 MAOS - Windows Installation & Usage Guide

## System Requirements

### Required Software
- **Windows 10/11** (64-bit)
- **Visual Studio 2019 or 2022** with C++ desktop development tools
- **CMake 3.20+** - [Download](https://cmake.org/download/)
- **LLVM 18.1.3** - [Download](https://github.com/llvm/llvm-project/releases)
- **Git for Windows** (optional) - [Download](https://git-scm.com/download/win)

### Optional Tools
- **Ninja Build System** (faster builds) - [Download](https://github.com/ninja-build/ninja/releases)
- **Python 3.8+** (for advanced analysis) - [Download](https://www.python.org/downloads/)

---

## Installation Steps

### Step 1: Install Visual Studio

1. Download **Visual Studio Community 2022** from: https://visualstudio.microsoft.com/downloads/
2. During installation, select:
   - ✅ **Desktop development with C++**
   - ✅ **C++ CMake tools for Windows**
   - ✅ **Windows 10/11 SDK**

### Step 2: Install CMake

1. Download CMake installer: https://cmake.org/download/
2. Run installer and select:
   - ✅ **Add CMake to system PATH for all users**
3. Verify installation:
   ```cmd
   cmake --version
   ```

### Step 3: Install LLVM

**Option A: Pre-built Binary (Recommended)**
1. Download LLVM 18.1.3 Windows installer
2. Install to: `C:\Program Files\LLVM`
3. Add to PATH:
   ```cmd
   setx PATH "%PATH%;C:\Program Files\LLVM\bin"
   ```

**Option B: Build from Source**
```cmd
git clone --depth 1 --branch llvmorg-18.1.3 https://github.com/llvm/llvm-project.git
cd llvm-project
mkdir build && cd build
cmake -G "Visual Studio 17 2022" -A x64 -DLLVM_ENABLE_PROJECTS="clang" ..\llvm
cmake --build . --config Release
```

### Step 4: Download MAOS

**Option A: Using Git**
```cmd
git clone https://github.com/NeuralNinja110/SIH25_2.git
cd SIH25_2
```

**Option B: Download ZIP**
1. Download from: https://github.com/NeuralNinja110/SIH25_2/archive/main.zip
2. Extract to: `C:\Users\YourName\MAOS`

---

## Building MAOS

### Method 1: Using Build Script (Recommended)

Open **Visual Studio Developer Command Prompt** and run:

```cmd
cd C:\Users\YourName\MAOS
build_windows.bat
```

This script will:
- ✅ Detect your build environment
- ✅ Configure CMake automatically
- ✅ Build with optimal settings
- ✅ Verify the installation

### Method 2: Manual Build

**Using Visual Studio IDE:**
1. Open Visual Studio 2022
2. Select: **Open a local folder**
3. Navigate to MAOS project folder
4. Visual Studio will auto-detect CMake
5. Build → Build All (Ctrl+Shift+B)

**Using Command Line:**
```cmd
REM Open Visual Studio Developer Command Prompt

cd C:\Users\YourName\MAOS
mkdir build
cd build

REM Configure
cmake -G "Visual Studio 17 2022" -A x64 ..

REM Build
cmake --build . --config Release --parallel
```

### Method 3: Using Ninja (Fastest)

```cmd
REM Install Ninja first
winget install Ninja-build.Ninja

REM Open Visual Studio Developer Command Prompt
cd C:\Users\YourName\MAOS
mkdir build && cd build

REM Configure with Ninja
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..

REM Build
ninja
```

---

## Verification

After building, verify installation:

```cmd
quick_test_windows.bat
```

Or manually:

```cmd
cd build\Release
llvm-obfuscator.exe --version
llvm-obfuscator.exe --help
```

---

## Running Tests

### Quick Test (30 seconds)

```cmd
quick_test_windows.bat
```

### Comprehensive Test Suite (5-10 minutes)

```cmd
run_tests_windows.bat
```

This runs:
- ✅ Normal obfuscation tests
- ✅ Size-conservative mode tests
- ✅ Maximum-security mode tests
- ✅ All 8+ test cases
- ✅ Generates detailed reports

### Scientific Testing (15-20 minutes)

```cmd
run_scientific_tests_windows.bat
```

This performs:
- ✅ Baseline measurements
- ✅ Size-conservative validation
- ✅ Maximum-security validation
- ✅ Comparative analysis
- ✅ JSON metrics export

---

## Usage Examples

### Basic Usage

```cmd
REM Navigate to project directory
cd C:\Users\YourName\MAOS

REM Set path to obfuscator
set OBFUSCATOR=build\Release\llvm-obfuscator.exe

REM Simple obfuscation
%OBFUSCATOR% -i tests\test_easy.c -o output.exe

REM With options
%OBFUSCATOR% -i myapp.c -o protected.exe -l 3 -C 5

REM Show help
%OBFUSCATOR% --help
```

### Size-Conservative Mode

```cmd
%OBFUSCATOR% -i myapp.c -o protected.exe -c config\size_conservative.yaml
```

**Features:**
- ✅ ≤15% size increase
- ✅ ≤5% performance overhead
- ✅ Good security (72/100)
- **Best for:** Mobile apps, embedded systems

### Maximum-Security Mode

```cmd
%OBFUSCATOR% -i myapp.c -o protected.exe -c config\maximum_security.yaml
```

**Features:**
- ✅ 95/100 security score
- ✅ All transformations enabled
- ✅ ML-guided optimization
- **Best for:** High-value IP, DRM

### Batch Processing

```cmd
REM Create batch script
@echo off
for %%f in (src\*.c) do (
    echo Processing %%f...
    build\Release\llvm-obfuscator.exe -i %%f -o bin\%%~nf.exe
)
echo Done!
```

---

## Configuration Files

### Location

```
MAOS\
├── config\
│   ├── size_conservative.yaml
│   └── maximum_security.yaml
```

### Creating Custom Config

Create `config\my_config.yaml`:

```yaml
mode: custom
level: medium
cycles: 3

passes:
  - instruction_substitution
  - string_encryption
  - constant_obfuscation
  - dead_code_injection

maos:
  enable_atie: true
  enable_qirl: true
  enable_pcge: true

genetic_algorithm:
  population_size: 30
  generations: 100
  mutation_rate: 0.15
```

Use it:
```cmd
%OBFUSCATOR% -i myapp.c -o protected.exe -c config\my_config.yaml
```

---

## Troubleshooting

### Issue 1: "cmake not found"

**Solution:**
```cmd
REM Add CMake to PATH
setx PATH "%PATH%;C:\Program Files\CMake\bin"

REM Restart terminal and verify
cmake --version
```

### Issue 2: "Visual Studio not found"

**Solution:**
Open **Visual Studio Developer Command Prompt** instead of regular CMD:
- Start Menu → Visual Studio 2022 → Developer Command Prompt

Or manually initialize:
```cmd
"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
```

### Issue 3: "LLVM not found"

**Solution:**
```cmd
REM Check LLVM installation
where llvm-config

REM If not found, add to PATH
setx PATH "%PATH%;C:\Program Files\LLVM\bin"
```

### Issue 4: Build fails with "MSB8036"

**Error:** Windows SDK not found

**Solution:**
1. Open Visual Studio Installer
2. Modify installation
3. Select: **Windows 10/11 SDK**
4. Install and retry build

### Issue 5: "Access denied" errors

**Solution:**
Run Command Prompt as Administrator:
- Right-click CMD → Run as Administrator

### Issue 6: Slow builds

**Solution:**
Use Ninja for faster builds:
```cmd
REM Install Ninja
winget install Ninja-build.Ninja

REM Build with Ninja
cd build
cmake -G Ninja ..
ninja
```

### Issue 7: Antivirus blocks obfuscator

**Solution:**
Add exception for:
- `build\Release\llvm-obfuscator.exe`
- MAOS project folder

---

## Performance Optimization

### Multi-Core Builds

```cmd
REM Use all CPU cores
cmake --build build --config Release --parallel

REM Or specify core count
cmake --build build --config Release -j 8
```

### Release vs Debug

**Release Build (Recommended for production):**
```cmd
cmake -DCMAKE_BUILD_TYPE=Release ..
```
- ✅ Optimized code
- ✅ Faster execution
- ✅ Smaller binary

**Debug Build (For development):**
```cmd
cmake -DCMAKE_BUILD_TYPE=Debug ..
```
- ✅ Debug symbols
- ✅ Better error messages
- ❌ Slower execution

---

## Integration with IDEs

### Visual Studio 2022

1. Open Visual Studio
2. File → Open → Folder
3. Select MAOS directory
4. CMake will auto-configure
5. Select build target: **llvm-obfuscator**
6. Build → Build All (Ctrl+Shift+B)

### Visual Studio Code

1. Install extensions:
   - C/C++ (Microsoft)
   - CMake Tools (Microsoft)
2. Open MAOS folder
3. Press F1 → CMake: Configure
4. Press F1 → CMake: Build

### CLion

1. Open MAOS project
2. CLion auto-detects CMake
3. Build → Build Project (Ctrl+F9)

---

## CI/CD Integration

### GitHub Actions

Create `.github\workflows\windows.yml`:

```yaml
name: Windows Build

on: [push, pull_request]

jobs:
  build:
    runs-on: windows-latest
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Setup CMake
      uses: lukka/get-cmake@latest
    
    - name: Setup MSVC
      uses: ilammy/msvc-dev-cmd@v1
    
    - name: Build
      run: |
        mkdir build
        cd build
        cmake -G "Visual Studio 17 2022" -A x64 ..
        cmake --build . --config Release
    
    - name: Test
      run: quick_test_windows.bat
```

### Jenkins

```groovy
pipeline {
    agent { label 'windows' }
    
    stages {
        stage('Build') {
            steps {
                bat 'build_windows.bat'
            }
        }
        
        stage('Test') {
            steps {
                bat 'run_tests_windows.bat'
            }
        }
    }
}
```

---

## Environment Variables

Useful environment variables for MAOS:

```cmd
REM Set LLVM path
setx LLVM_DIR "C:\Program Files\LLVM"

REM Set MAOS path
setx MAOS_HOME "C:\Users\YourName\MAOS"

REM Add to PATH
setx PATH "%PATH%;%MAOS_HOME%\build\Release"

REM Now you can use from anywhere
llvm-obfuscator.exe --help
```

---

## Directory Structure (Windows)

```
C:\Users\YourName\MAOS\
├── build\
│   ├── Release\
│   │   └── llvm-obfuscator.exe    ← Main executable
│   └── CMakeCache.txt
├── config\
│   ├── size_conservative.yaml
│   └── maximum_security.yaml
├── tests\
│   ├── test_easy.c
│   ├── test_medium.c
│   └── ...
├── test_results_windows\          ← Generated by tests
│   ├── binaries\
│   ├── logs\
│   └── reports\
├── scientific_results_windows\    ← Generated by scientific tests
│   ├── baseline\
│   ├── size_conservative\
│   ├── maximum_security\
│   └── comparisons\
├── build_windows.bat              ← Build script
├── run_tests_windows.bat          ← Test suite
├── run_scientific_tests_windows.bat
├── quick_test_windows.bat         ← Quick verification
├── CMakeLists.txt
└── README.md
```

---

## Best Practices for Windows

### 1. Use Long Path Support

Enable long paths (Windows 10+):
```cmd
REM Run as Administrator
reg add HKLM\SYSTEM\CurrentControlSet\Control\FileSystem /v LongPathsEnabled /t REG_DWORD /d 1
```

### 2. Use Developer Command Prompt

Always use Visual Studio Developer Command Prompt for:
- Building
- Testing
- Development

### 3. Disable Antivirus During Build

Temporarily disable antivirus for faster builds:
- Add MAOS folder to exclusions
- Or disable real-time protection during build

### 4. Use SSD for Build

Build on SSD for much faster compilation:
- Move project to C:\ (SSD)
- Avoid network drives

### 5. Regular Cleanup

```cmd
REM Clean build artifacts
rmdir /s /q build
rmdir /s /q test_results_windows
rmdir /s /q scientific_results_windows

REM Rebuild
build_windows.bat
```

---

## Command Reference

### Essential Commands

```cmd
REM Build
build_windows.bat

REM Quick test
quick_test_windows.bat

REM Full test suite
run_tests_windows.bat

REM Scientific tests
run_scientific_tests_windows.bat

REM Help
build\Release\llvm-obfuscator.exe --help

REM Version
build\Release\llvm-obfuscator.exe --version

REM Simple obfuscation
build\Release\llvm-obfuscator.exe -i input.c -o output.exe

REM Size-conservative
build\Release\llvm-obfuscator.exe -i input.c -o output.exe -c config\size_conservative.yaml

REM Maximum-security
build\Release\llvm-obfuscator.exe -i input.c -o output.exe -c config\maximum_security.yaml
```

---

## Getting Help

### Documentation
- [`START_HERE.md`](../START_HERE.md)
- [`USAGE_GUIDE.md`](../USAGE_GUIDE.md)
- [`MAOS_COMPLETE_IMPLEMENTATION_REPORT.md`](../MAOS_COMPLETE_IMPLEMENTATION_REPORT.md)

### Online Resources
- GitHub Issues: https://github.com/NeuralNinja110/SIH25_2/issues
- Documentation: See `docs/` folder

### Common Issues
- Check: [`TROUBLESHOOTING.md`](../TROUBLESHOOTING.md)
- FAQ: [`FAQ.md`](../FAQ.md)

---

## Quick Start Summary

```cmd
REM 1. Install prerequisites
REM    - Visual Studio 2022
REM    - CMake 3.20+
REM    - LLVM 18.1.3

REM 2. Clone repository
git clone https://github.com/NeuralNinja110/SIH25_2.git
cd SIH25_2

REM 3. Build
build_windows.bat

REM 4. Test
quick_test_windows.bat

REM 5. Use
build\Release\llvm-obfuscator.exe -i myapp.c -o protected.exe
```

---

## Success! 🎉

If you've made it here, MAOS should be running on your Windows system!

**Next steps:**
1. Run `quick_test_windows.bat` to verify
2. Try obfuscating your first file
3. Explore different modes
4. Read the full documentation

**Happy Obfuscating! 🔒**

---

**Document Version:** 1.0  
**Last Updated:** 2025-10-12  
**Platform:** Windows 10/11 (64-bit)  
**Status:** ✅ Complete
