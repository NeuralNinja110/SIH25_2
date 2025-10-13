# 🪟 MAOS for Windows - Quick Start

## One-Click Setup

### For Beginners

1. **Double-click**: `maos_launcher.bat`
2. **Select**: Option 1 (Build MAOS)
3. **Wait**: 2-5 minutes for build
4. **Select**: Option 2 (Quick Test)
5. **Done!** You're ready to use MAOS

### For Developers

```cmd
REM Open Visual Studio Developer Command Prompt
REM Then run:

build_windows.bat
quick_test_windows.bat
```

---

## Available Scripts

### Batch Files (.bat)

| Script | Purpose | Time | Use When |
|--------|---------|------|----------|
| `maos_launcher.bat` | Interactive menu | - | You want a GUI-like experience |
| `build_windows.bat` | Build MAOS | 2-5 min | First-time setup or after code changes |
| `quick_test_windows.bat` | Quick verification | 30 sec | Verify installation works |
| `run_tests_windows.bat` | Full test suite | 5-10 min | Comprehensive testing |
| `run_scientific_tests_windows.bat` | Scientific tests | 15-20 min | Detailed metrics and analysis |

### PowerShell Scripts (.ps1)

| Script | Purpose | Features |
|--------|---------|----------|
| `build_windows.ps1` | Modern build script | Progress bars, parallel builds, auto-detect |
| `run_tests_windows.ps1` | Test suite with reports | HTML reports, CSV export, detailed metrics |

**To run PowerShell scripts:**
```powershell
# Enable script execution (run once as Administrator)
Set-ExecutionPolicy RemoteSigned

# Run script
.\build_windows.ps1
.\run_tests_windows.ps1
```

---

## Quick Command Reference

### Build
```cmd
build_windows.bat
```

### Test
```cmd
quick_test_windows.bat
```

### Obfuscate a File

**Normal mode:**
```cmd
build\Release\llvm-obfuscator.exe -i myapp.c -o protected.exe
```

**Size-conservative mode:**
```cmd
build\Release\llvm-obfuscator.exe -i myapp.c -o protected.exe -c config\size_conservative.yaml
```

**Maximum-security mode:**
```cmd
build\Release\llvm-obfuscator.exe -i myapp.c -o protected.exe -c config\maximum_security.yaml
```

---

## Prerequisites

### Required (Must Have)
- Windows 10/11 (64-bit)
- Visual Studio 2019/2022 with C++ tools
- CMake 3.20+

### Optional (Nice to Have)
- Ninja (faster builds)
- Python 3.8+ (for analysis scripts)
- Git for Windows

### Installation Links

**Visual Studio Community (Free):**
https://visualstudio.microsoft.com/downloads/

**CMake:**
https://cmake.org/download/

**Ninja:**
```cmd
winget install Ninja-build.Ninja
```

---

## Troubleshooting

### "cmake not found"
```cmd
REM Add to PATH (run as Administrator)
setx /M PATH "%PATH%;C:\Program Files\CMake\bin"
REM Restart terminal
```

### "Visual Studio not found"
**Solution:** Use Visual Studio Developer Command Prompt
- Start Menu → Visual Studio 2022 → Developer Command Prompt

### "llvm-obfuscator.exe not found"
**Solution:** Run build first
```cmd
build_windows.bat
```

### Antivirus Blocking
**Solution:** Add exception for:
- `build\Release\llvm-obfuscator.exe`
- MAOS project folder

---

## Directory Structure

```
MAOS\
├── build\                          ← Created by build script
│   └── Release\
│       └── llvm-obfuscator.exe    ← Main executable
├── config\
│   ├── size_conservative.yaml
│   └── maximum_security.yaml
├── tests\                          ← Test files
│   ├── test_easy.c
│   ├── test_medium.c
│   └── ...
├── test_results_windows\           ← Created by test scripts
│   ├── binaries\
│   ├── logs\
│   └── reports\
├── maos_launcher.bat               ← Interactive menu
├── build_windows.bat               ← Build script
├── quick_test_windows.bat          ← Quick test
├── run_tests_windows.bat           ← Full tests
├── run_scientific_tests_windows.bat
├── build_windows.ps1               ← PowerShell build
├── run_tests_windows.ps1           ← PowerShell tests
├── WINDOWS_GUIDE.md                ← Detailed guide
└── README.md
```

---

## Examples

### Example 1: Protect Mobile App

```cmd
REM Use size-conservative mode for mobile
build\Release\llvm-obfuscator.exe ^
    -i mobile_app.c ^
    -o protected_app.exe ^
    -c config\size_conservative.yaml
```

### Example 2: Protect Crypto Algorithm

```cmd
REM Use maximum-security for crypto
build\Release\llvm-obfuscator.exe ^
    -i crypto_algo.c ^
    -o protected_crypto.exe ^
    -c config\maximum_security.yaml
```

### Example 3: Batch Process Directory

```cmd
@echo off
for %%f in (src\*.c) do (
    echo Processing %%f...
    build\Release\llvm-obfuscator.exe ^
        -i %%f ^
        -o bin\%%~nf.exe ^
        -l 2 -C 2
)
```

---

## Getting Help

### Documentation
- **[WINDOWS_GUIDE.md](WINDOWS_GUIDE.md)** - Complete Windows guide
- **[USAGE_GUIDE.md](USAGE_GUIDE.md)** - Usage instructions
- **[START_HERE.md](START_HERE.md)** - Quick start

### Interactive Help
```cmd
maos_launcher.bat
REM Select option 7 (View Documentation)
```

### Command-Line Help
```cmd
build\Release\llvm-obfuscator.exe --help
```

---

## Next Steps

1. ✅ Build: `build_windows.bat`
2. ✅ Test: `quick_test_windows.bat`
3. ✅ Obfuscate: `build\Release\llvm-obfuscator.exe -i myapp.c -o protected.exe`
4. ✅ Read: `WINDOWS_GUIDE.md` for advanced usage

---

## Support

- **Issues**: https://github.com/NeuralNinja110/SIH25_2/issues
- **Documentation**: See `docs/` folder
- **Examples**: See `tests/` folder

---

**Happy Obfuscating on Windows! 🪟🔒**

---

**Last Updated:** 2025-10-12  
**Platform:** Windows 10/11 (64-bit)  
**Version:** 1.0  
**Status:** ✅ Production Ready
