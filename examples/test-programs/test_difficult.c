/**
 * Test Case 3: Difficult - Complex Algorithms and Nested Structures
 * Tests advanced obfuscation with recursive algorithms, dynamic memory,
 * and complex control flow
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Binary tree node
typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// Graph structures
typedef struct Graph {
    int vertices;
    int** adjMatrix;
} Graph;

// Function prototypes
TreeNode* createNode(int data);
TreeNode* insertBST(TreeNode* root, int data);
void inorderTraversal(TreeNode* root);
int treeHeight(TreeNode* root);
void freeTree(TreeNode* root);

Graph* createGraph(int vertices);
void addEdge(Graph* g, int src, int dest);
void DFS(Graph* g, int vertex, int* visited);
void freeGraph(Graph* g);

int binarySearch(int arr[], int left, int right, int target);
void quickSort(int arr[], int low, int high);
int partition(int arr[], int low, int high);

double complexCalculation(int n);
int isPrime(int n);
void generatePrimes(int limit);

// Tree operations
TreeNode* createNode(int data) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node != NULL) {
        node->data = data;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

TreeNode* insertBST(TreeNode* root, int data) {
    if (root == NULL) {
        return createNode(data);
    }
    
    if (data < root->data) {
        root->left = insertBST(root->left, data);
    } else if (data > root->data) {
        root->right = insertBST(root->right, data);
    }
    
    return root;
}

void inorderTraversal(TreeNode* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("%d ", root->data);
        inorderTraversal(root->right);
    }
}

int treeHeight(TreeNode* root) {
    if (root == NULL) {
        return 0;
    }
    
    int leftHeight = treeHeight(root->left);
    int rightHeight = treeHeight(root->right);
    
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

void freeTree(TreeNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// Graph operations
Graph* createGraph(int vertices) {
    Graph* g = (Graph*)malloc(sizeof(Graph));
    g->vertices = vertices;
    
    g->adjMatrix = (int**)malloc(vertices * sizeof(int*));
    for (int i = 0; i < vertices; i++) {
        g->adjMatrix[i] = (int*)calloc(vertices, sizeof(int));
    }
    
    return g;
}

void addEdge(Graph* g, int src, int dest) {
    if (src < g->vertices && dest < g->vertices) {
        g->adjMatrix[src][dest] = 1;
        g->adjMatrix[dest][src] = 1;
    }
}

void DFS(Graph* g, int vertex, int* visited) {
    visited[vertex] = 1;
    printf("%d ", vertex);
    
    for (int i = 0; i < g->vertices; i++) {
        if (g->adjMatrix[vertex][i] == 1 && !visited[i]) {
            DFS(g, i, visited);
        }
    }
}

void freeGraph(Graph* g) {
    for (int i = 0; i < g->vertices; i++) {
        free(g->adjMatrix[i]);
    }
    free(g->adjMatrix);
    free(g);
}

// Searching and sorting
int binarySearch(int arr[], int left, int right, int target) {
    if (right >= left) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        }
        
        if (arr[mid] > target) {
            return binarySearch(arr, left, mid - 1, target);
        }
        
        return binarySearch(arr, mid + 1, right, target);
    }
    
    return -1;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Mathematical operations
double complexCalculation(int n) {
    double result = 0.0;
    
    for (int i = 1; i <= n; i++) {
        double term = 0.0;
        for (int j = 1; j <= i; j++) {
            term += 1.0 / (i * j);
        }
        result += sqrt(term);
    }
    
    return result;
}

int isPrime(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return 0;
        }
    }
    
    return 1;
}

void generatePrimes(int limit) {
    printf("Prime numbers up to %d: ", limit);
    for (int i = 2; i <= limit; i++) {
        if (isPrime(i)) {
            printf("%d ", i);
        }
    }
    printf("\n");
}

int main() {
    printf("=== Difficult Complexity Test ===\n\n");
    
    // Test 1: Binary Search Tree
    printf("1. Binary Search Tree Operations:\n");
    TreeNode* root = NULL;
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    
    for (int i = 0; i < 7; i++) {
        root = insertBST(root, values[i]);
    }
    
    printf("Inorder traversal: ");
    inorderTraversal(root);
    printf("\n");
    printf("Tree height: %d\n\n", treeHeight(root));
    
    // Test 2: Graph DFS
    printf("2. Graph Depth-First Search:\n");
    Graph* g = createGraph(5);
    addEdge(g, 0, 1);
    addEdge(g, 0, 2);
    addEdge(g, 1, 3);
    addEdge(g, 1, 4);
    addEdge(g, 2, 4);
    
    int* visited = (int*)calloc(g->vertices, sizeof(int));
    printf("DFS traversal starting from vertex 0: ");
    DFS(g, 0, visited);
    printf("\n\n");
    
    // Test 3: QuickSort
    printf("3. QuickSort Algorithm:\n");
    int arr[] = {64, 34, 25, 12, 22, 11, 90, 88, 45, 50};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("Original array: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
    
    quickSort(arr, 0, n - 1);
    
    printf("Sorted array: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n\n");
    
    // Test 4: Binary Search
    printf("4. Binary Search:\n");
    int target = 45;
    int result = binarySearch(arr, 0, n - 1, target);
    if (result != -1) {
        printf("Element %d found at index %d\n\n", target, result);
    } else {
        printf("Element %d not found\n\n", target);
    }
    
    // Test 5: Prime Generation
    printf("5. Prime Number Generation:\n");
    generatePrimes(50);
    printf("\n");
    
    // Test 6: Complex Mathematical Calculation
    printf("6. Complex Calculation:\n");
    double calcResult = complexCalculation(10);
    printf("Result of complex calculation(10): %.6f\n\n", calcResult);
    
    // Test 7: Nested loops with complex conditions
    printf("7. Complex Control Flow:\n");
    int matrix[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int sum = 0;
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == j) {
                sum += matrix[i][j];
            } else if (i < j) {
                sum -= matrix[i][j];
            } else {
                sum += matrix[i][j] * 2;
            }
        }
    }
    printf("Matrix operation result: %d\n", sum);
    
    // Cleanup
    freeTree(root);
    freeGraph(g);
    free(visited);
    
    printf("\n=== All tests completed successfully ===\n");
    
    return 0;
}
