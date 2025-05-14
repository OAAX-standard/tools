#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>

#include "queue.h"
#include "tensors_struct.h"
#include "cp_platform.h"

// Helper to create a dummy tensors_struct

// Test basic enqueue and dequeue
static void test_queue_basic() {
    printf("Running test_queue_basic...\n");
    Queue* q = new_queue(2, false);
    assert(q);

    tensors_struct* t1 = create_sample_tensors_struct(42);
    tensors_struct* t2 = create_sample_tensors_struct(43);

    assert(enqueue(q, t1) == 0);
    assert(enqueue(q, t2) == 0);

    tensors_struct* out1 = dequeue(q, 0);
    assert(out1 == t1);
    deep_free_tensors_struct(out1);

    tensors_struct* out2 = dequeue(q, 0);
    assert(out2 == t2);
    deep_free_tensors_struct(out2);

    assert(dequeue(q, 10) == NULL); // Should timeout

    free_queue(q);
    printf("test_queue_basic passed.\n");
}

// Test queue capacity and overwrite (oldest dropped)
static void test_queue_overwrite() {
    printf("Running test_queue_overwrite...\n");
    Queue* q = new_queue(2, false);
    assert(q);

    tensors_struct* t1 = create_sample_tensors_struct(1);
    tensors_struct* t2 = create_sample_tensors_struct(2);
    tensors_struct* t3 = create_sample_tensors_struct(3);

    assert(enqueue(q, t1) == 0);
    assert(enqueue(q, t2) == 0);
    assert(enqueue(q, t3) == 0); // t1 should be dropped

    tensors_struct* out1 = dequeue(q, 0);
    assert(out1 == t2);
    assert(compare_two_tensors_structs(out1, t2) == true);
    deep_free_tensors_struct(out1);

    tensors_struct* out2 = dequeue(q, 0);
    assert(out2 == t3);
    assert(compare_two_tensors_structs(out2, t3) == true);
    deep_free_tensors_struct(out2);

    free_queue(q);
    printf("test_queue_overwrite passed.\n");
}

// Test shutdown behavior
static void test_queue_shutdown() {
    printf("Running test_queue_shutdown...\n");
    Queue* q = new_queue(2, false);
    assert(q);

    tensors_struct* t1 = create_sample_tensors_struct(1);
    assert(enqueue(q, t1) == 0);

    shutdown_queue(q);

    // After shutdown, enqueue should fail
    tensors_struct* t2 = create_sample_tensors_struct(2);
    assert(enqueue(q, t2) != 0);
    deep_free_tensors_struct(t2);

    tensors_struct* out1 = dequeue(q, 0);
    assert(out1 == t1);
    deep_free_tensors_struct(out1);

    // After shutdown and empty, dequeue returns NULL
    assert(dequeue(q, 0) == NULL);

    free_queue(q);
    printf("test_queue_shutdown passed.\n");
}

// Threaded test: producer/consumer
typedef struct {
    Queue* q;
    int count;
} thread_arg_t;

static void* producer_thread(void* arg) {
    thread_arg_t* ta = (thread_arg_t*)arg;
    for (int i = 0; i < ta->count; ++i) {
        printf("Producer producing %d...\n", i);
        tensors_struct* t = create_sample_tensors_struct(i);
        assert(enqueue(ta->q, t) == 0);
        cp_sleep_ms(i+1);
    }
    return NULL;
}

static void* consumer_thread(void* arg) {
    thread_arg_t* ta = (thread_arg_t*)arg;
    int received = 0;
    while (received < ta->count) {
        tensors_struct* t = dequeue(ta->q, 100);
        printf("Consumer consuming %d...\n", received);

        if (t) {
            tensors_struct* generated = create_sample_tensors_struct(received);
            assert(compare_two_tensors_structs(t, generated) == true);
            deep_free_tensors_struct(t);
            deep_free_tensors_struct(generated);
            ++received;
        }
        if(10 - received > 0) 
            cp_sleep_ms(10 - received);
    }
    return NULL;
}

static void test_queue_threaded() {
    printf("Running test_queue_threaded...\n");
    Queue* q = new_queue(20, true);
    assert(q);

    thread_arg_t arg;
    arg.q = q;
    arg.count = 20;

    pthread_t prod, cons;
    pthread_create(&prod, NULL, producer_thread, &arg);
    pthread_create(&cons, NULL, consumer_thread, &arg);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    free_queue(q);
    printf("test_queue_threaded passed.\n");
}

void test_queue_main() {
    printf("==== Testing queue ====\n");
    test_queue_basic();
    test_queue_overwrite();
    test_queue_shutdown();
    test_queue_threaded();
    printf("All queue tests passed!\n");
}
