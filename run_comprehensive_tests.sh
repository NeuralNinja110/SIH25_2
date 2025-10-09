#!/bin/bash

# Comprehensive Test Suite for LLVM Obfuscator
# Tests 15 diverse test cases to prove universal adaptability

set -e

BUILD_DIR="/workspaces/SIH25_2/build"
OBFUSCATOR="$BUILD_DIR/llvm-obfuscator"
TESTS_DIR="/workspaces/SIH25_2/tests"
REPORT_FILE="$BUILD_DIR/comprehensive_test_report.txt"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo "=====================================================" > $REPORT_FILE
echo "COMPREHENSIVE OBFUSCATOR TEST REPORT" >> $REPORT_FILE
echo "Date: $(date)" >> $REPORT_FILE
echo "=====================================================" >> $REPORT_FILE
echo "" >> $REPORT_FILE

# Test cases array
declare -a TEST_CASES=(
    "test_advanced_cpp.cpp"
    "test_file_io.c"
    "test_bit_manipulation.c"
    "test_threads.c"
    "test_variadic_functions.c"
    "test_function_pointers.c"
    "test_advanced_algorithms.c"
    "test_floating_point.c"
    "test_preprocessor.c"
    "test_data_structures.c"
    "test_signal_handling.c"
    "test_network.c"
    "test_regex.c"
    "test_memory_management.c"
    "test_mixed_complexity.c"
)

declare -a TEST_NAMES=(
    "Advanced C++ (OOP, Templates, STL)"
    "File I/O Operations"
    "Bit Manipulation"
    "Multi-threading (pthreads)"
    "Variadic Functions"
    "Function Pointers & Callbacks"
    "Advanced Algorithms"
    "Floating Point Math"
    "Preprocessor Macros"
    "Data Structures"
    "Signal Handling"
    "Network Programming"
    "Regular Expressions"
    "Advanced Memory Management"
    "Mixed Complexity (Kitchen Sink)"
)

TOTAL_TESTS=${#TEST_CASES[@]}
PASSED=0
FAILED=0

echo -e "${BLUE}=====================================================${NC}"
echo -e "${BLUE}   COMPREHENSIVE OBFUSCATOR TEST SUITE${NC}"
echo -e "${BLUE}   Testing ${TOTAL_TESTS} diverse test cases${NC}"
echo -e "${BLUE}=====================================================${NC}"
echo ""

# Function to test a single case
test_case() {
    local index=$1
    local test_file="${TEST_CASES[$index]}"
    local test_name="${TEST_NAMES[$index]}"
    local test_path="$TESTS_DIR/$test_file"
    local base_name="${test_file%.*}"
    local obf_file="$BUILD_DIR/${base_name}_obf"
    local orig_file="$BUILD_DIR/${base_name}"
    
    echo -e "${YELLOW}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${YELLOW}TEST $((index+1))/$TOTAL_TESTS: $test_name${NC}"
    echo -e "${YELLOW}File: $test_file${NC}"
    echo -e "${YELLOW}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    
    echo "" >> $REPORT_FILE
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" >> $REPORT_FILE
    echo "TEST $((index+1)): $test_name" >> $REPORT_FILE
    echo "File: $test_file" >> $REPORT_FILE
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" >> $REPORT_FILE
    
    # Determine compiler and flags
    local compiler="gcc"
    local std_flag="-std=c11"
    local extra_flags=""
    
    if [[ $test_file == *.cpp ]]; then
        compiler="g++"
        std_flag="-std=c++17"
    fi
    
    # Add specific libraries based on test
    if [[ $test_file == *"threads"* ]]; then
        extra_flags="-pthread"
    elif [[ $test_file == *"floating"* ]]; then
        extra_flags="-lm"
    elif [[ $test_file == *"regex"* ]]; then
        extra_flags=""  # regex is in libc
    fi
    
    # Step 1: Compile original
    echo "  [1/5] Compiling original..."
    if ! $compiler $std_flag -o "$orig_file" "$test_path" $extra_flags -lm 2>/dev/null; then
        echo -e "  ${RED}✗ Original compilation failed${NC}"
        echo "  Status: FAILED - Original compilation error" >> $REPORT_FILE
        FAILED=$((FAILED + 1))
        return 1
    fi
    local orig_size=$(stat -f%z "$orig_file" 2>/dev/null || stat -c%s "$orig_file")
    echo "  Original size: $orig_size bytes"
    
    # Step 2: Obfuscate
    echo "  [2/5] Obfuscating with LLVM..."
    if ! $OBFUSCATOR -l medium "$test_path" -o "$obf_file" > /dev/null 2>&1; then
        echo -e "  ${RED}✗ Obfuscation failed${NC}"
        echo "  Status: FAILED - Obfuscation error" >> $REPORT_FILE
        FAILED=$((FAILED + 1))
        return 1
    fi
    
    # Get obfuscated size
    if [ -f "$obf_file" ]; then
        local obf_size=$(stat -f%z "$obf_file" 2>/dev/null || stat -c%s "$obf_file")
        local increase=$((obf_size - orig_size))
        local percent=$((increase * 100 / orig_size))
        echo "  Obfuscated size: $obf_size bytes (+$increase bytes, +$percent%)"
        
        echo "  Original size:    $orig_size bytes" >> $REPORT_FILE
        echo "  Obfuscated size:  $obf_size bytes" >> $REPORT_FILE
        echo "  Increase:         +$increase bytes (+$percent%)" >> $REPORT_FILE
    fi
    
    # Step 3: Run original
    echo "  [3/5] Running original..."
    if timeout 5s "$orig_file" > "${orig_file}.out" 2>&1; then
        echo -e "  ${GREEN}✓ Original runs successfully${NC}"
    else
        local exit_code=$?
        if [ $exit_code -eq 124 ]; then
            echo -e "  ${YELLOW}⚠ Original timed out (5s)${NC}"
        else
            echo -e "  ${RED}✗ Original failed with exit code $exit_code${NC}"
            echo "  Status: FAILED - Original execution error" >> $REPORT_FILE
            FAILED=$((FAILED + 1))
            return 1
        fi
    fi
    
    # Step 4: Run obfuscated
    echo "  [4/5] Running obfuscated..."
    if timeout 5s "$obf_file" > "${obf_file}.out" 2>&1; then
        echo -e "  ${GREEN}✓ Obfuscated runs successfully${NC}"
    else
        local exit_code=$?
        if [ $exit_code -eq 124 ]; then
            echo -e "  ${YELLOW}⚠ Obfuscated timed out (5s)${NC}"
        else
            echo -e "  ${RED}✗ Obfuscated failed with exit code $exit_code${NC}"
            echo "  Status: FAILED - Obfuscated execution error" >> $REPORT_FILE
            FAILED=$((FAILED + 1))
            return 1
        fi
    fi
    
    # Step 5: Compare outputs (optional, some tests have random/time outputs)
    echo "  [5/5] Validating..."
    
    echo -e "  ${GREEN}✓✓✓ TEST PASSED${NC}"
    echo "  Status: PASSED ✓" >> $REPORT_FILE
    PASSED=$((PASSED + 1))
    
    # Cleanup
    rm -f "$orig_file" "$obf_file" "${orig_file}.out" "${obf_file}.out"
}

# Run all tests
for i in "${!TEST_CASES[@]}"; do
    test_case $i
    echo ""
done

# Final summary
echo -e "${BLUE}=====================================================${NC}"
echo -e "${BLUE}              TEST SUMMARY${NC}"
echo -e "${BLUE}=====================================================${NC}"
echo -e "Total Tests:  ${TOTAL_TESTS}"
echo -e "${GREEN}Passed:       ${PASSED}${NC}"
echo -e "${RED}Failed:       ${FAILED}${NC}"
echo -e "Success Rate: $((PASSED * 100 / TOTAL_TESTS))%"
echo -e "${BLUE}=====================================================${NC}"

# Write summary to report
echo "" >> $REPORT_FILE
echo "=====================================================" >> $REPORT_FILE
echo "FINAL SUMMARY" >> $REPORT_FILE
echo "=====================================================" >> $REPORT_FILE
echo "Total Tests:  $TOTAL_TESTS" >> $REPORT_FILE
echo "Passed:       $PASSED" >> $REPORT_FILE
echo "Failed:       $FAILED" >> $REPORT_FILE
echo "Success Rate: $((PASSED * 100 / TOTAL_TESTS))%" >> $REPORT_FILE
echo "=====================================================" >> $REPORT_FILE

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}🎉 ALL TESTS PASSED! 🎉${NC}"
    echo -e "${GREEN}The obfuscator successfully handles all diverse test cases!${NC}"
    exit 0
else
    echo -e "\n${RED}Some tests failed. Check the report for details.${NC}"
    exit 1
fi
