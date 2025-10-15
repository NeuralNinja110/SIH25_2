/**
 * Test Case 15: Mixed Complexity - Kitchen Sink
 * Tests: Everything combined - unions, enums, goto, recursion, bit fields,
 *        inline assembly (commented), volatile, const, static, extern
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <time.h>
#include <stdarg.h>

// Enumerations
typedef enum {
    STATUS_OK = 0,
    STATUS_ERROR = 1,
    STATUS_PENDING = 2,
    STATUS_TIMEOUT = 3
} Status;

typedef enum {
    RED = 1,
    GREEN = 2,
    BLUE = 4,
    ALPHA = 8
} ColorFlags;

// Unions
typedef union {
    int as_int;
    float as_float;
    char as_bytes[4];
} Value;

typedef union {
    struct {
        unsigned char r, g, b, a;
    } rgba;
    unsigned int value;
} Color;

// Bit fields
typedef struct {
    unsigned int read : 1;
    unsigned int write : 1;
    unsigned int execute : 1;
    unsigned int reserved : 5;
    unsigned int uid : 16;
    unsigned int gid : 8;
} Permissions;

// Static and extern variables
static int static_counter = 0;
extern char **environ;  // External environment variables

// Volatile for signal handling or hardware access
volatile int interrupted = 0;

// Const correctness
const char* const CONST_MESSAGE = "Immutable message";

// setjmp/longjmp for non-local jumps
jmp_buf jump_buffer;

// Test union type punning
void test_unions() {
    printf("\n1. Unions and Type Punning:\n");
    
    Value v;
    v.as_float = 3.14159f;
    printf("As float: %f\n", v.as_float);
    printf("As int (bits): 0x%08X\n", v.as_int);
    printf("As bytes: %02X %02X %02X %02X\n", 
           v.as_bytes[0], v.as_bytes[1], v.as_bytes[2], v.as_bytes[3]);
    
    Color c;
    c.rgba.r = 255;
    c.rgba.g = 128;
    c.rgba.b = 64;
    c.rgba.a = 255;
    printf("\nColor RGBA: (%d, %d, %d, %d)\n", 
           c.rgba.r, c.rgba.g, c.rgba.b, c.rgba.a);
    printf("Color as uint: 0x%08X\n", c.value);
}

// Test enums
void test_enums() {
    printf("\n2. Enumerations:\n");
    
    Status s = STATUS_OK;
    printf("Status: %d (%s)\n", s, s == STATUS_OK ? "OK" : "ERROR");
    
    // Enum as bit flags
    int flags = RED | BLUE | ALPHA;
    printf("Color flags: 0x%02X\n", flags);
    printf("Has RED: %s\n", (flags & RED) ? "Yes" : "No");
    printf("Has GREEN: %s\n", (flags & GREEN) ? "Yes" : "No");
    printf("Has ALPHA: %s\n", (flags & ALPHA) ? "Yes" : "No");
}

// Test bit fields
void test_bit_fields() {
    printf("\n3. Bit Fields:\n");
    
    Permissions perm;
    perm.read = 1;
    perm.write = 1;
    perm.execute = 0;
    perm.reserved = 0;
    perm.uid = 1000;
    perm.gid = 100;
    
    printf("Permissions size: %zu bytes\n", sizeof(perm));
    printf("Read: %u, Write: %u, Execute: %u\n", 
           perm.read, perm.write, perm.execute);
    printf("UID: %u, GID: %u\n", perm.uid, perm.gid);
}

// Test goto (state machine)
void test_goto_state_machine() {
    printf("\n4. goto Statement (State Machine):\n");
    
    int state = 0;
    int input = 5;
    
STATE_START:
    printf("State START (input=%d)\n", input);
    if (input <= 0) goto STATE_ERROR;
    if (input > 10) goto STATE_END;
    state = 1;
    input += 3;
    goto STATE_PROCESS;
    
STATE_PROCESS:
    printf("State PROCESS (input=%d)\n", input);
    if (input > 15) goto STATE_END;
    state = 2;
    input *= 2;
    goto STATE_FINALIZE;
    
STATE_FINALIZE:
    printf("State FINALIZE (input=%d)\n", input);
    state = 3;
    goto STATE_END;
    
STATE_ERROR:
    printf("State ERROR\n");
    state = -1;
    goto STATE_END;
    
STATE_END:
    printf("State END (final state=%d, input=%d)\n", state, input);
}

// Recursive function
int fibonacci_recursive(int n) {
    if (n <= 1) return n;
    return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
}

// Tail recursive (optimizable)
int factorial_tail(int n, int acc) {
    if (n <= 1) return acc;
    return factorial_tail(n - 1, n * acc);
}

void test_recursion() {
    printf("\n5. Recursion:\n");
    
    printf("Fibonacci(10) = %d\n", fibonacci_recursive(10));
    printf("Factorial(5) = %d\n", factorial_tail(5, 1));
}

// setjmp/longjmp for error handling
void deep_function() {
    printf("Deep function: simulating error...\n");
    longjmp(jump_buffer, 42);  // Jump back with value 42
    printf("This will never print\n");
}

void middle_function() {
    printf("Middle function\n");
    deep_function();
}

void test_setjmp_longjmp() {
    printf("\n6. setjmp/longjmp (Non-local Jump):\n");
    
    int ret = setjmp(jump_buffer);
    if (ret == 0) {
        printf("First time through setjmp\n");
        middle_function();
    } else {
        printf("Returned from longjmp with value: %d\n", ret);
    }
}

// Static variables
void test_static() {
    printf("\n7. Static Variables:\n");
    
    static_counter++;
    printf("Static counter (in function): %d\n", static_counter);
    
    static int local_static = 0;
    local_static += 10;
    printf("Local static: %d\n", local_static);
}

// Inline function (hint to compiler)
static inline int max_inline(int a, int b) {
    return a > b ? a : b;
}

void test_inline() {
    printf("\n8. Inline Functions:\n");
    printf("max_inline(10, 20) = %d\n", max_inline(10, 20));
}

// Volatile usage
void test_volatile() {
    printf("\n9. Volatile Variables:\n");
    
    volatile int sensor_value = 42;
    printf("Sensor value: %d\n", sensor_value);
    
    // Simulate hardware update
    sensor_value = 100;
    printf("Updated sensor value: %d\n", sensor_value);
}

// Const correctness
void test_const() {
    printf("\n10. Const Correctness:\n");
    
    const int const_int = 42;
    printf("Const int: %d\n", const_int);
    printf("Const message: %s\n", CONST_MESSAGE);
    
    const char* str1 = "Hello";  // Pointer to const
    char* const str2 = (char*)"World";  // Const pointer (cast for demo)
    printf("str1: %s, str2: %s\n", str1, str2);
}

// Nested structures
void test_nested_structures() {
    printf("\n11. Nested Structures:\n");
    
    typedef struct {
        int x, y;
    } Point;
    
    typedef struct {
        Point top_left;
        Point bottom_right;
    } Rectangle;
    
    Rectangle rect = {{10, 20}, {100, 200}};
    printf("Rectangle: (%d,%d) to (%d,%d)\n",
           rect.top_left.x, rect.top_left.y,
           rect.bottom_right.x, rect.bottom_right.y);
}

// Function pointers in struct (vtable)
typedef struct {
    void (*init)(void);
    int (*process)(int);
    void (*cleanup)(void);
} Operations;

void op_init() { printf("  Operations initialized\n"); }
int op_process(int x) { return x * 2; }
void op_cleanup() { printf("  Operations cleaned up\n"); }

void test_function_table() {
    printf("\n12. Function Pointer Table:\n");
    
    Operations ops = {op_init, op_process, op_cleanup};
    
    ops.init();
    printf("  Process result: %d\n", ops.process(21));
    ops.cleanup();
}

// Designated initializers
void test_designated_init() {
    printf("\n13. Designated Initializers:\n");
    
    int arr[10] = {[0] = 1, [5] = 2, [9] = 3};
    printf("Array with designated init: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Compound literals
typedef struct { int x, y; } Point;

void print_point(Point p) {
    printf("  Point: (%d, %d)\n", p.x, p.y);
}

void test_compound_literals() {
    printf("\n14. Compound Literals:\n");
    
    print_point((Point){5, 10});
    print_point((Point){.y = 20, .x = 15});
}

// Variable length arrays (VLA) - C99 feature
void test_vla(int n) {
    printf("\n15. Variable Length Arrays:\n");
    
    int vla[n];
    printf("VLA size: %d\n", n);
    
    for (int i = 0; i < n; i++) {
        vla[i] = i * i;
    }
    
    printf("VLA contents: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", vla[i]);
    }
    printf("\n");
}

// Restrict keyword (C99)
void copy_array_restrict(int * restrict dest, const int * restrict src, size_t n) {
    for (size_t i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

void test_restrict() {
    printf("\n16. Restrict Keyword:\n");
    
    int src[] = {1, 2, 3, 4, 5};
    int dest[5];
    
    copy_array_restrict(dest, src, 5);
    
    printf("Copied array: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", dest[i]);
    }
    printf("\n");
}

int main() {
    printf("=== Mixed Complexity Test (Kitchen Sink) ===\n");
    
    test_unions();
    test_enums();
    test_bit_fields();
    test_goto_state_machine();
    test_recursion();
    test_setjmp_longjmp();
    test_static();
    test_static();  // Call again to see static increment
    test_inline();
    test_volatile();
    test_const();
    test_nested_structures();
    test_function_table();
    test_designated_init();
    test_compound_literals();
    test_vla(5);
    test_restrict();
    
    printf("\n=== Mixed Complexity Test Complete ===\n");
    printf("This test demonstrates: unions, enums, bit fields, goto, recursion,\n");
    printf("setjmp/longjmp, static, inline, volatile, const, nested structs,\n");
    printf("function pointers, designated initializers, compound literals,\n");
    printf("VLAs, restrict keyword, and more!\n");
    
    return 0;
}
