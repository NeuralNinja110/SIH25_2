# Quick Build and Usage Guide

## Building the Project

### Prerequisites Check

```bash
# Check if LLVM is installed
llvm-config --version

# Check if Clang is installed
clang --version

# Check if CMake is installed
cmake --version
```

If any of these are missing, install them first (see README.md for installation instructions).

### Build Steps

1. **Clone and navigate to the project**:
```bash
cd /workspaces/SIH25_2
```

2. **Build using the build script** (Recommended):
```bash
./build.sh release
```

3. **Or build manually**:
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### Expected Output

After a successful build, you should see:
- `build/llvm-obfuscator` - Main executable
- `build/libobfuscator_lib.a` - Static library
- `build/obfuscator_tests` - Test executable

## How to Build This Workspace

### Quick Start (One Command)

```bash
./build.sh release
```

This will:
1. Check all dependencies
2. Configure CMake
3. Build the project with optimizations
4. Show build status

### Build Options

```bash
# Clean build
./build.sh clean

# Debug build (with debugging symbols)
./build.sh debug

# Release build (optimized)
./build.sh release

# Build and run tests
./build.sh test
```

### Manual Build (Advanced)

If you prefer manual control:

```bash
# 1. Create build directory
mkdir -p build && cd build

# 2. Configure CMake
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_DIR=/usr/lib/llvm-14/lib/cmake/llvm

# 3. Build
make -j$(nproc)

# 4. Optional: Run tests
ctest --output-on-failure

# 5. Optional: Install system-wide
sudo make install
```

## Testing the Build

### 1. Run Unit Tests

```bash
cd build
./obfuscator_tests
```

### 2. Test with Easy Example

```bash
cd build
./llvm-obfuscator ../tests/test_easy.c -o test_easy_obf --verbose
./test_easy_obf
```

Expected output: Hello World program with obfuscation applied

### 3. Test with Medium Example

```bash
./llvm-obfuscator -l medium ../tests/test_medium.c -o test_medium_obf --verbose
./test_medium_obf
```

Expected output: Data structures and sorting algorithms working correctly

### 4. Test with Difficult Example

```bash
./llvm-obfuscator -l high ../tests/test_difficult.c -o test_difficult_obf --verbose
./test_difficult_obf
```

Expected output: Complex algorithms (trees, graphs) working correctly

## Quick Usage Examples

### Basic Obfuscation

```bash
./build/llvm-obfuscator input.c -o output
```

### High-Security Obfuscation

```bash
./build/llvm-obfuscator -l high --cycles 5 --enable-anti-debug input.c -o output
```

### With Detailed Report

```bash
./build/llvm-obfuscator \
    -l medium \
    --verbose \
    --report my_report \
    --report-format both \
    input.c -o output
```

This generates:
- `output` - Obfuscated binary
- `my_report.json` - JSON report
- `my_report.html` - HTML report

## Troubleshooting

### Problem: CMake can't find LLVM

**Solution**:
```bash
# Find LLVM installation
llvm-config --cmakedir

# Use it in cmake command
cmake .. -DLLVM_DIR=$(llvm-config --cmakedir)
```

### Problem: Build fails with undefined references

**Solution**: Ensure LLVM development packages are installed:
```bash
# Ubuntu/Debian
sudo apt-get install llvm-14-dev

# Fedora
sudo dnf install llvm-devel
```

### Problem: Clang not found during obfuscation

**Solution**: Add Clang to PATH:
```bash
export PATH=/usr/lib/llvm-14/bin:$PATH
```

## Project Structure After Build

```
SIH25_2/
├── build/                      # Build artifacts (created)
│   ├── llvm-obfuscator        # Main executable
│   ├── libobfuscator_lib.a    # Static library
│   ├── obfuscator_tests       # Test executable
│   ├── CMakeFiles/            # CMake cache
│   └── ...
├── include/                    # Header files
├── src/                        # Source files
├── tests/                      # Test cases
├── docs/                       # Documentation
├── CMakeLists.txt             # Build configuration
├── build.sh                    # Build script
└── README.md                   # Main documentation
```

## Next Steps

1. ✅ Build the project: `./build.sh release`
2. ✅ Run tests: `cd build && ./obfuscator_tests`
3. ✅ Try examples: `./llvm-obfuscator ../tests/test_easy.c -o test`
4. ✅ Read documentation: See `README.md` and `docs/ARCHITECTURE.md`
5. ✅ Experiment with options: `./llvm-obfuscator --help`

## Performance Notes

- **First build**: May take 2-5 minutes
- **Incremental builds**: Usually < 30 seconds
- **Test compilation**: Each test case compiles in < 10 seconds

## Support

For issues or questions:
- Check `README.md` for detailed documentation
- Review `docs/ARCHITECTURE.md` for technical details
- See test cases in `tests/` for examples

---

**Quick Command Reference**:

```bash
# Build
./build.sh release

# Test
cd build && ./obfuscator_tests

# Use
./build/llvm-obfuscator input.c -o output

# Help
./build/llvm-obfuscator --help
```
