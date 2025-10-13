# 🪟 Windows Support - Complete Index

## Overview

MAOS now has **complete Windows support** with:
- ✅ Native Windows batch scripts
- ✅ Modern PowerShell scripts
- ✅ Interactive launcher menu
- ✅ Comprehensive testing suite
- ✅ Detailed documentation

---

## Quick Access

### 🚀 For Beginners

**One-Click Experience:**
```cmd
maos_launcher.bat
```
Interactive menu with all features!

**Quick Start:**
```cmd
build_windows.bat          REM Build MAOS (2-5 minutes)
quick_test_windows.bat     REM Verify it works (30 seconds)
```

### 🔧 For Developers

**PowerShell (Recommended):**
```powershell
.\build_windows.ps1        # Modern build with progress
.\run_tests_windows.ps1    # Comprehensive tests with HTML reports
```

**Batch Files:**
```cmd
build_windows.bat                      REM Build
run_tests_windows.bat                  REM Full tests
run_scientific_tests_windows.bat       REM Scientific analysis
```

---

## Complete File Reference

### Build Scripts

| File | Type | Purpose | Features |
|------|------|---------|----------|
| `build_windows.bat` | Batch | Build MAOS | Auto-detect environment, simple |
| `build_windows.ps1` | PowerShell | Modern build | Progress bars, parallel, configurable |

**Choose:**
- Batch: If you want simple, compatible
- PowerShell: If you want modern, advanced

### Test Scripts

| File | Type | Tests | Output |
|------|------|-------|--------|
| `quick_test_windows.bat` | Batch | 3 basic tests | Console only |
| `run_tests_windows.bat` | Batch | 24 tests (3 modes × 8 cases) | TXT reports |
| `run_tests_windows.ps1` | PowerShell | 24 tests | HTML + CSV + TXT |
| `run_scientific_tests_windows.bat` | Batch | Detailed metrics | JSON + TXT |

**Choose:**
- Quick test: Verify installation
- Batch tests: Standard testing
- PowerShell tests: Advanced with HTML reports
- Scientific tests: Research and metrics

### Documentation

| File | Purpose | Audience |
|------|---------|----------|
| `WINDOWS_README.md` | Quick start | Beginners |
| `WINDOWS_GUIDE.md` | Complete guide | All users |
| `WINDOWS_SUPPORT_INDEX.md` | This file | Reference |

### Utilities

| File | Purpose | Use When |
|------|---------|----------|
| `maos_launcher.bat` | Interactive menu | You want GUI-like experience |

---

## Installation & Setup

### Step 1: Prerequisites

**Required Software:**
```
✓ Windows 10/11 (64-bit)
✓ Visual Studio 2019 or 2022
✓ CMake 3.20+
```

**Install Visual Studio:**
1. Download from: https://visualstudio.microsoft.com/downloads/
2. Select: "Desktop development with C++"

**Install CMake:**
1. Download from: https://cmake.org/download/
2. Add to PATH during installation

### Step 2: Build MAOS

**Option A: Interactive Menu**
```cmd
maos_launcher.bat
REM Select: 1 (Build MAOS)
```

**Option B: Direct Build**
```cmd
build_windows.bat
```

**Option C: PowerShell**
```powershell
.\build_windows.ps1
```

### Step 3: Verify

```cmd
quick_test_windows.bat
```

---

## Usage Examples

### Example 1: Simple Obfuscation

```cmd
build\Release\llvm-obfuscator.exe -i myapp.c -o protected.exe
```

### Example 2: Size-Conservative Mode

```cmd
build\Release\llvm-obfuscator.exe ^
    -i myapp.c ^
    -o protected.exe ^
    -c config\size_conservative.yaml
```

**Best for:**
- Mobile applications
- Embedded systems
- Size-constrained environments

### Example 3: Maximum-Security Mode

```cmd
build\Release\llvm-obfuscator.exe ^
    -i myapp.c ^
    -o protected.exe ^
    -c config\maximum_security.yaml
```

**Best for:**
- High-value intellectual property
- Financial applications
- DRM systems

### Example 4: Batch Processing

```cmd
@echo off
for %%f in (src\*.c) do (
    echo Processing %%f...
    build\Release\llvm-obfuscator.exe -i %%f -o bin\%%~nf.exe
)
echo Done!
```

### Example 5: Interactive Mode

```cmd
maos_launcher.bat
REM Select: 5 (Obfuscate Single File)
REM Follow prompts
```

---

## Testing Scenarios

### Scenario 1: Quick Verification

**Purpose:** Verify MAOS works after installation

```cmd
quick_test_windows.bat
```

**Time:** 30 seconds  
**Tests:** 3 basic tests  
**Output:** Console

### Scenario 2: Comprehensive Testing

**Purpose:** Test all modes and test cases

```cmd
run_tests_windows.bat
```

**Time:** 5-10 minutes  
**Tests:** 24 tests (3 modes × 8 cases)  
**Output:**
- Text reports in `test_results_windows\reports\`
- Binary files in `test_results_windows\binaries\`
- Logs in `test_results_windows\logs\`

### Scenario 3: Scientific Analysis

**Purpose:** Detailed metrics and comparative analysis

```cmd
run_scientific_tests_windows.bat
```

**Time:** 15-20 minutes  
**Tests:** 15 tests with detailed metrics  
**Output:**
- JSON metrics for each test
- Comparison reports
- Statistical analysis

### Scenario 4: Advanced Testing (PowerShell)

**Purpose:** Modern testing with HTML reports

```powershell
.\run_tests_windows.ps1
```

**Features:**
- HTML dashboard with graphs
- CSV export for Excel
- Detailed text reports
- Progress indicators

**Advanced Options:**
```powershell
# Skip certain modes
.\run_tests_windows.ps1 -SkipBaseline

# Verbose output
.\run_tests_windows.ps1 -Verbose

# Skip maximum-security (faster)
.\run_tests_windows.ps1 -SkipMaximumSecurity
```

---

## Mode Comparison

### Normal Obfuscation

**Configuration:** `-l 2 -C 2`

**Characteristics:**
- Fast compilation (~500ms)
- Good security (60/100)
- Standard size increase

**Use when:**
- Quick obfuscation needed
- Standard protection sufficient
- Legacy compatibility required

### Size-Conservative Mode

**Configuration:** `-c config\size_conservative.yaml`

**Characteristics:**
- ≤15% size increase
- ≤5% performance overhead
- Good security (72/100)

**Use when:**
- Size constraints critical
- Mobile/embedded applications
- Performance is important

### Maximum-Security Mode

**Configuration:** `-c config\maximum_security.yaml`

**Characteristics:**
- 95/100 security score
- All transformations enabled
- ML-guided optimization

**Use when:**
- Maximum protection needed
- High-value IP
- Security > size/performance

---

## Output Files & Reports

### Build Output

```
build\
├── Release\
│   └── llvm-obfuscator.exe    ← Main executable (14MB)
└── CMakeFiles\                 ← Build logs
```

### Test Results (Batch)

```
test_results_windows\
├── binaries\
│   ├── test_easy_normal.exe
│   ├── test_easy_size_conservative.exe
│   ├── test_easy_maximum_security.exe
│   └── ... (24+ files)
├── logs\
│   ├── test_easy_normal.log
│   └── ... (24+ files)
└── reports\
    └── test_summary_TIMESTAMP.txt
```

### Test Results (PowerShell)

```
test_results_windows\
├── binaries\               ← Obfuscated binaries
├── logs\                   ← Compilation logs
└── reports\
    ├── test_results_TIMESTAMP.html    ← Interactive dashboard
    ├── test_results_TIMESTAMP.csv     ← Excel-compatible
    └── test_summary_TIMESTAMP.txt     ← Text report
```

### Scientific Results

```
scientific_results_windows\
├── baseline\
│   ├── test_easy.exe
│   └── test_easy.json
├── size_conservative\
│   ├── test_easy.exe
│   └── test_easy.json
├── maximum_security\
│   ├── test_easy.exe
│   └── test_easy.json
└── comparisons\
    └── comparison_report_TIMESTAMP.txt
```

---

## Troubleshooting

### Common Issues

#### Issue 1: "cmake not found"

**Symptoms:**
```
'cmake' is not recognized as an internal or external command
```

**Solution:**
```cmd
REM Add CMake to PATH (run as Administrator)
setx /M PATH "%PATH%;C:\Program Files\CMake\bin"

REM Restart terminal and verify
cmake --version
```

#### Issue 2: "Visual Studio not found"

**Symptoms:**
```
'cl' is not recognized as an internal or external command
```

**Solution:**
Open **Visual Studio Developer Command Prompt**:
- Start Menu → Visual Studio 2022 → Developer Command Prompt

Or initialize manually:
```cmd
"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
```

#### Issue 3: "llvm-obfuscator.exe not found"

**Symptoms:**
```
The system cannot find the file specified.
```

**Solution:**
```cmd
REM Build first
build_windows.bat

REM Check if it exists
dir /s /b llvm-obfuscator.exe
```

#### Issue 4: PowerShell execution policy

**Symptoms:**
```
cannot be loaded because running scripts is disabled
```

**Solution:**
```powershell
# Run PowerShell as Administrator
Set-ExecutionPolicy RemoteSigned

# Or for current user only
Set-ExecutionPolicy RemoteSigned -Scope CurrentUser
```

#### Issue 5: Antivirus blocks obfuscator

**Symptoms:**
- Build succeeds but executable disappears
- Access denied errors

**Solution:**
Add exclusions for:
1. `build\Release\llvm-obfuscator.exe`
2. MAOS project folder
3. Temporarily disable real-time protection during build

#### Issue 6: Long path errors

**Symptoms:**
```
The specified path, file name, or both are too long
```

**Solution:**
```cmd
REM Enable long paths (Windows 10 1607+, run as Administrator)
reg add HKLM\SYSTEM\CurrentControlSet\Control\FileSystem /v LongPathsEnabled /t REG_DWORD /d 1
```

---

## Performance Tips

### Faster Builds

**1. Use Ninja:**
```cmd
REM Install Ninja
winget install Ninja-build.Ninja

REM Build with Ninja
cd build
cmake -G Ninja ..
ninja
```
**Result:** 2-3× faster than MSBuild

**2. Use Parallel Builds:**
```cmd
cmake --build build --config Release --parallel
```

**3. Use SSD:**
- Build on C:\ (usually SSD)
- Avoid network drives

### Faster Tests

**Run quick test instead of full suite:**
```cmd
quick_test_windows.bat
```

**Skip certain modes:**
```powershell
.\run_tests_windows.ps1 -SkipMaximumSecurity
```

---

## Integration

### Visual Studio IDE

1. File → Open → Folder
2. Select MAOS directory
3. CMake auto-configures
4. Build → Build All (Ctrl+Shift+B)

### Visual Studio Code

1. Install extensions:
   - C/C++ (Microsoft)
   - CMake Tools
2. Open MAOS folder
3. F1 → CMake: Configure
4. F1 → CMake: Build

### GitHub Actions

```yaml
name: Windows Build

on: [push]

jobs:
  build:
    runs-on: windows-latest
    steps:
    - uses: actions/checkout@v3
    - uses: ilammy/msvc-dev-cmd@v1
    - name: Build
      run: build_windows.bat
    - name: Test
      run: quick_test_windows.bat
```

---

## Advanced Usage

### Custom Configuration

Create `config\my_config.yaml`:
```yaml
mode: custom
level: medium
cycles: 3
passes:
  - instruction_substitution
  - string_encryption
maos:
  enable_atie: true
  enable_qirl: true
  enable_pcge: true
```

Use it:
```cmd
build\Release\llvm-obfuscator.exe -i app.c -o protected.exe -c config\my_config.yaml
```

### Scripted Workflows

```cmd
@echo off
REM Automated build and test workflow

echo Building...
call build_windows.bat
if %ERRORLEVEL% neq 0 (
    echo Build failed!
    exit /b 1
)

echo Testing...
call quick_test_windows.bat
if %ERRORLEVEL% neq 0 (
    echo Tests failed!
    exit /b 1
)

echo Obfuscating production code...
for %%f in (release\*.c) do (
    build\Release\llvm-obfuscator.exe -i %%f -o protected\%%~nf.exe
)

echo Done!
```

---

## Documentation Map

### For Beginners
1. Start: [`WINDOWS_README.md`](WINDOWS_README.md)
2. Quick start: Run `maos_launcher.bat`
3. First obfuscation: Follow prompts

### For Users
1. Complete guide: [`WINDOWS_GUIDE.md`](WINDOWS_GUIDE.md)
2. Usage examples: [`USAGE_GUIDE.md`](USAGE_GUIDE.md)
3. This index: [`WINDOWS_SUPPORT_INDEX.md`](WINDOWS_SUPPORT_INDEX.md)

### For Developers
1. Architecture: [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md)
2. Implementation: [`MAOS_COMPLETE_IMPLEMENTATION_REPORT.md`](MAOS_COMPLETE_IMPLEMENTATION_REPORT.md)
3. ML model: [`ML_MODEL_EXPLANATION.md`](ML_MODEL_EXPLANATION.md)

---

## Command Cheat Sheet

```cmd
REM === BUILD ===
build_windows.bat                    # Build MAOS
.\build_windows.ps1                  # Build (PowerShell)

REM === TEST ===
quick_test_windows.bat               # Quick test
run_tests_windows.bat                # Full tests
.\run_tests_windows.ps1              # Full tests (PowerShell)
run_scientific_tests_windows.bat     # Scientific tests

REM === OBFUSCATE ===
# Normal
build\Release\llvm-obfuscator.exe -i input.c -o output.exe

# Size-conservative
build\Release\llvm-obfuscator.exe -i input.c -o output.exe -c config\size_conservative.yaml

# Maximum-security
build\Release\llvm-obfuscator.exe -i input.c -o output.exe -c config\maximum_security.yaml

REM === INTERACTIVE ===
maos_launcher.bat                    # Interactive menu

REM === HELP ===
build\Release\llvm-obfuscator.exe --help      # Show help
build\Release\llvm-obfuscator.exe --version   # Show version

REM === CLEAN ===
rmdir /s /q build                    # Clean build files
rmdir /s /q test_results_windows     # Clean test results
```

---

## Support & Resources

### Documentation
- [WINDOWS_README.md](WINDOWS_README.md) - Quick start
- [WINDOWS_GUIDE.md](WINDOWS_GUIDE.md) - Complete guide
- [USAGE_GUIDE.md](USAGE_GUIDE.md) - Usage instructions
- [START_HERE.md](START_HERE.md) - Project overview

### Scripts
- Build: `build_windows.bat`, `build_windows.ps1`
- Test: `run_tests_windows.bat`, `run_tests_windows.ps1`
- Quick test: `quick_test_windows.bat`
- Scientific: `run_scientific_tests_windows.bat`
- Launcher: `maos_launcher.bat`

### Online
- GitHub: https://github.com/NeuralNinja110/SIH25_2
- Issues: https://github.com/NeuralNinja110/SIH25_2/issues

---

## Summary

✅ **Windows support is complete and production-ready!**

**Key Features:**
- Native Windows batch and PowerShell scripts
- Interactive launcher menu
- Comprehensive testing (3 test suites)
- HTML reports and dashboards
- Full documentation
- Troubleshooting guides
- Integration examples

**Choose your workflow:**
- **Beginner**: `maos_launcher.bat` (interactive)
- **Standard**: `build_windows.bat` + `run_tests_windows.bat`
- **Advanced**: PowerShell scripts with HTML reports
- **Professional**: IDE integration + CI/CD

---

**Happy Obfuscating on Windows! 🪟🔒**

---

**Document Version:** 1.0  
**Last Updated:** 2025-10-12  
**Platform:** Windows 10/11 (64-bit)  
**Status:** ✅ Complete & Production Ready
