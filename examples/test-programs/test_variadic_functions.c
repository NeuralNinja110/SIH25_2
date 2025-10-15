/**
 * Test Case 5: Variadic Functions
 * Tests: va_list, va_start, va_arg, va_end, variable arguments
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// Sum variable number of integers
int sum(int count, ...) {
    va_list args;
    va_start(args, count);
    
    int total = 0;
    for (int i = 0; i < count; i++) {
        total += va_arg(args, int);
    }
    
    va_end(args);
    return total;
}

// Find maximum of variable number of doubles
double max_double(int count, ...) {
    va_list args;
    va_start(args, count);
    
    double maximum = va_arg(args, double);
    for (int i = 1; i < count; i++) {
        double val = va_arg(args, double);
        if (val > maximum) {
            maximum = val;
        }
    }
    
    va_end(args);
    return maximum;
}

// Custom printf-like function
void my_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'd': {
                    int val = va_arg(args, int);
                    printf("%d", val);
                    break;
                }
                case 'f': {
                    double val = va_arg(args, double);
                    printf("%f", val);
                    break;
                }
                case 's': {
                    char* val = va_arg(args, char*);
                    printf("%s", val);
                    break;
                }
                case 'c': {
                    int val = va_arg(args, int);  // char promoted to int
                    printf("%c", val);
                    break;
                }
                default:
                    printf("%c", *format);
            }
        } else {
            printf("%c", *format);
        }
        format++;
    }
    
    va_end(args);
}

// Concatenate variable number of strings
char* concat_strings(int count, ...) {
    va_list args;
    va_start(args, count);
    
    // Calculate total length
    size_t total_len = 0;
    va_list args_copy;
    va_copy(args_copy, args);
    
    for (int i = 0; i < count; i++) {
        const char* str = va_arg(args_copy, const char*);
        total_len += strlen(str);
    }
    va_end(args_copy);
    
    // Allocate memory
    char* result = (char*)malloc(total_len + 1);
    if (!result) return NULL;
    
    result[0] = '\0';
    
    // Concatenate strings
    for (int i = 0; i < count; i++) {
        const char* str = va_arg(args, const char*);
        strcat(result, str);
    }
    
    va_end(args);
    return result;
}

// Average of variable number of floats
float average(int count, ...) {
    if (count <= 0) return 0.0f;
    
    va_list args;
    va_start(args, count);
    
    float sum = 0.0f;
    for (int i = 0; i < count; i++) {
        sum += (float)va_arg(args, double);  // float promoted to double
    }
    
    va_end(args);
    return sum / count;
}

// Generic logging function
void log_message(const char* level, const char* format, ...) {
    printf("[%s] ", level);
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);  // Use vprintf for va_list
    va_end(args);
    
    printf("\n");
}

// Build array from variable arguments
int* build_array(int count, ...) {
    int* arr = (int*)malloc(count * sizeof(int));
    if (!arr) return NULL;
    
    va_list args;
    va_start(args, count);
    
    for (int i = 0; i < count; i++) {
        arr[i] = va_arg(args, int);
    }
    
    va_end(args);
    return arr;
}

// Multiply all arguments
long long multiply_all(int count, ...) {
    va_list args;
    va_start(args, count);
    
    long long result = 1;
    for (int i = 0; i < count; i++) {
        result *= va_arg(args, int);
    }
    
    va_end(args);
    return result;
}

// Check if all arguments are positive
int all_positive(int count, ...) {
    va_list args;
    va_start(args, count);
    
    for (int i = 0; i < count; i++) {
        if (va_arg(args, int) <= 0) {
            va_end(args);
            return 0;
        }
    }
    
    va_end(args);
    return 1;
}

// Format string with multiple types
void print_formatted(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    printf("Formatted output: ");
    vprintf(fmt, args);
    printf("\n");
    
    va_end(args);
}

int main() {
    printf("=== Variadic Functions Test ===\n");
    
    // 1. Sum function
    printf("\n1. Sum of integers:\n");
    printf("sum(3, 10, 20, 30) = %d\n", sum(3, 10, 20, 30));
    printf("sum(5, 1, 2, 3, 4, 5) = %d\n", sum(5, 1, 2, 3, 4, 5));
    
    // 2. Max function
    printf("\n2. Maximum of doubles:\n");
    printf("max(4, 1.5, 3.7, 2.1, 4.9) = %.2f\n", max_double(4, 1.5, 3.7, 2.1, 4.9));
    
    // 3. Custom printf
    printf("\n3. Custom printf:\n");
    my_printf("Int: %d, Float: %f, String: %s, Char: %c\n", 42, 3.14, "Hello", 'A');
    
    // 4. String concatenation
    printf("\n4. String concatenation:\n");
    char* concat = concat_strings(4, "Hello", " ", "World", "!");
    printf("Concatenated: %s\n", concat);
    free(concat);
    
    // 5. Average function
    printf("\n5. Average of floats:\n");
    printf("average(5, 1.0, 2.0, 3.0, 4.0, 5.0) = %.2f\n", 
           average(5, 1.0, 2.0, 3.0, 4.0, 5.0));
    
    // 6. Logging function
    printf("\n6. Logging function:\n");
    log_message("INFO", "System started successfully");
    log_message("WARNING", "Memory usage: %d%%", 85);
    log_message("ERROR", "Failed to open file: %s", "data.txt");
    
    // 7. Build array
    printf("\n7. Build array from arguments:\n");
    int* arr = build_array(6, 10, 20, 30, 40, 50, 60);
    printf("Array: ");
    for (int i = 0; i < 6; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    free(arr);
    
    // 8. Multiply all
    printf("\n8. Multiply all arguments:\n");
    printf("multiply(4, 2, 3, 4, 5) = %lld\n", multiply_all(4, 2, 3, 4, 5));
    
    // 9. Check all positive
    printf("\n9. Check if all positive:\n");
    printf("all_positive(3, 5, 10, 15) = %s\n", all_positive(3, 5, 10, 15) ? "Yes" : "No");
    printf("all_positive(3, 5, -10, 15) = %s\n", all_positive(3, 5, -10, 15) ? "Yes" : "No");
    
    // 10. Format string
    printf("\n10. Format with vprintf:\n");
    print_formatted("Name: %s, Age: %d, Score: %.2f", "Alice", 25, 95.5);
    
    printf("\n=== Variadic Functions Test Complete ===\n");
    return 0;
}
