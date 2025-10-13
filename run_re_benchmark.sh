#!/bin/bash
# Comprehensive Reverse Engineering Benchmark Runner
# Automates the complete benchmarking process

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR"
RE_DIR="$PROJECT_ROOT/reverse_engineering"

echo "========================================================================"
echo "LLVM OBFUSCATOR REVERSE ENGINEERING BENCHMARK"
echo "========================================================================"
echo ""
echo "Project Root: $PROJECT_ROOT"
echo "RE Directory: $RE_DIR"
echo ""

# Check if obfuscator is built
if [ ! -f "$PROJECT_ROOT/build/llvm-obfuscator" ]; then
    echo "✗ ERROR: Obfuscator not built!"
    echo ""
    echo "Please build the project first:"
    echo "  cd $PROJECT_ROOT"
    echo "  ./build.sh"
    echo ""
    exit 1
fi

echo "✓ Obfuscator found: $PROJECT_ROOT/build/llvm-obfuscator"
echo ""

# Check Python dependencies
echo "Checking Python dependencies..."
python3 -c "import sys; print(f'Python version: {sys.version}')"

# Make scripts executable
chmod +x "$RE_DIR/scripts/re_framework.py"
chmod +x "$RE_DIR/scripts/run_benchmark.py"

echo ""
echo "========================================================================"
echo "STARTING BENCHMARK"
echo "========================================================================"
echo ""
echo "This will:"
echo "  1. Compile test cases in all modes (baseline, normal, size-conservative, maximum-security)"
echo "  2. Perform reverse engineering analysis on each binary"
echo "  3. Generate comprehensive reports with statistical analysis"
echo ""
echo "Estimated time: 10-20 minutes depending on system performance"
echo ""
read -p "Press Enter to continue or Ctrl+C to cancel..."
echo ""

# Run the benchmark
cd "$PROJECT_ROOT"
python3 "$RE_DIR/scripts/run_benchmark.py" \
    --project-root "$PROJECT_ROOT" \
    --output "$RE_DIR/results"

# Check if benchmark succeeded
if [ $? -eq 0 ]; then
    echo ""
    echo "========================================================================"
    echo "BENCHMARK COMPLETE!"
    echo "========================================================================"
    echo ""
    echo "Results location: $RE_DIR/results/"
    echo ""
    echo "Generated files:"
    echo "  📊 $RE_DIR/results/reports/comprehensive_report.md"
    echo "  📈 $RE_DIR/results/reports/comparison_table.csv"
    echo "  📉 $RE_DIR/results/reports/statistical_analysis.json"
    echo "  📁 $RE_DIR/results/binaries/ (compiled binaries)"
    echo "  📁 $RE_DIR/results/metrics/ (detailed metrics)"
    echo ""
    echo "To view the main report:"
    echo "  cat $RE_DIR/results/reports/comprehensive_report.md"
    echo ""
    echo "To open in VS Code:"
    echo "  code $RE_DIR/results/reports/comprehensive_report.md"
    echo ""
else
    echo ""
    echo "✗ Benchmark failed! Check error messages above."
    exit 1
fi
