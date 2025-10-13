#!/bin/bash

###############################################################################
# MAOS v2.0 Auto-Tuning Demo
# Quick demonstration of auto-tuning capabilities
###############################################################################

set -e

BLUE='\033[0;34m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${BLUE}╔══════════════════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║           MAOS v2.0 Auto-Tuning Demonstration                        ║${NC}"
echo -e "${BLUE}╚══════════════════════════════════════════════════════════════════════╝${NC}"
echo ""

# Check if build exists
if [ ! -f "./build/llvm-obfuscator" ]; then
    echo -e "${YELLOW}Building MAOS...${NC}"
    mkdir -p build && cd build
    cmake .. && make llvm-obfuscator
    cd ..
    echo -e "${GREEN}✓ Build completed${NC}"
    echo ""
fi

# Create demo directory
DEMO_DIR="demo_auto_tune"
mkdir -p "$DEMO_DIR"

echo -e "${GREEN}This demo will show auto-tuning with different goals:${NC}"
echo ""
echo "1. Quick 3-iteration security optimization"
echo "2. Balanced optimization (default)"
echo "3. Size-focused optimization"
echo ""

# Demo 1: Security-focused (quick)
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}Demo 1: Security-Focused Optimization (3 iterations)${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""

./run_auto_tune.sh \
    --iterations 3 \
    --goal security \
    --output "$DEMO_DIR/security_results" \
    tests/test_easy.c

echo ""
echo -e "${GREEN}✓ Security-focused optimization complete!${NC}"
echo -e "  Results: $DEMO_DIR/security_results/optimization_report.md"
echo ""

# Show key metrics
if [ -f "$DEMO_DIR/security_results/optimization_report.md" ]; then
    echo -e "${YELLOW}Key Metrics:${NC}"
    grep "RE Difficulty Score:" "$DEMO_DIR/security_results/optimization_report.md" | head -1
    grep "Security Score:" "$DEMO_DIR/security_results/optimization_report.md" | head -1
    echo ""
fi

read -p "Press Enter to continue to Demo 2..."
echo ""

# Demo 2: Balanced
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}Demo 2: Balanced Optimization (3 iterations)${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""

./run_auto_tune.sh \
    --iterations 3 \
    --goal balanced \
    --output "$DEMO_DIR/balanced_results" \
    tests/test_easy.c

echo ""
echo -e "${GREEN}✓ Balanced optimization complete!${NC}"
echo -e "  Results: $DEMO_DIR/balanced_results/optimization_report.md"
echo ""

if [ -f "$DEMO_DIR/balanced_results/optimization_report.md" ]; then
    echo -e "${YELLOW}Key Metrics:${NC}"
    grep "RE Difficulty Score:" "$DEMO_DIR/balanced_results/optimization_report.md" | head -1
    grep "Compilation Time:" "$DEMO_DIR/balanced_results/optimization_report.md" | head -1
    echo ""
fi

read -p "Press Enter to continue to Demo 3..."
echo ""

# Demo 3: Size-focused
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}Demo 3: Size-Focused Optimization (3 iterations)${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""

./run_auto_tune.sh \
    --iterations 3 \
    --goal size \
    --output "$DEMO_DIR/size_results" \
    tests/test_easy.c

echo ""
echo -e "${GREEN}✓ Size-focused optimization complete!${NC}"
echo -e "  Results: $DEMO_DIR/size_results/optimization_report.md"
echo ""

if [ -f "$DEMO_DIR/size_results/optimization_report.md" ]; then
    echo -e "${YELLOW}Key Metrics:${NC}"
    grep "RE Difficulty Score:" "$DEMO_DIR/size_results/optimization_report.md" | head -1
    grep "Binary Size:" "$DEMO_DIR/size_results/optimization_report.md" | head -1
    echo ""
fi

# Comparison
echo ""
echo -e "${GREEN}╔══════════════════════════════════════════════════════════════════════╗${NC}"
echo -e "${GREEN}║                        Demo Complete!                                ║${NC}"
echo -e "${GREEN}╚══════════════════════════════════════════════════════════════════════╝${NC}"
echo ""
echo -e "${YELLOW}Comparison of Optimization Goals:${NC}"
echo ""
echo "📁 Results saved to: $DEMO_DIR/"
echo ""
echo "┌────────────┬─────────────────────────────────────────┐"
echo "│ Goal       │ Output Directory                        │"
echo "├────────────┼─────────────────────────────────────────┤"
echo "│ Security   │ $DEMO_DIR/security_results/  │"
echo "│ Balanced   │ $DEMO_DIR/balanced_results/  │"
echo "│ Size       │ $DEMO_DIR/size_results/      │"
echo "└────────────┴─────────────────────────────────────────┘"
echo ""
echo -e "${YELLOW}Next Steps:${NC}"
echo "1. Compare reports in each results directory"
echo "2. Use optimized configs for your production code:"
echo "   ./build/llvm-obfuscator -c $DEMO_DIR/security_results/final_optimized_config.yaml -i your_file.c"
echo ""
echo "3. For real production use, increase iterations to 8-15:"
echo "   ./run_auto_tune.sh --iterations 10 --goal security your_production_code.c"
echo ""
