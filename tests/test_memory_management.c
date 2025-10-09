/**
 * Test Case 14: Advanced Memory Management
 * Tests: malloc, calloc, realloc, memory pools, alignment, custom allocators
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#define POOL_SIZE 1024
#define ALIGN_SIZE 8

// Memory statistics
typedef struct {
    size_t total_allocated;
    size_t total_freed;
    size_t current_usage;
    size_t peak_usage;
    int alloc_count;
    int free_count;
} MemStats;

MemStats global_stats = {0};

// Custom allocator wrapper
void* tracked_malloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr) {
        global_stats.total_allocated += size;
        global_stats.current_usage += size;
        global_stats.alloc_count++;
        if (global_stats.current_usage > global_stats.peak_usage) {
            global_stats.peak_usage = global_stats.current_usage;
        }
    }
    return ptr;
}

void tracked_free(void* ptr, size_t size) {
    if (ptr) {
        free(ptr);
        global_stats.total_freed += size;
        global_stats.current_usage -= size;
        global_stats.free_count++;
    }
}

// Memory pool allocator
typedef struct MemPool {
    char memory[POOL_SIZE];
    size_t offset;
    size_t size;
} MemPool;

void pool_init(MemPool* pool) {
    pool->offset = 0;
    pool->size = POOL_SIZE;
    memset(pool->memory, 0, POOL_SIZE);
}

void* pool_alloc(MemPool* pool, size_t size) {
    // Align to ALIGN_SIZE boundary
    size_t aligned_size = (size + ALIGN_SIZE - 1) & ~(ALIGN_SIZE - 1);
    
    if (pool->offset + aligned_size > pool->size) {
        return NULL;  // Pool exhausted
    }
    
    void* ptr = pool->memory + pool->offset;
    pool->offset += aligned_size;
    return ptr;
}

void pool_reset(MemPool* pool) {
    pool->offset = 0;
    memset(pool->memory, 0, POOL_SIZE);
}

// Test basic allocation
void test_basic_allocation() {
    printf("\n1. Basic Allocation:\n");
    
    int* arr = (int*)tracked_malloc(10 * sizeof(int));
    if (!arr) {
        printf("Allocation failed!\n");
        return;
    }
    
    printf("Allocated array of 10 ints\n");
    for (int i = 0; i < 10; i++) {
        arr[i] = i * i;
    }
    
    printf("Array: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    tracked_free(arr, 10 * sizeof(int));
    printf("Memory freed\n");
}

// Test calloc vs malloc
void test_calloc_vs_malloc() {
    printf("\n2. calloc vs malloc:\n");
    
    int* arr1 = (int*)malloc(5 * sizeof(int));
    int* arr2 = (int*)calloc(5, sizeof(int));
    
    printf("malloc (uninitialized): ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr1[i]);
    }
    printf("\n");
    
    printf("calloc (zero-initialized): ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr2[i]);
    }
    printf("\n");
    
    free(arr1);
    free(arr2);
}

// Test realloc
void test_realloc() {
    printf("\n3. Dynamic Resizing with realloc:\n");
    
    int* arr = (int*)malloc(5 * sizeof(int));
    printf("Initial allocation: 5 elements\n");
    
    for (int i = 0; i < 5; i++) {
        arr[i] = i;
    }
    
    printf("Before realloc: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    arr = (int*)realloc(arr, 10 * sizeof(int));
    printf("After realloc: 10 elements\n");
    
    for (int i = 5; i < 10; i++) {
        arr[i] = i;
    }
    
    printf("After realloc: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    free(arr);
}

// Test memory pool
void test_memory_pool() {
    printf("\n4. Memory Pool Allocator:\n");
    
    MemPool pool;
    pool_init(&pool);
    
    printf("Pool size: %zu bytes\n", pool.size);
    
    int* arr1 = (int*)pool_alloc(&pool, 10 * sizeof(int));
    printf("Allocated 10 ints from pool (offset: %zu)\n", pool.offset);
    
    char* str = (char*)pool_alloc(&pool, 100 * sizeof(char));
    strcpy(str, "Hello from memory pool!");
    printf("Allocated 100 chars from pool (offset: %zu)\n", pool.offset);
    printf("String: %s\n", str);
    
    double* arr2 = (double*)pool_alloc(&pool, 20 * sizeof(double));
    printf("Allocated 20 doubles from pool (offset: %zu)\n", pool.offset);
    
    printf("Pool usage: %zu / %zu bytes\n", pool.offset, pool.size);
    
    pool_reset(&pool);
    printf("Pool reset (offset: %zu)\n", pool.offset);
}

// Test alignment
void test_alignment() {
    printf("\n5. Memory Alignment:\n");
    
    char* ptr1 = (char*)malloc(1);
    short* ptr2 = (short*)malloc(sizeof(short));
    int* ptr3 = (int*)malloc(sizeof(int));
    long* ptr4 = (long*)malloc(sizeof(long));
    double* ptr5 = (double*)malloc(sizeof(double));
    
    printf("char   address: %p (alignment: %zu)\n", (void*)ptr1, (size_t)ptr1 % 8);
    printf("short  address: %p (alignment: %zu)\n", (void*)ptr2, (size_t)ptr2 % 8);
    printf("int    address: %p (alignment: %zu)\n", (void*)ptr3, (size_t)ptr3 % 8);
    printf("long   address: %p (alignment: %zu)\n", (void*)ptr4, (size_t)ptr4 % 8);
    printf("double address: %p (alignment: %zu)\n", (void*)ptr5, (size_t)ptr5 % 8);
    
    free(ptr1);
    free(ptr2);
    free(ptr3);
    free(ptr4);
    free(ptr5);
}

// Test memory leaks detection (simple)
void test_memory_leaks() {
    printf("\n6. Memory Leak Detection:\n");
    
    void* ptrs[5];
    size_t sizes[] = {100, 200, 300, 400, 500};
    
    // Allocate
    for (int i = 0; i < 5; i++) {
        ptrs[i] = tracked_malloc(sizes[i]);
        printf("Allocated %zu bytes at %p\n", sizes[i], ptrs[i]);
    }
    
    printf("\nCurrent usage: %zu bytes\n", global_stats.current_usage);
    
    // Free only first 3
    for (int i = 0; i < 3; i++) {
        tracked_free(ptrs[i], sizes[i]);
        printf("Freed %zu bytes at %p\n", sizes[i], ptrs[i]);
    }
    
    printf("\nAfter partial free:\n");
    printf("Current usage: %zu bytes\n", global_stats.current_usage);
    printf("Leaked: %zu bytes\n", 
           global_stats.total_allocated - global_stats.total_freed);
    
    // Clean up remaining
    for (int i = 3; i < 5; i++) {
        tracked_free(ptrs[i], sizes[i]);
    }
}

// Test struct allocation
void test_struct_allocation() {
    printf("\n7. Structure Allocation:\n");
    
    typedef struct {
        char name[50];
        int age;
        double salary;
    } Person;
    
    Person* p1 = (Person*)malloc(sizeof(Person));
    strcpy(p1->name, "Alice");
    p1->age = 30;
    p1->salary = 75000.0;
    
    printf("Person: %s, Age: %d, Salary: %.2f\n", 
           p1->name, p1->age, p1->salary);
    
    // Array of structs
    Person* team = (Person*)calloc(3, sizeof(Person));
    strcpy(team[0].name, "Bob");
    team[0].age = 25;
    strcpy(team[1].name, "Charlie");
    team[1].age = 35;
    strcpy(team[2].name, "Diana");
    team[2].age = 28;
    
    printf("\nTeam:\n");
    for (int i = 0; i < 3; i++) {
        printf("  %s (age %d)\n", team[i].name, team[i].age);
    }
    
    free(p1);
    free(team);
}

// Test 2D array allocation
void test_2d_array() {
    printf("\n8. 2D Array Allocation:\n");
    
    int rows = 3, cols = 4;
    
    // Method 1: Array of pointers
    int** matrix = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
    }
    
    // Fill matrix
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = i * cols + j;
        }
    }
    
    printf("Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }
    
    // Free matrix
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Test flexible array member
void test_flexible_array() {
    printf("\n9. Flexible Array Member:\n");
    
    typedef struct {
        int size;
        int data[];  // Flexible array member
    } DynamicArray;
    
    int n = 5;
    DynamicArray* arr = (DynamicArray*)malloc(sizeof(DynamicArray) + n * sizeof(int));
    arr->size = n;
    
    for (int i = 0; i < n; i++) {
        arr->data[i] = i * 10;
    }
    
    printf("Dynamic array (size %d): ", arr->size);
    for (int i = 0; i < arr->size; i++) {
        printf("%d ", arr->data[i]);
    }
    printf("\n");
    
    free(arr);
}

// Print memory statistics
void print_stats() {
    printf("\n10. Memory Statistics:\n");
    printf("Total allocated: %zu bytes\n", global_stats.total_allocated);
    printf("Total freed:     %zu bytes\n", global_stats.total_freed);
    printf("Current usage:   %zu bytes\n", global_stats.current_usage);
    printf("Peak usage:      %zu bytes\n", global_stats.peak_usage);
    printf("Alloc count:     %d\n", global_stats.alloc_count);
    printf("Free count:      %d\n", global_stats.free_count);
}

int main() {
    printf("=== Advanced Memory Management Test ===\n");
    
    test_basic_allocation();
    test_calloc_vs_malloc();
    test_realloc();
    test_memory_pool();
    test_alignment();
    test_memory_leaks();
    test_struct_allocation();
    test_2d_array();
    test_flexible_array();
    print_stats();
    
    printf("\n=== Advanced Memory Management Test Complete ===\n");
    return 0;
}
