#!/bin/bash

###############################################################################
# MAOS v2.0 Auto-Tuning Script
# Automated parameter optimization for obfuscation
###############################################################################

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_header() {
    echo -e "${BLUE}╔══════════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${BLUE}║         MAOS v2.0 - Auto-Tuning Optimization System              ║${NC}"
    echo -e "${BLUE}╚══════════════════════════════════════════════════════════════════════╝${NC}"
    echo ""
}

print_usage() {
    echo "Usage: $0 [options] <input-file>"
    echo ""
    echo "Options:"
    echo "  -i, --iterations <n>    Number of optimization iterations (default: 5)"
    echo "  -g, --goal <goal>       Optimization goal: security, balanced, size (default: balanced)"
    echo "  -o, --output <dir>      Output directory (default: auto_tune_results)"
    echo "  -c, --config <file>     Base configuration YAML (default: config/maximum_security.yaml)"
    echo "  -h, --help              Show this help message"
    echo ""
    echo "Examples:"
    echo "  # Basic auto-tuning with 5 iterations"
    echo "  $0 tests/test_medium.c"
    echo ""
    echo "  # Security-focused with 10 iterations"
    echo "  $0 --iterations 10 --goal security tests/test_difficult.c"
    echo ""
    echo "  # Balanced optimization with custom output"
    echo "  $0 -i 8 -g balanced -o my_results tests/test_easy.c"
    echo ""
}

# Default values
ITERATIONS=5
GOAL="balanced"
OUTPUT_DIR="auto_tune_results"
BASE_CONFIG="config/maximum_security.yaml"
INPUT_FILE=""

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -i|--iterations)
            ITERATIONS="$2"
            shift 2
            ;;
        -g|--goal)
            GOAL="$2"
            shift 2
            ;;
        -o|--output)
            OUTPUT_DIR="$2"
            shift 2
            ;;
        -c|--config)
            BASE_CONFIG="$2"
            shift 2
            ;;
        -h|--help)
            print_usage
            exit 0
            ;;
        *)
            INPUT_FILE="$1"
            shift
            ;;
    esac
done

# Validate input
if [ -z "$INPUT_FILE" ]; then
    echo -e "${RED}Error: No input file specified${NC}"
    print_usage
    exit 1
fi

if [ ! -f "$INPUT_FILE" ]; then
    echo -e "${RED}Error: Input file not found: $INPUT_FILE${NC}"
    exit 1
fi

if [ ! -f "$BASE_CONFIG" ]; then
    echo -e "${RED}Error: Base config not found: $BASE_CONFIG${NC}"
    exit 1
fi

# Validate goal
if [[ ! "$GOAL" =~ ^(security|balanced|size)$ ]]; then
    echo -e "${RED}Error: Invalid goal '$GOAL'. Must be: security, balanced, or size${NC}"
    exit 1
fi

# Validate iterations
if ! [[ "$ITERATIONS" =~ ^[0-9]+$ ]] || [ "$ITERATIONS" -lt 1 ] || [ "$ITERATIONS" -gt 50 ]; then
    echo -e "${RED}Error: Iterations must be between 1 and 50${NC}"
    exit 1
fi

print_header

echo -e "${GREEN}Configuration:${NC}"
echo "  Input File:     $INPUT_FILE"
echo "  Iterations:     $ITERATIONS"
echo "  Goal:           $GOAL"
echo "  Output Dir:     $OUTPUT_DIR"
echo "  Base Config:    $BASE_CONFIG"
echo ""

# Check if build exists
if [ ! -f "./build/llvm-obfuscator" ]; then
    echo -e "${YELLOW}Build not found. Building MAOS...${NC}"
    make -C build llvm-obfuscator
    if [ $? -ne 0 ]; then
        echo -e "${RED}Build failed!${NC}"
        exit 1
    fi
    echo -e "${GREEN}Build completed successfully${NC}"
    echo ""
fi

# Create output directory
mkdir -p "$OUTPUT_DIR"

echo -e "${BLUE}Starting auto-tuning optimization...${NC}"
echo ""

# Run auto-tuner
./build/llvm-obfuscator \
    --auto-tune \
    --auto-tune-iterations "$ITERATIONS" \
    --auto-tune-goal "$GOAL" \
    -i "$INPUT_FILE" \
    -o "$OUTPUT_DIR/optimized_output" \
    --verbose

EXIT_CODE=$?

echo ""
if [ $EXIT_CODE -eq 0 ]; then
    echo -e "${GREEN}╔══════════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║              AUTO-TUNING COMPLETED SUCCESSFULLY!                     ║${NC}"
    echo -e "${GREEN}╚══════════════════════════════════════════════════════════════════════╝${NC}"
    echo ""
    echo -e "${GREEN}Results:${NC}"
    echo "  📁 Output Directory:      $OUTPUT_DIR"
    echo "  🏆 Best Config:           $OUTPUT_DIR/final_optimized_config.yaml"
    echo "  📊 Optimization Report:   $OUTPUT_DIR/optimization_report.md"
    echo "  💾 Optimized Binary:      $OUTPUT_DIR/final_optimized_binary"
    echo ""
    
    # Show quick summary if report exists
    if [ -f "$OUTPUT_DIR/optimization_report.md" ]; then
        echo -e "${BLUE}Quick Summary:${NC}"
        grep -A 5 "## Best Configuration Found" "$OUTPUT_DIR/optimization_report.md" | tail -n 5
        echo ""
    fi
    
    echo -e "${YELLOW}💡 Tip: Use the optimized config for production:${NC}"
    echo "   ./build/llvm-obfuscator -c $OUTPUT_DIR/final_optimized_config.yaml -i <input> -o <output>"
    echo ""
else
    echo -e "${RED}╔══════════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${RED}║                   AUTO-TUNING FAILED!                                ║${NC}"
    echo -e "${RED}╚══════════════════════════════════════════════════════════════════════╝${NC}"
    echo ""
    echo -e "${RED}Check the logs above for error details${NC}"
    exit $EXIT_CODE
fi
