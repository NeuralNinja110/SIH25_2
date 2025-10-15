cmake_minimum_required(VERSION 3.20)
project(MAOS VERSION 2.0.0 LANGUAGES CXX C)  # Multi-Layered Adaptive Obfuscation System

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Find LLVM
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Find required libraries
find_package(Threads REQUIRED)

# Library sources
set(LIB_SOURCES
    # Core obfuscation infrastructure
    src/core/ObfuscationEngine.cpp
    src/core/ObfuscationPass.cpp
    src/core/PassManager.cpp
    src/config/ConfigParser.cpp
    src/config/ObfuscationConfig.cpp
    src/report/ReportGenerator.cpp
    src/report/MetricsCollector.cpp
    src/utils/RandomGenerator.cpp
    src/utils/Logger.cpp
    src/utils/FileUtils.cpp
    src/utils/AutoTuner.cpp
    
    # Advanced Quantum & Hardware Passes (v2.0)
    src/passes/QuantumOpaquePredicates.cpp
    src/passes/HardwareCacheObfuscation.cpp
    src/passes/MBAObfuscation.cpp
    src/passes/GrammarMetamorphic.cpp
    
    # MAOS Components (ATIE, PCGE, QIRL)
    src/passes/ControlFlowFlattening.cpp
    src/passes/StringEncryption.cpp
    src/passes/DeadCodeInjection.cpp
    src/passes/CallGraphObfuscation.cpp
    src/passes/ConstantObfuscation.cpp
    src/passes/AntiDebug.cpp
    
    # MAOS components
    src/core/ATIE.cpp
    src/core/QIRL.cpp
    src/core/PCGE.cpp
    src/core/MAOSEngine.cpp
)

# Create static library
add_library(obfuscator_lib STATIC ${LIB_SOURCES})

# Link LLVM libraries
llvm_map_components_to_libnames(llvm_libs 
    core 
    support 
    irreader 
    bitwriter
    transformutils
    analysis
    scalaropts
    instcombine
    ipo
    passes
    target
    mc
    object
    asmparser
)

target_link_libraries(obfuscator_lib PUBLIC ${llvm_libs})
target_link_libraries(obfuscator_lib PRIVATE Threads::Threads)

# Main executable
add_executable(phantron-llvm-obfuscator src/main.cpp src/cli/CLIParser.cpp)
target_link_libraries(phantron-llvm-obfuscator PRIVATE obfuscator_lib)

# Test executable
add_executable(obfuscator_tests 
    tests/test_main.cpp
    tests/test_obfuscation.cpp
)
target_link_libraries(obfuscator_tests PRIVATE obfuscator_lib)

# Install targets
install(TARGETS phantron-llvm-obfuscator DESTINATION bin)
install(TARGETS obfuscator_lib DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

# Enable testing
enable_testing()
add_test(NAME obfuscator_tests COMMAND obfuscator_tests)
