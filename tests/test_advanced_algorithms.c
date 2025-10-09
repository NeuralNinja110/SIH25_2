/**
 * Test Case 7: Advanced Algorithms
 * Tests: Floyd-Warshall, KMP string matching, LRU cache, Trie, Dijkstra
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INF 99999
#define MAX_V 10

// Floyd-Warshall Algorithm for all-pairs shortest path
void floyd_warshall(int graph[][MAX_V], int V) {
    int dist[MAX_V][MAX_V];
    
    // Initialize distance matrix
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            dist[i][j] = graph[i][j];
        }
    }
    
    // Floyd-Warshall
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
    
    // Print result
    printf("Shortest paths between all pairs:\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF)
                printf("%7s", "INF");
            else
                printf("%7d", dist[i][j]);
        }
        printf("\n");
    }
}

// KMP String Matching Algorithm
void compute_lps(const char* pattern, int M, int* lps) {
    int len = 0;
    lps[0] = 0;
    int i = 1;
    
    while (i < M) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

void kmp_search(const char* text, const char* pattern) {
    int M = strlen(pattern);
    int N = strlen(text);
    int* lps = (int*)malloc(M * sizeof(int));
    
    compute_lps(pattern, M, lps);
    
    int i = 0;  // text index
    int j = 0;  // pattern index
    
    printf("KMP Search for '%s' in '%s':\n", pattern, text);
    int found = 0;
    
    while (i < N) {
        if (pattern[j] == text[i]) {
            j++;
            i++;
        }
        
        if (j == M) {
            printf("Pattern found at index %d\n", i - j);
            found = 1;
            j = lps[j - 1];
        } else if (i < N && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    
    if (!found) {
        printf("Pattern not found\n");
    }
    
    free(lps);
}

// LRU Cache Implementation
typedef struct LRUNode {
    int key;
    int value;
    struct LRUNode* prev;
    struct LRUNode* next;
} LRUNode;

typedef struct {
    int capacity;
    int size;
    LRUNode* head;
    LRUNode* tail;
} LRUCache;

LRUCache* lru_create(int capacity) {
    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;
    return cache;
}

void lru_move_to_front(LRUCache* cache, LRUNode* node) {
    if (node == cache->head) return;
    
    // Remove from current position
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    if (node == cache->tail) cache->tail = node->prev;
    
    // Move to front
    node->prev = NULL;
    node->next = cache->head;
    if (cache->head) cache->head->prev = node;
    cache->head = node;
    if (!cache->tail) cache->tail = node;
}

void lru_put(LRUCache* cache, int key, int value) {
    // Check if key exists
    LRUNode* current = cache->head;
    while (current) {
        if (current->key == key) {
            current->value = value;
            lru_move_to_front(cache, current);
            return;
        }
        current = current->next;
    }
    
    // Create new node
    LRUNode* node = (LRUNode*)malloc(sizeof(LRUNode));
    node->key = key;
    node->value = value;
    node->prev = NULL;
    node->next = cache->head;
    
    if (cache->head) cache->head->prev = node;
    cache->head = node;
    if (!cache->tail) cache->tail = node;
    
    cache->size++;
    
    // Evict if necessary
    if (cache->size > cache->capacity) {
        LRUNode* old_tail = cache->tail;
        cache->tail = old_tail->prev;
        if (cache->tail) cache->tail->next = NULL;
        free(old_tail);
        cache->size--;
    }
}

int lru_get(LRUCache* cache, int key) {
    LRUNode* current = cache->head;
    while (current) {
        if (current->key == key) {
            lru_move_to_front(cache, current);
            return current->value;
        }
        current = current->next;
    }
    return -1;  // Not found
}

// Trie Data Structure
#define ALPHABET_SIZE 26

typedef struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    int is_end_of_word;
} TrieNode;

TrieNode* trie_create_node() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->is_end_of_word = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void trie_insert(TrieNode* root, const char* word) {
    TrieNode* current = root;
    while (*word) {
        int index = *word - 'a';
        if (index < 0 || index >= ALPHABET_SIZE) {
            word++;
            continue;
        }
        if (!current->children[index]) {
            current->children[index] = trie_create_node();
        }
        current = current->children[index];
        word++;
    }
    current->is_end_of_word = 1;
}

int trie_search(TrieNode* root, const char* word) {
    TrieNode* current = root;
    while (*word) {
        int index = *word - 'a';
        if (index < 0 || index >= ALPHABET_SIZE || !current->children[index]) {
            return 0;
        }
        current = current->children[index];
        word++;
    }
    return current && current->is_end_of_word;
}

// Dijkstra's Algorithm
int min_distance(int dist[], int visited[], int V) {
    int min = INT_MAX, min_index = 0;
    for (int v = 0; v < V; v++) {
        if (!visited[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(int graph[][MAX_V], int src, int V) {
    int dist[MAX_V];
    int visited[MAX_V];
    
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    
    dist[src] = 0;
    
    for (int count = 0; count < V - 1; count++) {
        int u = min_distance(dist, visited, V);
        visited[u] = 1;
        
        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INT_MAX &&
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
    
    printf("Dijkstra from vertex %d:\n", src);
    for (int i = 0; i < V; i++) {
        printf("Vertex %d: Distance = %d\n", i, dist[i]);
    }
}

int main() {
    printf("=== Advanced Algorithms Test ===\n");
    
    // 1. Floyd-Warshall
    printf("\n1. Floyd-Warshall (All-Pairs Shortest Path):\n");
    int graph1[MAX_V][MAX_V] = {
        {0, 5, INF, 10},
        {INF, 0, 3, INF},
        {INF, INF, 0, 1},
        {INF, INF, INF, 0}
    };
    floyd_warshall(graph1, 4);
    
    // 2. KMP String Matching
    printf("\n2. KMP String Matching:\n");
    kmp_search("ABABDABACDABABCABAB", "ABABCABAB");
    kmp_search("AABAACAADAABAABA", "AABA");
    
    // 3. LRU Cache
    printf("\n3. LRU Cache:\n");
    LRUCache* cache = lru_create(3);
    lru_put(cache, 1, 10);
    lru_put(cache, 2, 20);
    lru_put(cache, 3, 30);
    printf("Get key 2: %d\n", lru_get(cache, 2));
    lru_put(cache, 4, 40);  // Evicts key 1
    printf("Get key 1: %d (should be -1)\n", lru_get(cache, 1));
    printf("Get key 3: %d\n", lru_get(cache, 3));
    
    // 4. Trie
    printf("\n4. Trie Data Structure:\n");
    TrieNode* trie = trie_create_node();
    trie_insert(trie, "hello");
    trie_insert(trie, "world");
    trie_insert(trie, "help");
    printf("Search 'hello': %s\n", trie_search(trie, "hello") ? "Found" : "Not found");
    printf("Search 'help': %s\n", trie_search(trie, "help") ? "Found" : "Not found");
    printf("Search 'hell': %s\n", trie_search(trie, "hell") ? "Found" : "Not found");
    
    // 5. Dijkstra
    printf("\n5. Dijkstra's Algorithm:\n");
    int graph2[MAX_V][MAX_V] = {
        {0, 4, 0, 0, 0, 0, 0, 8, 0},
        {4, 0, 8, 0, 0, 0, 0, 11, 0},
        {0, 8, 0, 7, 0, 4, 0, 0, 2},
        {0, 0, 7, 0, 9, 14, 0, 0, 0},
        {0, 0, 0, 9, 0, 10, 0, 0, 0},
        {0, 0, 4, 14, 10, 0, 2, 0, 0},
        {0, 0, 0, 0, 0, 2, 0, 1, 6},
        {8, 11, 0, 0, 0, 0, 1, 0, 7},
        {0, 0, 2, 0, 0, 0, 6, 7, 0}
    };
    dijkstra(graph2, 0, 9);
    
    printf("\n=== Advanced Algorithms Test Complete ===\n");
    return 0;
}
