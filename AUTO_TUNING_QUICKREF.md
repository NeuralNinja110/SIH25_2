# MAOS v2.0 Auto-Tuning Quick Reference

## 🚀 Quick Commands

```bash
# Basic (5 iterations, balanced)
./run_auto_tune.sh input.c

# Security-focused
./run_auto_tune.sh --iterations 10 --goal security input.c

# Size-optimized
./run_auto_tune.sh --iterations 8 --goal size embedded.c

# Demo
./demo_auto_tune.sh
```

## 📊 Optimization Goals

| Goal | Security | Performance | Size | Best For |
|------|----------|-------------|------|----------|
| **security** | 80% | 15% | 5% | IP protection, commercial |
| **balanced** | 60% | 25% | 15% | General production use |
| **size** | 50% | 20% | 30% | Embedded, IoT devices |

## 📁 Output Files

```
auto_tune_results/
├── final_optimized_config.yaml    ← Use this in production!
├── final_optimized_binary         ← Best binary
└── optimization_report.md         ← Full analysis
```

## 🔄 Workflow

1. **Auto-tune** once to find best parameters
2. **Review** optimization report
3. **Apply** best config to all production files

```bash
# Step 1: Find optimal params
./run_auto_tune.sh --iterations 8 --goal security sample.c

# Step 2: Apply to production
./build/llvm-obfuscator \
    -c auto_tune_results/final_optimized_config.yaml \
    -i production.c -o production_obf
```

## 📈 Expected Improvements

| Iterations | RE Difficulty | Improvement |
|------------|---------------|-------------|
| Baseline | 40-45/100 | - |
| 3 | 70-78/100 | +60-75% |
| 8 | 80-88/100 | +90-110% |
| 15 | 85-92/100 | +100-120% |

## ⏱️ Time Estimates

| Iterations | Time |
|------------|------|
| 3 | 5-10 min |
| 5 | 10-20 min |
| 8 | 20-40 min |
| 15 | 40-80 min |

## 📖 Documentation

- **Quick Start:** `README.md`
- **User Guide:** `docs/AUTO_TUNING.md`
- **Technical:** `AUTO_TUNING_IMPLEMENTATION.md`

## 🔧 CLI Options

```bash
./build/llvm-obfuscator \
    --auto-tune \                          # Enable auto-tuning
    --auto-tune-iterations 8 \             # Number of iterations
    --auto-tune-goal security \            # Goal: security/balanced/size
    -i input.c
```

## 💡 Pro Tips

✅ Use 8-12 iterations for production  
✅ Test with representative code  
✅ Save optimized configs to version control  
✅ Reuse configs for similar projects  
✅ Run RE benchmark to validate results  

---

**Auto-Tuning: Automated Parameter Optimization for Maximum Security! 🔒**
