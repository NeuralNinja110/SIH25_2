#ifndef PCGE_H
#define PCGE_H

#include <vector>
#include <string>
#include <memory>
#include <map>
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"

// Polymorphic Code Generation Engine (PCGE)
// Generates multiple functionally equivalent code variants

namespace MAOS {

// Code variant representation
struct CodeVariant {
    std::string variantId;
    std::unique_ptr<llvm::Module> moduleIR;
    double semanticEquivalence;
    double structuralDifference;
    int morphingGeneration;
    std::vector<std::string> appliedTransformations;
};

// Metamorphic template
struct MetamorphicTemplate {
    std::string templateName;
    std::string inputPattern;
    std::vector<std::string> outputVariants;
    double complexityIncrease;
    int variantCount;
};

// Dynamic code morpher
class DynamicCodeMorpher {
public:
    DynamicCodeMorpher();
    ~DynamicCodeMorpher();
    
    // Morphing operations
    std::vector<CodeVariant> generateVariants(
        llvm::Module& originalModule,
        int variantCount
    );
    
    CodeVariant morphCode(
        llvm::Module& module,
        int morphingIntensity
    );
    
    // Transformation types
    enum class MorphType {
        INSTRUCTION_REORDERING,
        REGISTER_RENAMING,
        EXPRESSION_RESTRUCTURING,
        CONTROL_FLOW_VARIATION,
        DATA_LAYOUT_ALTERATION,
        CALLING_CONVENTION_CHANGE
    };
    
    void applyMorphing(llvm::Module& module, MorphType type);
    
    // Variant management
    bool verifySemanticEquivalence(const CodeVariant& v1, const CodeVariant& v2);
    double calculateStructuralDifference(const CodeVariant& v1, const CodeVariant& v2);
    size_t selectOptimalVariantIndex(const std::vector<CodeVariant>& variants);
    
    // Configuration
    void setMorphingIntensity(int intensity);
    void setVariantConstraints(double minDifference, double maxOverhead);
    
private:
    int morphingIntensity_;
    double minStructuralDifference_;
    double maxPerformanceOverhead_;
    
    std::vector<MorphType> enabledMorphTypes_;
    
    // Morphing strategies
    void reorderInstructions(llvm::Function& F);
    void renameRegisters(llvm::Function& F);
    void restructureExpressions(llvm::Function& F);
    void varyControlFlow(llvm::Function& F);
    void alterDataLayout(llvm::Module& M);
};

// Self-modifying IR generator
class SelfModifyingIRGenerator {
public:
    SelfModifyingIRGenerator();
    ~SelfModifyingIRGenerator();
    
    // IR transformation
    struct SelfModificationPoint {
        std::string locationId;
        llvm::Instruction* instruction;
        std::vector<llvm::Instruction*> alternatives;
        std::string triggerCondition;
        int modificationGeneration;
    };
    
    // Generation
    void injectSelfModification(llvm::Module& module, int intensity);
    std::vector<SelfModificationPoint> identifyModificationPoints(llvm::Module& module);
    
    // Self-modification strategies
    void createRuntimeMorpher(llvm::Function& F);
    void injectTransformationLogic(llvm::Function& F);
    void generateAlternativeImplementations(llvm::Function& F, int count);
    
    // Triggers
    enum class TriggerType {
        TIME_BASED,
        EXECUTION_COUNT,
        ENVIRONMENT_DETECTION,
        RANDOM_PROBABILITY,
        MULTI_CONDITION
    };
    
    void setTrigger(SelfModificationPoint& point, TriggerType type);
    
    // Safety
    bool verifySafety(const SelfModificationPoint& point);
    void validateModifications(llvm::Module& module);
    
private:
    std::vector<SelfModificationPoint> modificationPoints_;
    int intensity_;
    
    // Helper methods
    bool canSafelyModify(llvm::Instruction* inst);
    std::vector<llvm::Instruction*> generateAlternatives(llvm::Instruction* inst);
    void injectTriggerCode(llvm::Instruction* location, TriggerType type);
};

// Metamorphic template system
class MetamorphicTemplateSystem {
public:
    MetamorphicTemplateSystem();
    ~MetamorphicTemplateSystem();
    
    // Template management
    void loadTemplates();
    void addTemplate(const MetamorphicTemplate& tmpl);
    MetamorphicTemplate getTemplate(const std::string& name);
    
    // Template application
    std::vector<std::string> applyTemplate(
        const std::string& inputCode,
        const MetamorphicTemplate& tmpl
    );
    
    // Template generation
    MetamorphicTemplate generateTemplate(
        const std::string& pattern,
        int desiredVariants
    );
    
    // Pattern matching
    struct PatternMatch {
        std::string matchedPattern;
        std::vector<std::string> capturedGroups;
        int matchLocation;
        double confidence;
    };
    
    std::vector<PatternMatch> findPatterns(const std::string& code);
    
    // Database
    std::vector<MetamorphicTemplate> getAllTemplates();
    int getTemplateCount();
    
private:
    std::map<std::string, MetamorphicTemplate> templates_;
    std::vector<std::string> knownPatterns_;
    
    void initializeBuiltInTemplates();
    void loadFromDatabase();
    std::string applyTransformation(const std::string& input, const std::string& rule);
};

// Polymorphic function generator
class PolymorphicFunctionGenerator {
public:
    PolymorphicFunctionGenerator();
    ~PolymorphicFunctionGenerator();
    
    // Function variants
    struct FunctionVariant {
        std::string variantName;
        llvm::Function* function;
        std::vector<std::string> differences;
        double performance;
        double obfuscationLevel;
    };
    
    // Generation
    std::vector<FunctionVariant> generatePolymorphicVariants(
        llvm::Function& original,
        int variantCount
    );
    
    // Selection strategy
    enum class SelectionStrategy {
        RANDOM_SELECTION,
        PERFORMANCE_OPTIMAL,
        SECURITY_OPTIMAL,
        BALANCED,
        ADAPTIVE
    };
    
    FunctionVariant selectVariant(
        const std::vector<FunctionVariant>& variants,
        SelectionStrategy strategy
    );
    
    // Runtime polymorphism
    void injectVariantSelector(
        llvm::Module& module,
        const std::vector<FunctionVariant>& variants
    );
    
private:
    SelectionStrategy defaultStrategy_;
    
    // Variant creation strategies
    FunctionVariant createInstructionVariant(llvm::Function& F);
    FunctionVariant createControlFlowVariant(llvm::Function& F);
    FunctionVariant createDataFlowVariant(llvm::Function& F);
    
    // Analysis
    double analyzPerformance(const FunctionVariant& variant);
    double analyzeObfuscationLevel(const FunctionVariant& variant);
};

// Runtime code generator
class RuntimeCodeGenerator {
public:
    RuntimeCodeGenerator();
    ~RuntimeCodeGenerator();
    
    // Runtime generation
    void injectRuntimeCodegen(llvm::Module& module);
    void createJITStubs(llvm::Module& module);
    
    // Code cache
    struct CodeCache {
        std::map<std::string, std::vector<uint8_t>> generatedCode;
        std::map<std::string, uint64_t> lastAccessTime;
        size_t maxCacheSize;
    };
    
    void configureCodeCache(size_t maxSize);
    void clearCache();
    
    // Generation strategies
    enum class GenerationStrategy {
        ON_DEMAND,
        PREGENERATE,
        LAZY_GENERATION,
        ADAPTIVE_CACHING
    };
    
    void setGenerationStrategy(GenerationStrategy strategy);
    
private:
    CodeCache cache_;
    GenerationStrategy strategy_;
    
    void generateCodeAtRuntime(const std::string& functionId);
    std::vector<uint8_t> compileFunction(llvm::Function& F);
    void injectGenerationLogic(llvm::Module& module);
};

// PCGE Manager - Coordinates all polymorphic components
class PCGEManager {
public:
    PCGEManager();
    ~PCGEManager();
    
    // Initialization
    void initialize(llvm::Module& module);
    void configureForMode(bool maximumSecurity);
    
    // Component access
    DynamicCodeMorpher& getMorpher();
    SelfModifyingIRGenerator& getSelfModifyingGenerator();
    MetamorphicTemplateSystem& getTemplateSystem();
    PolymorphicFunctionGenerator& getFunctionGenerator();
    RuntimeCodeGenerator& getRuntimeGenerator();
    
    // Orchestration
    void applyPolymorphicTransformations(llvm::Module& module, int intensity);
    std::vector<CodeVariant> generateMultipleVariants(llvm::Module& module, int count);
    
    // Metrics
    struct PolymorphicMetrics {
        int variantsGenerated;
        int selfModificationPoints;
        int templatesApplied;
        int polymorphicFunctions;
        double averageStructuralDifference;
        double semanticEquivalenceScore;
    };
    
    PolymorphicMetrics collectMetrics();
    
    // Validation
    bool validateAllVariants();
    void verifySemanticPreservation();
    
private:
    std::unique_ptr<DynamicCodeMorpher> morpher_;
    std::unique_ptr<SelfModifyingIRGenerator> selfModifier_;
    std::unique_ptr<MetamorphicTemplateSystem> templateSystem_;
    std::unique_ptr<PolymorphicFunctionGenerator> functionGenerator_;
    std::unique_ptr<RuntimeCodeGenerator> runtimeGenerator_;
    
    bool initialized_;
    bool maximumSecurityMode_;
    llvm::Module* currentModule_;
};

} // namespace MAOS

#endif // PCGE_H
