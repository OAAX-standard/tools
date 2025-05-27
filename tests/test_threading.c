// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#include <stdio.h>
#include <assert.h>

#include "threading.h"  // NOLINT[build/include_subdir]

// Simple thread function
typedef struct {
  int value;
} ThreadArg;

void *sample_thread_func(void *arg) {
  ThreadArg *threadArg = (ThreadArg *)arg;
  threadArg->value += 1;  // Modify value to show effect
  return NULL;
}

void *heavy_computation(void *arg) {
  int *num = (int *)arg;
  for (int i = 0; i < 1000000; ++i) {
    *num += i;  // Simulate heavy computation
  }
  return NULL;
}

void test_basic_threading() {
  printf("[Test] Starting threading test...\n");
  ThreadHandle handle;
  ThreadArg arg = {42};
  if (thread_create(&handle, sample_thread_func, &arg) == 0) {
    thread_join(&handle);
    // Check if the thread modified the value
    assert(arg.value == 43);
  } else {
    printf("[Test] Failed to create thread!\n");
  }
}

void test_multiple_threads() {
  printf("[Test] Starting multiple threads test...\n");
  ThreadHandle handles[5];
  ThreadArg args[5];

  for (int i = 0; i < 5; ++i) {
    args[i].value = i;
    if (thread_create(&handles[i], heavy_computation, &args[i]) != 0) {
      printf("[Test] Failed to create thread %d!\n", i);
    }
  }
  // Ground-truth values
  int expected_values[] = {1783293664, 1783293665, 1783293666, 1783293667,
                           1783293668};
  for (int i = 0; i < 5; ++i) {
    thread_join(&handles[i]);
    assert(args[i].value == expected_values[i]);
  }
  printf("[Test] All threads finished.\n");
}

void test_threading_main() {
  printf("[Test] Running threading tests...\n");
  test_basic_threading();
  test_multiple_threads();
  printf("[Test] Threading test completed.\n");
}
