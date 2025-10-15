/**
 * Test Case 9: Preprocessor Macros and Conditional Compilation
 * Tests: #define, #ifdef, #ifndef, macros, token pasting, stringification
 */

#include <stdio.h>
#include <string.h>

// Define constants
#define MAX_SIZE 100
#define PI 3.14159
#define VERSION "1.0.0"

// Macro functions
#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(x) ((x) < 0 ? -(x) : (x))

// Multi-line macro
#define SWAP(a, b, type) do { \
    type temp = a;            \
    a = b;                    \
    b = temp;                 \
} while(0)

// Stringification
#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

// Token pasting
#define CONCAT(a, b) a##b
#define MAKE_VAR(name, num) name##num

// Conditional compilation
#define DEBUG_MODE 1
#define FEATURE_LOGGING 1
#define PLATFORM_LINUX 1

#ifdef DEBUG_MODE
    #define DEBUG_PRINT(fmt, ...) printf("DEBUG: " fmt "\n", ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...) // No-op
#endif

#if FEATURE_LOGGING
    #define LOG(msg) printf("[LOG] %s\n", msg)
#else
    #define LOG(msg)
#endif

// Compiler information
#define COMPILER_INFO() printf("Compiled with: %s %s on %s at %s\n", \
    __FILE__, __VERSION__, __DATE__, __TIME__)

// Variadic macros
#define PRINT_ARGS(...) printf(__VA_ARGS__)
#define PRINT_VALUES(fmt, ...) printf("Values: " fmt "\n", ##__VA_ARGS__)

// Macro for loop unrolling
#define UNROLL_4(op, i) \
    op(i);     \
    op(i + 1); \
    op(i + 2); \
    op(i + 3)

// Conditional macros
#ifdef PLATFORM_LINUX
    #define PLATFORM_NAME "Linux"
    #define PATH_SEPARATOR '/'
#elif defined(PLATFORM_WINDOWS)
    #define PLATFORM_NAME "Windows"
    #define PATH_SEPARATOR '\\'
#else
    #define PLATFORM_NAME "Unknown"
    #define PATH_SEPARATOR '/'
#endif

// Macro to create struct
#define DEFINE_POINT_STRUCT(type) \
    typedef struct {              \
        type x;                   \
        type y;                   \
    } Point_##type

DEFINE_POINT_STRUCT(int);
DEFINE_POINT_STRUCT(float);

// Array size macro
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

// Bit manipulation macros
#define SET_BIT(num, pos) ((num) | (1 << (pos)))
#define CLEAR_BIT(num, pos) ((num) & ~(1 << (pos)))
#define TOGGLE_BIT(num, pos) ((num) ^ (1 << (pos)))
#define CHECK_BIT(num, pos) (((num) >> (pos)) & 1)

// Macro for range checking
#define IN_RANGE(val, min, max) ((val) >= (min) && (val) <= (max))

// Function-like macro with multiple statements
#define PRINT_ARRAY(arr, size) do {     \
    printf("[");                         \
    for (int i = 0; i < size; i++) {    \
        printf("%d", arr[i]);            \
        if (i < size - 1) printf(", "); \
    }                                    \
    printf("]\n");                       \
} while(0)

// Compile-time assertion
#define STATIC_ASSERT(expr) typedef char static_assert_[(expr) ? 1 : -1]

// Test function for unrolling
void process(int i) {
    printf("%d ", i);
}

int main() {
    printf("=== Preprocessor Macros Test ===\n");
    
    // 1. Basic macros
    printf("\n1. Basic Macros:\n");
    printf("MAX_SIZE = %d\n", MAX_SIZE);
    printf("PI = %.5f\n", PI);
    printf("VERSION = %s\n", VERSION);
    
    // 2. Macro functions
    printf("\n2. Macro Functions:\n");
    printf("SQUARE(5) = %d\n", SQUARE(5));
    printf("MAX(10, 20) = %d\n", MAX(10, 20));
    printf("MIN(10, 20) = %d\n", MIN(10, 20));
    printf("ABS(-15) = %d\n", ABS(-15));
    
    // 3. SWAP macro
    printf("\n3. SWAP Macro:\n");
    int a = 10, b = 20;
    printf("Before: a=%d, b=%d\n", a, b);
    SWAP(a, b, int);
    printf("After:  a=%d, b=%d\n", a, b);
    
    // 4. Stringification
    printf("\n4. Stringification:\n");
    printf("STRINGIFY(MAX_SIZE) = %s\n", STRINGIFY(MAX_SIZE));
    printf("TO_STRING(123) = %s\n", TO_STRING(123));
    
    // 5. Token pasting
    printf("\n5. Token Pasting:\n");
    int MAKE_VAR(value, 1) = 100;
    int MAKE_VAR(value, 2) = 200;
    printf("value1 = %d\n", value1);
    printf("value2 = %d\n", value2);
    
    // 6. Debug macros
    printf("\n6. Debug Macros:\n");
    DEBUG_PRINT("This is a debug message");
    DEBUG_PRINT("Value: %d", 42);
    LOG("Application started");
    
    // 7. Variadic macros
    printf("\n7. Variadic Macros:\n");
    PRINT_ARGS("Hello, %s! Number: %d\n", "World", 123);
    PRINT_VALUES("%d %d %d", 1, 2, 3);
    
    // 8. Platform detection
    printf("\n8. Platform Detection:\n");
    printf("Platform: %s\n", PLATFORM_NAME);
    printf("Path separator: '%c'\n", PATH_SEPARATOR);
    
    // 9. Generated structs
    printf("\n9. Generated Structs:\n");
    Point_int p1 = {10, 20};
    Point_float p2 = {3.5f, 7.2f};
    printf("Point_int: (%d, %d)\n", p1.x, p1.y);
    printf("Point_float: (%.1f, %.1f)\n", p2.x, p2.y);
    
    // 10. Array size
    printf("\n10. Array Size:\n");
    int numbers[] = {1, 2, 3, 4, 5};
    printf("Array size: %zu\n", ARRAY_SIZE(numbers));
    
    // 11. Bit manipulation macros
    printf("\n11. Bit Manipulation Macros:\n");
    unsigned int num = 0;
    num = SET_BIT(num, 2);
    num = SET_BIT(num, 5);
    printf("After setting bits 2 and 5: %u\n", num);
    printf("Bit 2 is: %d\n", CHECK_BIT(num, 2));
    printf("Bit 3 is: %d\n", CHECK_BIT(num, 3));
    
    // 12. Range checking
    printf("\n12. Range Checking:\n");
    int value = 50;
    printf("%d in range [0, 100]: %s\n", value, 
           IN_RANGE(value, 0, 100) ? "Yes" : "No");
    printf("%d in range [60, 100]: %s\n", value,
           IN_RANGE(value, 60, 100) ? "Yes" : "No");
    
    // 13. Array printing
    printf("\n13. Array Printing Macro:\n");
    int arr[] = {10, 20, 30, 40, 50};
    PRINT_ARRAY(arr, 5);
    
    // 14. Loop unrolling
    printf("\n14. Loop Unrolling:\n");
    printf("Unrolled loop: ");
    UNROLL_4(process, 0);
    printf("\n");
    
    // 15. Compiler information
    printf("\n15. Compiler Information:\n");
    printf("File: %s\n", __FILE__);
    printf("Line: %d\n", __LINE__);
    printf("Date: %s\n", __DATE__);
    printf("Time: %s\n", __TIME__);
    
    // 16. Conditional features
    printf("\n16. Conditional Features:\n");
    #ifdef DEBUG_MODE
    printf("Debug mode is ENABLED\n");
    #else
    printf("Debug mode is DISABLED\n");
    #endif
    
    #if FEATURE_LOGGING
    printf("Logging feature is ENABLED\n");
    #else
    printf("Logging feature is DISABLED\n");
    #endif
    
    printf("\n=== Preprocessor Macros Test Complete ===\n");
    return 0;
}
