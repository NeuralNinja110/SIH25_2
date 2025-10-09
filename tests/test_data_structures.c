/**
 * Test Case 10: Advanced Data Structures
 * Tests: Hash tables, binary search trees, heaps, linked lists
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 100

// Hash Table Implementation
typedef struct HashNode {
    char* key;
    int value;
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode* buckets[HASH_SIZE];
} HashTable;

unsigned int hash(const char* key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) + hash + *key++;
    }
    return hash % HASH_SIZE;
}

HashTable* ht_create() {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    for (int i = 0; i < HASH_SIZE; i++) {
        ht->buckets[i] = NULL;
    }
    return ht;
}

void ht_insert(HashTable* ht, const char* key, int value) {
    unsigned int index = hash(key);
    HashNode* node = ht->buckets[index];
    
    // Check if key exists
    while (node) {
        if (strcmp(node->key, key) == 0) {
            node->value = value;
            return;
        }
        node = node->next;
    }
    
    // Create new node
    node = (HashNode*)malloc(sizeof(HashNode));
    node->key = strdup(key);
    node->value = value;
    node->next = ht->buckets[index];
    ht->buckets[index] = node;
}

int ht_get(HashTable* ht, const char* key) {
    unsigned int index = hash(key);
    HashNode* node = ht->buckets[index];
    
    while (node) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return -1;  // Not found
}

// Binary Search Tree
typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* tree_create_node(int data) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

TreeNode* tree_insert(TreeNode* root, int data) {
    if (!root) return tree_create_node(data);
    
    if (data < root->data) {
        root->left = tree_insert(root->left, data);
    } else if (data > root->data) {
        root->right = tree_insert(root->right, data);
    }
    return root;
}

void tree_inorder(TreeNode* root) {
    if (root) {
        tree_inorder(root->left);
        printf("%d ", root->data);
        tree_inorder(root->right);
    }
}

int tree_height(TreeNode* root) {
    if (!root) return 0;
    int left_height = tree_height(root->left);
    int right_height = tree_height(root->right);
    return 1 + (left_height > right_height ? left_height : right_height);
}

// Min Heap
typedef struct {
    int* arr;
    int size;
    int capacity;
} MinHeap;

MinHeap* heap_create(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->arr = (int*)malloc(capacity * sizeof(int));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void heap_swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heap_heapify_up(MinHeap* heap, int index) {
    if (index == 0) return;
    int parent = (index - 1) / 2;
    if (heap->arr[index] < heap->arr[parent]) {
        heap_swap(&heap->arr[index], &heap->arr[parent]);
        heap_heapify_up(heap, parent);
    }
}

void heap_heapify_down(MinHeap* heap, int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int smallest = index;
    
    if (left < heap->size && heap->arr[left] < heap->arr[smallest]) {
        smallest = left;
    }
    if (right < heap->size && heap->arr[right] < heap->arr[smallest]) {
        smallest = right;
    }
    
    if (smallest != index) {
        heap_swap(&heap->arr[index], &heap->arr[smallest]);
        heap_heapify_down(heap, smallest);
    }
}

void heap_insert(MinHeap* heap, int value) {
    if (heap->size >= heap->capacity) return;
    heap->arr[heap->size] = value;
    heap_heapify_up(heap, heap->size);
    heap->size++;
}

int heap_extract_min(MinHeap* heap) {
    if (heap->size == 0) return -1;
    int min = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;
    heap_heapify_down(heap, 0);
    return min;
}

// Doubly Linked List
typedef struct DListNode {
    int data;
    struct DListNode* prev;
    struct DListNode* next;
} DListNode;

typedef struct {
    DListNode* head;
    DListNode* tail;
    int size;
} DList;

DList* dlist_create() {
    DList* list = (DList*)malloc(sizeof(DList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void dlist_push_back(DList* list, int data) {
    DListNode* node = (DListNode*)malloc(sizeof(DListNode));
    node->data = data;
    node->next = NULL;
    node->prev = list->tail;
    
    if (list->tail) {
        list->tail->next = node;
    } else {
        list->head = node;
    }
    list->tail = node;
    list->size++;
}

void dlist_push_front(DList* list, int data) {
    DListNode* node = (DListNode*)malloc(sizeof(DListNode));
    node->data = data;
    node->prev = NULL;
    node->next = list->head;
    
    if (list->head) {
        list->head->prev = node;
    } else {
        list->tail = node;
    }
    list->head = node;
    list->size++;
}

void dlist_print(DList* list) {
    DListNode* current = list->head;
    while (current) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

// Stack using array
typedef struct {
    int* arr;
    int top;
    int capacity;
} Stack;

Stack* stack_create(int capacity) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->arr = (int*)malloc(capacity * sizeof(int));
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

void stack_push(Stack* stack, int value) {
    if (stack->top < stack->capacity - 1) {
        stack->arr[++stack->top] = value;
    }
}

int stack_pop(Stack* stack) {
    if (stack->top >= 0) {
        return stack->arr[stack->top--];
    }
    return -1;
}

int stack_peek(Stack* stack) {
    if (stack->top >= 0) {
        return stack->arr[stack->top];
    }
    return -1;
}

// Queue using circular array
typedef struct {
    int* arr;
    int front;
    int rear;
    int size;
    int capacity;
} Queue;

Queue* queue_create(int capacity) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->arr = (int*)malloc(capacity * sizeof(int));
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    queue->capacity = capacity;
    return queue;
}

void queue_enqueue(Queue* queue, int value) {
    if (queue->size < queue->capacity) {
        queue->rear = (queue->rear + 1) % queue->capacity;
        queue->arr[queue->rear] = value;
        queue->size++;
    }
}

int queue_dequeue(Queue* queue) {
    if (queue->size > 0) {
        int value = queue->arr[queue->front];
        queue->front = (queue->front + 1) % queue->capacity;
        queue->size--;
        return value;
    }
    return -1;
}

int main() {
    printf("=== Advanced Data Structures Test ===\n");
    
    // 1. Hash Table
    printf("\n1. Hash Table:\n");
    HashTable* ht = ht_create();
    ht_insert(ht, "apple", 100);
    ht_insert(ht, "banana", 200);
    ht_insert(ht, "cherry", 300);
    printf("apple: %d\n", ht_get(ht, "apple"));
    printf("banana: %d\n", ht_get(ht, "banana"));
    printf("cherry: %d\n", ht_get(ht, "cherry"));
    printf("orange: %d (not found)\n", ht_get(ht, "orange"));
    
    // 2. Binary Search Tree
    printf("\n2. Binary Search Tree:\n");
    TreeNode* root = NULL;
    root = tree_insert(root, 50);
    root = tree_insert(root, 30);
    root = tree_insert(root, 70);
    root = tree_insert(root, 20);
    root = tree_insert(root, 40);
    root = tree_insert(root, 60);
    root = tree_insert(root, 80);
    printf("Inorder traversal: ");
    tree_inorder(root);
    printf("\nTree height: %d\n", tree_height(root));
    
    // 3. Min Heap
    printf("\n3. Min Heap:\n");
    MinHeap* heap = heap_create(10);
    heap_insert(heap, 10);
    heap_insert(heap, 5);
    heap_insert(heap, 15);
    heap_insert(heap, 2);
    heap_insert(heap, 8);
    printf("Extract min: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", heap_extract_min(heap));
    }
    printf("\n");
    
    // 4. Doubly Linked List
    printf("\n4. Doubly Linked List:\n");
    DList* list = dlist_create();
    dlist_push_back(list, 10);
    dlist_push_back(list, 20);
    dlist_push_back(list, 30);
    dlist_push_front(list, 5);
    printf("List: ");
    dlist_print(list);
    printf("Size: %d\n", list->size);
    
    // 5. Stack
    printf("\n5. Stack:\n");
    Stack* stack = stack_create(10);
    stack_push(stack, 10);
    stack_push(stack, 20);
    stack_push(stack, 30);
    printf("Pop: %d\n", stack_pop(stack));
    printf("Peek: %d\n", stack_peek(stack));
    printf("Pop: %d\n", stack_pop(stack));
    
    // 6. Queue
    printf("\n6. Circular Queue:\n");
    Queue* queue = queue_create(5);
    queue_enqueue(queue, 10);
    queue_enqueue(queue, 20);
    queue_enqueue(queue, 30);
    printf("Dequeue: %d\n", queue_dequeue(queue));
    queue_enqueue(queue, 40);
    queue_enqueue(queue, 50);
    printf("Dequeue: %d\n", queue_dequeue(queue));
    printf("Dequeue: %d\n", queue_dequeue(queue));
    
    printf("\n=== Advanced Data Structures Test Complete ===\n");
    return 0;
}
