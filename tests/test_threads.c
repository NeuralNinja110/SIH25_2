/**
 * Test Case 4: Multi-threading with POSIX threads
 * Tests: pthread_create, mutexes, condition variables, thread synchronization
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define NUM_THREADS 5
#define BUFFER_SIZE 10

// Shared counter with mutex protection
typedef struct {
    int value;
    pthread_mutex_t lock;
} Counter;

// Producer-Consumer buffer
typedef struct {
    int buffer[BUFFER_SIZE];
    int count;
    int in;
    int out;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} BoundedBuffer;

Counter shared_counter;
BoundedBuffer bb;
int thread_results[NUM_THREADS];

// Initialize counter
void counter_init(Counter* c) {
    c->value = 0;
    pthread_mutex_init(&c->lock, NULL);
}

// Increment counter safely
void counter_increment(Counter* c) {
    pthread_mutex_lock(&c->lock);
    c->value++;
    pthread_mutex_unlock(&c->lock);
}

// Get counter value safely
int counter_get(Counter* c) {
    pthread_mutex_lock(&c->lock);
    int val = c->value;
    pthread_mutex_unlock(&c->lock);
    return val;
}

// Initialize bounded buffer
void bb_init(BoundedBuffer* b) {
    b->count = 0;
    b->in = 0;
    b->out = 0;
    pthread_mutex_init(&b->mutex, NULL);
    pthread_cond_init(&b->not_empty, NULL);
    pthread_cond_init(&b->not_full, NULL);
}

// Producer: add item to buffer
void bb_produce(BoundedBuffer* b, int item) {
    pthread_mutex_lock(&b->mutex);
    
    // Wait while buffer is full
    while (b->count == BUFFER_SIZE) {
        pthread_cond_wait(&b->not_full, &b->mutex);
    }
    
    // Add item
    b->buffer[b->in] = item;
    b->in = (b->in + 1) % BUFFER_SIZE;
    b->count++;
    
    // Signal that buffer is not empty
    pthread_cond_signal(&b->not_empty);
    pthread_mutex_unlock(&b->mutex);
}

// Consumer: remove item from buffer
int bb_consume(BoundedBuffer* b) {
    pthread_mutex_lock(&b->mutex);
    
    // Wait while buffer is empty
    while (b->count == 0) {
        pthread_cond_wait(&b->not_empty, &b->mutex);
    }
    
    // Remove item
    int item = b->buffer[b->out];
    b->out = (b->out + 1) % BUFFER_SIZE;
    b->count--;
    
    // Signal that buffer is not full
    pthread_cond_signal(&b->not_full);
    pthread_mutex_unlock(&b->mutex);
    
    return item;
}

// Thread function: increment counter
void* thread_increment(void* arg) {
    long tid = (long)arg;
    
    for (int i = 0; i < 1000; i++) {
        counter_increment(&shared_counter);
    }
    
    thread_results[tid] = 1;
    return NULL;
}

// Producer thread
void* producer_thread(void* arg) {
    long tid = (long)arg;
    
    for (int i = 0; i < 5; i++) {
        int item = tid * 100 + i;
        bb_produce(&bb, item);
        printf("Producer %ld produced: %d\n", tid, item);
        usleep(100000);  // 100ms
    }
    
    return NULL;
}

// Consumer thread
void* consumer_thread(void* arg) {
    long tid = (long)arg;
    
    for (int i = 0; i < 5; i++) {
        int item = bb_consume(&bb);
        printf("Consumer %ld consumed: %d\n", tid, item);
        usleep(150000);  // 150ms
    }
    
    return NULL;
}

// Compute factorial (for thread workload)
unsigned long long factorial(int n) {
    if (n <= 1) return 1;
    unsigned long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

// Thread function: compute factorial
void* thread_factorial(void* arg) {
    long n = (long)arg;
    unsigned long long result = factorial(n);
    printf("Thread: factorial(%ld) = %llu\n", n, result);
    return (void*)result;
}

int main() {
    printf("=== Multi-threading Test ===\n");
    
    // 1. Basic thread creation
    printf("\n1. Basic thread creation:\n");
    pthread_t threads[NUM_THREADS];
    counter_init(&shared_counter);
    memset(thread_results, 0, sizeof(thread_results));
    
    // Create threads
    for (long i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, thread_increment, (void*)i) != 0) {
            perror("pthread_create");
            return 1;
        }
    }
    
    // Join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    int final_count = counter_get(&shared_counter);
    printf("Expected counter: %d\n", NUM_THREADS * 1000);
    printf("Final counter:    %d\n", final_count);
    printf("Test %s\n", (final_count == NUM_THREADS * 1000) ? "PASSED" : "FAILED");
    
    // 2. Producer-Consumer pattern
    printf("\n2. Producer-Consumer with bounded buffer:\n");
    bb_init(&bb);
    
    pthread_t producers[2];
    pthread_t consumers[2];
    
    // Create producers
    for (long i = 0; i < 2; i++) {
        pthread_create(&producers[i], NULL, producer_thread, (void*)i);
    }
    
    // Create consumers
    for (long i = 0; i < 2; i++) {
        pthread_create(&consumers[i], NULL, consumer_thread, (void*)i);
    }
    
    // Wait for all threads
    for (int i = 0; i < 2; i++) {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }
    
    printf("Producer-Consumer test completed\n");
    
    // 3. Thread with return value
    printf("\n3. Thread return values:\n");
    pthread_t factorial_threads[5];
    
    for (long i = 0; i < 5; i++) {
        pthread_create(&factorial_threads[i], NULL, thread_factorial, (void*)(i + 5));
    }
    
    for (int i = 0; i < 5; i++) {
        void* retval;
        pthread_join(factorial_threads[i], &retval);
    }
    
    // Cleanup
    pthread_mutex_destroy(&shared_counter.lock);
    pthread_mutex_destroy(&bb.mutex);
    pthread_cond_destroy(&bb.not_empty);
    pthread_cond_destroy(&bb.not_full);
    
    printf("\n=== Multi-threading Test Complete ===\n");
    return 0;
}
