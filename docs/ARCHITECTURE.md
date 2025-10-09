# LLVM Code Obfuscator - Architecture Documentation

## System Architecture

### Overview

The LLVM Code Obfuscator follows a modular, layered architecture designed for extensibility, maintainability, and performance.

```
┌─────────────────────────────────────────────┐
│          Command Line Interface             │
│           (CLIParser, main.cpp)             │
└─────────────────────────────────────────────┘
                    ↓
┌─────────────────────────────────────────────┐
│         Obfuscation Engine Core             │
│        (ObfuscationEngine.cpp)              │
└─────────────────────────────────────────────┘
                    ↓
┌─────────────────────────────────────────────┐
│           Pass Manager                       │
│         (PassManager.cpp)                    │
└─────────────────────────────────────────────┘
                    ↓
┌─────────────────────────────────────────────┐
│        Obfuscation Passes                   │
│  (ControlFlow, Instructions, Strings, etc)  │
└─────────────────────────────────────────────┘
                    ↓
┌─────────────────────────────────────────────┐
│         LLVM IR Transformations             │
│         (Module, Function, BB)              │
└─────────────────────────────────────────────┘
                    ↓
┌─────────────────────────────────────────────┐
│       Report Generation & Metrics           │
│     (ReportGenerator, MetricsCollector)     │
└─────────────────────────────────────────────┘
```

## Component Description

### 1. CLI Layer
- **CLIParser**: Parses command-line arguments
- **ConfigParser**: Loads configuration from files
- Validates input parameters
- Provides user-friendly error messages

### 2. Engine Core
- **ObfuscationEngine**: Main orchestrator
- Manages compilation pipeline
- Coordinates passes
- Handles file I/O

### 3. Pass Management
- **PassManager**: Orchestrates obfuscation passes
- Manages pass ordering
- Tracks transformations
- Collects metrics

### 4. Obfuscation Passes
Individual transformation modules:
- ControlFlowFlattening
- OpaquePredicates
- InstructionSubstitution
- BogusControlFlow
- DeadCodeInjection
- StringEncryption
- ConstantObfuscation
- FunctionVirtualization
- CallGraphObfuscation
- AntiDebug

### 5. Utilities
Support modules:
- **Logger**: Logging system
- **RandomGenerator**: Controlled randomness
- **FileUtils**: File operations
- **MetricsCollector**: Statistics gathering
- **ReportGenerator**: Report creation

## Data Flow

```
Source Code (.c/.cpp)
        ↓
    [Clang] Compilation to LLVM IR
        ↓
    LLVM Module (in-memory)
        ↓
    Pass 1: Instruction Substitution
        ↓
    Pass 2: String Encryption
        ↓
    Pass 3: Constant Obfuscation
        ↓
    Pass 4: Opaque Predicates
        ↓
    Pass 5: Bogus Control Flow
        ↓
    Pass 6: Dead Code Injection
        ↓
    Pass 7: Control Flow Flattening
        ↓
    [Multiple Cycles...]
        ↓
    Object File (.o)
        ↓
    [Linker]
        ↓
    Obfuscated Binary
        ↓
    Report Generation
```

## Design Patterns

### 1. Strategy Pattern
- ObfuscationPass hierarchy
- Different obfuscation strategies
- Runtime selection based on configuration

### 2. Singleton Pattern
- Logger
- RandomGenerator
- Metrics Collector

### 3. Factory Pattern
- Pass creation based on configuration
- Report format selection

### 4. Observer Pattern
- Metrics collection during transformation
- Progress reporting

## Key Algorithms

### Control Flow Flattening
```
1. Identify entry block
2. Create dispatch block with switch
3. Assign case numbers to all blocks
4. Replace terminators with switch updates
5. Route all branches through dispatcher
```

### Opaque Predicate Insertion
```
1. Select insertion points
2. Create predicates with known outcomes:
   - (x² ≥ 0)
   - (x | y ≥ x)
   - (2x = x + x)
3. Split basic blocks
4. Insert bogus branches
5. Ensure correct path always taken
```

### String Encryption
```
1. Identify string constants
2. For each string:
   - Generate random key
   - Encrypt: encrypted[i] = str[i] XOR key
   - Create encrypted global
   - Generate decryption stub
3. Replace references
```

### Constant Obfuscation
```
1. Find integer constants (value > 10)
2. Apply transformation:
   - Split: C = A + B
   - XOR: C = A XOR B
   - Multiply: C = A * B
3. Replace with computed expression
```

## Metrics Collection

### Tracked Metrics
- File sizes (before/after)
- Instruction counts
- Basic block counts
- Function counts
- Per-pass transformations
- Timing information
- String encryption statistics
- Constant obfuscation count

### Reporting
- JSON format for machine processing
- HTML format for human viewing
- Console summary

## Configuration System

### Hierarchy
1. Built-in defaults
2. Configuration file (JSON)
3. Command-line arguments (highest priority)

### Levels
- **Low**: Fast, basic obfuscation
- **Medium**: Balanced approach (default)
- **High**: Maximum security, slower

## Performance Considerations

### Optimization Strategies
1. **Lazy Evaluation**: Only process enabled passes
2. **Early Exit**: Skip unsuitable functions
3. **Batch Processing**: Group similar transformations
4. **Parallel Compilation**: Future enhancement

### Memory Management
- Smart pointers for LLVM objects
- RAII for resource cleanup
- Efficient pass over IR

## Security Features

### Defense Layers
1. **Syntactic**: Code structure obfuscation
2. **Semantic**: Logic obfuscation
3. **Dynamic**: Runtime checks
4. **Anti-Analysis**: Debugger detection

### Threat Model
Protected against:
- Static analysis tools
- Dynamic analysis/debugging
- Symbolic execution
- Binary diffing
- Automated deobfuscation

## Extensibility

### Adding New Passes
1. Inherit from `ObfuscationPass`
2. Implement `runOnModule()`
3. Register in `PassManager`
4. Add configuration options
5. Update documentation

### Example
```cpp
class MyCustomPass : public ObfuscationPass {
public:
    MyCustomPass() : ObfuscationPass("MyCustomPass") {}
    
    bool runOnModule(llvm::Module& module, 
                    MetricsCollector& metrics) override {
        // Implementation
        return modified;
    }
};
```

## Testing Strategy

### Unit Tests
- Individual pass functionality
- Configuration validation
- Utility functions

### Integration Tests
- End-to-end obfuscation
- Multiple test cases
- Performance benchmarks

### Validation
- Output correctness
- Metrics accuracy
- Report generation

## Build System

### CMake Structure
- Top-level CMakeLists.txt
- LLVM integration
- Library creation
- Executable linking
- Test configuration

### Dependencies
- LLVM/Clang 14+
- CMake 3.20+
- C++17 compiler

## Future Enhancements

### Planned Features
1. Machine learning-based obfuscation
2. Cross-architecture support
3. WebAssembly target
4. GUI configuration tool
5. Build system integration
6. Advanced virtualization
7. Code splitting
8. Polymorphic code generation

## License & Usage

- Professional-grade implementation
- Patent-worthy innovations
- Follows international coding standards
- Suitable for commercial use

---

**Document Version**: 1.0.0  
**Last Updated**: 2025-10-09  
**Maintained By**: NeuralNinja110
