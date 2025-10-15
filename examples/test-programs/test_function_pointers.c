/**
 * Test Case 6: Function Pointers and Callbacks
 * Tests: Function pointers, callbacks, vtables, dynamic dispatch, sorting with comparators
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Function pointer type definitions
typedef int (*BinaryOp)(int, int);
typedef void (*Callback)(int);
typedef int (*Comparator)(const void*, const void*);

// Basic arithmetic operations
int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }
int divide(int a, int b) { return b != 0 ? a / b : 0; }

// Apply operation using function pointer
int apply_operation(int a, int b, BinaryOp op) {
    return op(a, b);
}

// Callback functions
void print_number(int n) {
    printf("%d ", n);
}

void print_square(int n) {
    printf("%d ", n * n);
}

// Execute callback for each element
void for_each(int* arr, int size, Callback cb) {
    for (int i = 0; i < size; i++) {
        cb(arr[i]);
    }
}

// Comparators for sorting
int compare_ascending(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int compare_descending(const void* a, const void* b) {
    return (*(int*)b - *(int*)a);
}

// String comparator
int compare_strings(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

// Calculator function using function pointer table
int calculate(char op, int a, int b) {
    BinaryOp operations[4] = {add, subtract, multiply, divide};
    const char* symbols = "+-*/";
    
    for (int i = 0; i < 4; i++) {
        if (symbols[i] == op) {
            return operations[i](a, b);
        }
    }
    return 0;
}

// Simple OOP: Shape vtable simulation
typedef struct Shape {
    double (*area)(struct Shape*);
    double (*perimeter)(struct Shape*);
    void (*print)(struct Shape*);
    void* data;
} Shape;

typedef struct {
    double radius;
} Circle;

typedef struct {
    double width;
    double height;
} Rectangle;

// Circle methods
double circle_area(Shape* s) {
    Circle* c = (Circle*)s->data;
    return 3.14159 * c->radius * c->radius;
}

double circle_perimeter(Shape* s) {
    Circle* c = (Circle*)s->data;
    return 2 * 3.14159 * c->radius;
}

void circle_print(Shape* s) {
    Circle* c = (Circle*)s->data;
    printf("Circle(radius=%.2f)\n", c->radius);
}

// Rectangle methods
double rectangle_area(Shape* s) {
    Rectangle* r = (Rectangle*)s->data;
    return r->width * r->height;
}

double rectangle_perimeter(Shape* s) {
    Rectangle* r = (Rectangle*)s->data;
    return 2 * (r->width + r->height);
}

void rectangle_print(Shape* s) {
    Rectangle* r = (Rectangle*)s->data;
    printf("Rectangle(width=%.2f, height=%.2f)\n", r->width, r->height);
}

// Create shape objects
Shape* create_circle(double radius) {
    Shape* s = (Shape*)malloc(sizeof(Shape));
    Circle* c = (Circle*)malloc(sizeof(Circle));
    c->radius = radius;
    s->data = c;
    s->area = circle_area;
    s->perimeter = circle_perimeter;
    s->print = circle_print;
    return s;
}

Shape* create_rectangle(double width, double height) {
    Shape* s = (Shape*)malloc(sizeof(Shape));
    Rectangle* r = (Rectangle*)malloc(sizeof(Rectangle));
    r->width = width;
    r->height = height;
    s->data = r;
    s->area = rectangle_area;
    s->perimeter = rectangle_perimeter;
    s->print = rectangle_print;
    return s;
}

// Destroy shape
void destroy_shape(Shape* s) {
    free(s->data);
    free(s);
}

// Event handler system
typedef void (*EventHandler)(const char*);

typedef struct {
    EventHandler handlers[10];
    int count;
} EventSystem;

void event_system_init(EventSystem* es) {
    es->count = 0;
}

void event_system_register(EventSystem* es, EventHandler handler) {
    if (es->count < 10) {
        es->handlers[es->count++] = handler;
    }
}

void event_system_trigger(EventSystem* es, const char* event) {
    for (int i = 0; i < es->count; i++) {
        es->handlers[i](event);
    }
}

// Event handlers
void log_handler(const char* event) {
    printf("[LOG] Event: %s\n", event);
}

void alert_handler(const char* event) {
    printf("[ALERT] Event: %s\n", event);
}

// Filter function with predicate
int filter(int* input, int* output, int size, int (*predicate)(int)) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (predicate(input[i])) {
            output[count++] = input[i];
        }
    }
    return count;
}

// Predicates
int is_even(int n) { return n % 2 == 0; }
int is_positive(int n) { return n > 0; }

int main() {
    printf("=== Function Pointers and Callbacks Test ===\n");
    
    // 1. Function pointer operations
    printf("\n1. Function pointer arithmetic:\n");
    printf("add(10, 5) = %d\n", apply_operation(10, 5, add));
    printf("subtract(10, 5) = %d\n", apply_operation(10, 5, subtract));
    printf("multiply(10, 5) = %d\n", apply_operation(10, 5, multiply));
    printf("divide(10, 5) = %d\n", apply_operation(10, 5, divide));
    
    // 2. Callbacks
    printf("\n2. Callbacks with for_each:\n");
    int arr[] = {1, 2, 3, 4, 5};
    printf("Numbers: ");
    for_each(arr, 5, print_number);
    printf("\nSquares: ");
    for_each(arr, 5, print_square);
    printf("\n");
    
    // 3. Sorting with comparators
    printf("\n3. Sorting with custom comparators:\n");
    int nums[] = {5, 2, 8, 1, 9, 3};
    
    qsort(nums, 6, sizeof(int), compare_ascending);
    printf("Ascending: ");
    for (int i = 0; i < 6; i++) printf("%d ", nums[i]);
    printf("\n");
    
    qsort(nums, 6, sizeof(int), compare_descending);
    printf("Descending: ");
    for (int i = 0; i < 6; i++) printf("%d ", nums[i]);
    printf("\n");
    
    // 4. Function pointer table
    printf("\n4. Calculator with function pointer table:\n");
    printf("10 + 5 = %d\n", calculate('+', 10, 5));
    printf("10 - 5 = %d\n", calculate('-', 10, 5));
    printf("10 * 5 = %d\n", calculate('*', 10, 5));
    printf("10 / 5 = %d\n", calculate('/', 10, 5));
    
    // 5. Virtual table simulation
    printf("\n5. Virtual table (polymorphism):\n");
    Shape* shapes[2];
    shapes[0] = create_circle(5.0);
    shapes[1] = create_rectangle(4.0, 6.0);
    
    for (int i = 0; i < 2; i++) {
        shapes[i]->print(shapes[i]);
        printf("  Area: %.2f\n", shapes[i]->area(shapes[i]));
        printf("  Perimeter: %.2f\n", shapes[i]->perimeter(shapes[i]));
    }
    
    destroy_shape(shapes[0]);
    destroy_shape(shapes[1]);
    
    // 6. Event system
    printf("\n6. Event handler system:\n");
    EventSystem es;
    event_system_init(&es);
    event_system_register(&es, log_handler);
    event_system_register(&es, alert_handler);
    event_system_trigger(&es, "UserLogin");
    event_system_trigger(&es, "DataUpdate");
    
    // 7. Filter with predicates
    printf("\n7. Filter with predicates:\n");
    int data[] = {-5, 2, -3, 8, 0, 9, -1, 4};
    int filtered[8];
    
    int count = filter(data, filtered, 8, is_even);
    printf("Even numbers: ");
    for (int i = 0; i < count; i++) printf("%d ", filtered[i]);
    printf("\n");
    
    count = filter(data, filtered, 8, is_positive);
    printf("Positive numbers: ");
    for (int i = 0; i < count; i++) printf("%d ", filtered[i]);
    printf("\n");
    
    printf("\n=== Function Pointers and Callbacks Test Complete ===\n");
    return 0;
}
