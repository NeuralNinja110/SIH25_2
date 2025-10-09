#!/bin/bash

# Build script for LLVM Code Obfuscator
# Usage: ./build.sh [clean|release|debug|test]

set -e  # Exit on error

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$PROJECT_DIR/build"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}LLVM Code Obfuscator Build Script${NC}"
echo "===================================="

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Check dependencies
check_dependencies() {
    echo -e "\n${YELLOW}Checking dependencies...${NC}"
    
    if ! command_exists cmake; then
        echo -e "${RED}Error: cmake not found${NC}"
        exit 1
    fi
    
    if ! command_exists clang; then
        echo -e "${RED}Error: clang not found${NC}"
        exit 1
    fi
    
    if ! command_exists llvm-config; then
        echo -e "${RED}Error: llvm-config not found${NC}"
        echo "Please install LLVM development packages"
        exit 1
    fi
    
    echo -e "${GREEN}✓ All dependencies found${NC}"
    echo "LLVM Version: $(llvm-config --version)"
}

# Clean build directory
clean_build() {
    echo -e "\n${YELLOW}Cleaning build directory...${NC}"
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
        echo -e "${GREEN}✓ Build directory cleaned${NC}"
    fi
}

# Configure CMake
configure_cmake() {
    local build_type=$1
    
    echo -e "\n${YELLOW}Configuring CMake (${build_type} build)...${NC}"
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    
    cmake .. \
        -DCMAKE_BUILD_TYPE="$build_type" \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    
    echo -e "${GREEN}✓ CMake configuration complete${NC}"
}

# Build project
build_project() {
    echo -e "\n${YELLOW}Building project...${NC}"
    cd "$BUILD_DIR"
    
    # Get number of CPU cores
    if [ "$(uname)" == "Darwin" ]; then
        CORES=$(sysctl -n hw.ncpu)
    else
        CORES=$(nproc)
    fi
    
    make -j"$CORES"
    
    echo -e "${GREEN}✓ Build complete${NC}"
}

# Run tests
run_tests() {
    echo -e "\n${YELLOW}Running tests...${NC}"
    cd "$BUILD_DIR"
    
    if [ -f "./obfuscator_tests" ]; then
        ./obfuscator_tests
    else
        echo -e "${YELLOW}Warning: Test executable not found${NC}"
    fi
    
    echo -e "${GREEN}✓ Tests complete${NC}"
}

# Main build function
main_build() {
    local build_type=${1:-Release}
    
    check_dependencies
    configure_cmake "$build_type"
    build_project
    
    echo -e "\n${GREEN}========================================${NC}"
    echo -e "${GREEN}Build completed successfully!${NC}"
    echo -e "${GREEN}========================================${NC}"
    echo -e "\nExecutable location: ${BUILD_DIR}/llvm-obfuscator"
    echo -e "Run with: ${BUILD_DIR}/llvm-obfuscator --help\n"
}

# Parse command line arguments
case "${1:-build}" in
    clean)
        clean_build
        ;;
    release)
        main_build "Release"
        ;;
    debug)
        main_build "Debug"
        ;;
    test)
        if [ ! -d "$BUILD_DIR" ]; then
            main_build "Release"
        fi
        run_tests
        ;;
    *)
        main_build "Release"
        ;;
esac
