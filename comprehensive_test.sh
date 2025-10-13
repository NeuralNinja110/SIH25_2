#!/bin/bash
echo "============================================"
echo " MAOS v2.0 Comprehensive Verification Test"
echo "============================================"
echo ""

# Test all levels on all files
TEST_FILES=("test_easy" "test_medium" "test_difficult")
LEVELS=(0 1 2)
TOTAL_TESTS=0
PASSED_TESTS=0

for file in "${TEST_FILES[@]}"; do
    for level in "${LEVELS[@]}"; do
        TOTAL_TESTS=$((TOTAL_TESTS + 1))
        echo -n "Testing $file at level $level... "
        
        if ./build/llvm-obfuscator -l $level --cycles $((level*2+2)) -i tests/${file}.c > /dev/null 2>&1; then
            echo "✅ PASS"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo "❌ FAIL"
        fi
    done
done

echo ""
echo "============================================"
echo " Test Results: $PASSED_TESTS/$TOTAL_TESTS passed"
echo "============================================"

if [ $PASSED_TESTS -eq $TOTAL_TESTS ]; then
    echo "🎉 ALL TESTS PASSED! System is operational."
    exit 0
else
    echo "⚠️  Some tests failed. Review errors above."
    exit 1
fi
