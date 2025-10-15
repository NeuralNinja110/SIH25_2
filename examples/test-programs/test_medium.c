/**
 * Test Case 2: Medium - Data Structures and Functions
 * Tests obfuscation with multiple functions, structures, and algorithms
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    float score;
} Student;

// Function prototypes
int fibonacci(int n);
void bubbleSort(int arr[], int n);
void printArray(int arr[], int n);
Student* createStudent(int id, const char* name, float score);

int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

Student* createStudent(int id, const char* name, float score) {
    Student* s = (Student*)malloc(sizeof(Student));
    if (s != NULL) {
        s->id = id;
        strncpy(s->name, name, 49);
        s->name[49] = '\0';
        s->score = score;
    }
    return s;
}

int main() {
    printf("=== Medium Complexity Test ===\n\n");
    
    // Test Fibonacci
    printf("Fibonacci sequence:\n");
    for (int i = 0; i < 10; i++) {
        printf("F(%d) = %d\n", i, fibonacci(i));
    }
    
    // Test sorting
    printf("\nSorting test:\n");
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("Original array: ");
    printArray(arr, n);
    
    bubbleSort(arr, n);
    
    printf("Sorted array: ");
    printArray(arr, n);
    
    // Test structures
    printf("\nStudent records:\n");
    Student* students[3];
    students[0] = createStudent(1, "Alice", 95.5);
    students[1] = createStudent(2, "Bob", 87.3);
    students[2] = createStudent(3, "Charlie", 92.1);
    
    for (int i = 0; i < 3; i++) {
        if (students[i] != NULL) {
            printf("ID: %d, Name: %s, Score: %.1f\n", 
                   students[i]->id, students[i]->name, students[i]->score);
            free(students[i]);
        }
    }
    
    // Complex control flow
    int x = 42;
    int y = 17;
    
    if (x > y) {
        printf("\nConditional test: x (%d) > y (%d)\n", x, y);
        
        switch (x % 3) {
            case 0:
                printf("x is divisible by 3\n");
                break;
            case 1:
                printf("x mod 3 = 1\n");
                break;
            case 2:
                printf("x mod 3 = 2\n");
                break;
        }
    }
    
    return 0;
}
