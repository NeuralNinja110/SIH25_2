# MAOS v2.0 Auto-Tuning System Documentation

## Overview

The MAOS v2.0 Auto-Tuning System is an intelligent parameter optimization framework that automatically discovers the best obfuscation configuration for your specific needs. It uses iterative evaluation and mutation-based optimization to maximize security metrics while considering performance and size trade-offs.

## Features

✅ **Automated Parameter Optimization** - No manual tuning required  
✅ **Multi-Objective Optimization** - Balance security, performance, and size  
✅ **RE Benchmark Integration** - Real-world security validation  
✅ **Iterative Improvement** - Learns from previous iterations  
✅ **Configuration Persistence** - Save and reuse optimized configs  

## How It Works

### 1. Baseline Evaluation (Iteration 0)
- Loads base configuration (default: `maximum_security.yaml`)
- Runs obfuscation on input file
- Executes RE benchmark to measure security
- Establishes baseline metrics

### 2. Iterative Optimization (Iterations 1-N)
For each iteration:
1. **Generate Candidates** - Create 3-5 configuration variations using mutation
2. **Evaluate Each** - Run obfuscation + RE benchmark on each candidate
3. **Calculate Fitness** - Weighted score based on optimization goal
4. **Track Best** - Keep the best configuration found so far
5. **Adapt** - Adjust mutation strategy based on progress

### 3. Final Run
- Uses the best configuration found
- Generates optimized binary
- Creates comprehensive optimization report

## Evaluation Metrics

### Primary Scores
- **RE Difficulty Score** (0-100) - How hard to reverse engineer
- **Security Score** (0-100) - Overall security assessment
- **Resilience Score** (0-100) - Resistance to various attacks

### Component Scores
- Complexity, Symbol Stripping, String Obfuscation
- Disassembly Resistance, Control Flow, Data Flow
- Anti-Analysis Features, Decompilation Resistance

### Performance Metrics
- Compilation time
- Binary size overhead
- Functionality preservation

### Fitness Calculation

The fitness score combines metrics based on optimization goal:

| Goal      | Security | Performance | Size |
|-----------|----------|-------------|------|
| security  | 80%      | 15%         | 5%   |
| balanced  | 60%      | 25%         | 15%  |
| size      | 50%      | 20%         | 30%  |

## Usage

### Command Line

#### Basic Auto-Tuning
```bash
./build/llvm-obfuscator --auto-tune -i input.c
```

#### With Custom Iterations
```bash
./build/llvm-obfuscator --auto-tune --auto-tune-iterations 10 -i input.c
```

#### With Optimization Goal
```bash
./build/llvm-obfuscator \
    --auto-tune \
    --auto-tune-iterations 8 \
    --auto-tune-goal security \
    -i input.cpp
```

#### Using Helper Script
```bash
# Basic
./run_auto_tune.sh tests/test_medium.c

# Security-focused with 10 iterations
./run_auto_tune.sh --iterations 10 --goal security tests/test_difficult.c

# Balanced optimization with custom output
./run_auto_tune.sh -i 8 -g balanced -o my_results tests/test_easy.c
```

### Configuration Options

#### `--auto-tune`
Enable automatic parameter optimization.

#### `--auto-tune-iterations <n>`
Number of optimization iterations (1-50, default: 5).
- **1-3**: Quick optimization, good for testing
- **5-8**: Balanced exploration (recommended)
- **10-20**: Thorough optimization, better results
- **20+**: Extensive search, diminishing returns

#### `--auto-tune-goal <goal>`
Optimization objective:

**security** - Maximize RE difficulty (80% weight)
- Best for: Production binaries, IP protection
- Trades: Higher overhead acceptable
- Target: 80-90+ RE difficulty score

**balanced** - Balance security/performance (60/25/15%)
- Best for: General use, reasonable overhead
- Trades: Moderate security vs performance
- Target: 70-85 RE difficulty score

**size** - Minimize size with good security (50/20/30%)
- Best for: Embedded systems, size-constrained
- Trades: Lower overhead priority
- Target: 60-75 RE difficulty score

## Output Files

### Directory Structure
```
auto_tune_results/
├── config_iter0.yaml              # Baseline config
├── binary_iter0                   # Baseline binary
├── config_iter1_cand0.yaml        # Iteration 1, candidate 0
├── binary_iter1_cand0             # Iteration 1, candidate 0 binary
├── ...                            # More iterations
├── final_optimized_config.yaml    # ✅ Best config found
├── final_optimized_binary         # ✅ Final optimized binary
├── optimization_report.md         # ✅ Comprehensive report
└── re_results_*/                  # RE benchmark results
```

### Key Output Files

#### `final_optimized_config.yaml`
The best configuration discovered. **Use this for production!**

```bash
# Apply optimized config to other files
./build/llvm-obfuscator \
    -c auto_tune_results/final_optimized_config.yaml \
    -i my_production_code.c \
    -o my_production_binary
```

#### `optimization_report.md`
Comprehensive report including:
- Best configuration summary
- Optimization history table
- Component score breakdown
- Recommendations

#### `final_optimized_binary`
The binary produced with the best configuration.

## Optimization Strategies

### Mutation-Based Exploration

The auto-tuner uses controlled mutations to explore parameter space:

**Numerical Parameters** (with mutation rate):
- `obfuscationCycles` (1-10)
- `flatteningComplexity` (10-100)
- `opaquePredicateCount` (5-50)
- `constantObfuscationComplexity` (10-100)
- `cacheObfuscationIntensity` (0-100)

**Boolean Flags** (lower mutation rate):
- `enableBogusControlFlow`
- `enableDeadCodeInjection`
- `enableFunctionVirtualization`

### Adaptive Behavior

The system adapts its strategy:
- **Early Iterations**: Broader exploration, higher mutation
- **Later Iterations**: Refinement, focused improvements
- **Stagnation Detected**: Increased mutation to escape local optima

## Examples

### Example 1: Quick Security Optimization

```bash
./run_auto_tune.sh --iterations 3 --goal security tests/test_easy.c
```

**Expected Output:**
```
=== ITERATION 0: Baseline Evaluation ===
Baseline RE Difficulty: 42.3/100

=== ITERATION 1 ===
Evaluating candidate 1/3
  Fitness: 58.2
  RE Difficulty: 67.5/100
*** NEW BEST CANDIDATE FOUND! ***

=== ITERATION 2 ===
Evaluating candidate 1/3
  Fitness: 61.4
  RE Difficulty: 73.1/100
*** NEW BEST CANDIDATE FOUND! ***

=== ITERATION 3 ===
Evaluating candidate 2/4
  Fitness: 64.8
  RE Difficulty: 78.9/100
*** NEW BEST CANDIDATE FOUND! ***

=== FINAL RUN WITH BEST CONFIGURATION ===
Best RE Difficulty: 78.9/100
Best Security Score: 74.2/100
```

### Example 2: Balanced Optimization for Production

```bash
./build/llvm-obfuscator \
    --auto-tune \
    --auto-tune-iterations 8 \
    --auto-tune-goal balanced \
    -i production_app.c \
    --verbose
```

**Results:**
- 8 iterations with 3-5 candidates each
- ~30-40 total evaluations
- Optimized for security/performance balance
- Expected improvement: 50-100% over baseline

### Example 3: Size-Constrained Embedded System

```bash
./run_auto_tune.sh \
    --iterations 10 \
    --goal size \
    --output embedded_results \
    embedded_firmware.c
```

**Results:**
- Minimizes binary size overhead
- Maintains good security (60-75 range)
- Suitable for resource-constrained devices

## Performance Considerations

### Iteration Time
Each iteration includes:
1. Obfuscation: 1-30 seconds (depends on input size)
2. RE Benchmark: 5-15 seconds per binary
3. Metric calculation: <1 second

**Total Time Estimates:**
- 3 iterations: ~5-10 minutes
- 5 iterations: ~10-20 minutes
- 10 iterations: ~20-40 minutes
- 20 iterations: ~40-80 minutes

### Computational Resources
- **CPU**: Moderate (LLVM compilation + analysis)
- **Memory**: ~1-2 GB peak
- **Disk**: ~100-500 MB per run (binaries + results)

### Parallelization
Current version evaluates candidates sequentially. Future versions may support parallel evaluation for faster optimization.

## Best Practices

### 1. Choose Appropriate Iterations
- **Testing/Development**: 3-5 iterations
- **Production**: 8-12 iterations
- **Critical Systems**: 15-20 iterations

### 2. Select Right Goal
- **IP Protection**: Use `security`
- **General Software**: Use `balanced`
- **Embedded/IoT**: Use `size`

### 3. Use Representative Input
Auto-tune with a file similar to your production code for best results.

### 4. Validate Results
Always test the optimized binary:
```bash
# Run functional tests
./final_optimized_binary

# Verify RE difficulty
cd reverse_engineering/scripts
python3 re_framework.py --binary ../../auto_tune_results/final_optimized_binary
```

### 5. Reuse Configurations
Save and version control your optimized configs:
```bash
cp auto_tune_results/final_optimized_config.yaml config/production_optimized.yaml
git add config/production_optimized.yaml
```

## Troubleshooting

### Issue: Low Fitness Scores

**Symptom:** All candidates have fitness < 30

**Solutions:**
1. Increase iterations (try 10+)
2. Use better base config (maximum_security.yaml)
3. Check if input file is too simple
4. Verify RE benchmark is working

### Issue: No Improvement After Iteration 0

**Symptom:** Best remains at baseline

**Solutions:**
1. Baseline may already be near-optimal
2. Try different optimization goal
3. Increase mutation rate (automatic after iteration 2)
4. Use more iterations for exploration

### Issue: Obfuscation Failures

**Symptom:** "Obfuscation failed for candidate X"

**Solutions:**
1. Check LLVM compatibility
2. Verify input file compiles normally
3. Reduce mutation aggression
4. Check build/error logs

### Issue: RE Benchmark Failures

**Symptom:** "RE benchmark failed for candidate X"

**Solutions:**
1. Ensure Python dependencies installed: `pip install -r requirements.txt`
2. Check binary is executable
3. Verify `reverse_engineering/scripts/re_framework.py` exists
4. Run benchmark manually for debugging

## Advanced Usage

### Custom Base Configuration

```bash
# Create custom base config
cp config/maximum_security.yaml config/my_base.yaml
# Edit my_base.yaml with your preferences

# Use custom base (modify run_auto_tune.sh or call directly)
./build/llvm-obfuscator \
    --auto-tune \
    --auto-tune-iterations 8 \
    -c config/my_base.yaml \
    -i input.c
```

### Integration with CI/CD

```yaml
# .github/workflows/optimize.yml
name: Auto-Tune Obfuscation

on: [push]

jobs:
  optimize:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - name: Build MAOS
        run: |
          mkdir -p build && cd build
          cmake .. && make
      - name: Auto-Tune
        run: |
          ./run_auto_tune.sh --iterations 5 --goal security src/main.c
      - name: Archive Results
        uses: actions/upload-artifact@v2
        with:
          name: optimized-config
          path: auto_tune_results/final_optimized_config.yaml
```

## API Reference

### AutoTuner Class

```cpp
class AutoTuner {
public:
    AutoTuner(const std::string& baseConfigPath,
              const std::string& inputFile,
              const std::string& outputDir);
    
    // Set optimization goal: "security", "balanced", "size"
    void setOptimizationGoal(const std::string& goal);
    
    // Run optimization for N iterations
    ConfigCandidate optimize(int iterations);
    
    // Get best configuration found
    const ConfigCandidate& getBestCandidate() const;
    
    // Generate comprehensive report
    void generateReport(const std::string& reportPath) const;
};
```

### EvaluationMetrics Structure

```cpp
struct EvaluationMetrics {
    double reDifficultyScore;      // 0-100
    double securityScore;          // 0-100
    double resilienceScore;        // 0-100
    double complexityScore;        // 0-100
    double cfgScore;               // 0-100
    // ... other component scores
    double compilationTime;        // seconds
    double binarySize;             // bytes
    bool functionalityPreserved;
    double fitnessScore;           // 0-100 weighted
};
```

## Future Enhancements

### Planned Features
- [ ] Parallel candidate evaluation
- [ ] Machine learning-based parameter prediction
- [ ] Multi-file optimization
- [ ] Genetic algorithm with crossover
- [ ] A/B testing framework
- [ ] Cloud-based optimization service
- [ ] Interactive parameter explorer

### Research Directions
- Bayesian optimization for parameter search
- Transfer learning between similar inputs
- Multi-objective Pareto frontier exploration
- Adversarial testing integration

## FAQ

**Q: How many iterations should I use?**  
A: Start with 5-8. If improving significantly, add more. 15+ for production.

**Q: Can I interrupt and resume?**  
A: Not currently. Each run is atomic. Future versions may support checkpointing.

**Q: What if baseline is already 85+?**  
A: You're already near-optimal! Minor improvements possible but harder to find.

**Q: Does it work with C++?**  
A: Yes! Works with C, C++, and any LLVM-compatible language.

**Q: How to compare different goals?**  
A: Run all three, compare fitness and RE difficulty in reports.

**Q: Can I customize fitness weights?**  
A: Currently via code. Future versions will support config-based weights.

## Support

For issues, questions, or contributions:
- GitHub Issues: [Report bugs or request features]
- Documentation: `docs/ARCHITECTURE.md`
- Examples: `tests/` directory

## License

MAOS v2.0 Auto-Tuning System
Copyright (c) 2025

---

**Last Updated:** October 13, 2025  
**Version:** 2.0.0  
**Status:** Production Ready ✅
