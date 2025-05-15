// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#include <stdio.h>

#include "utils.h"  // NOLINT[build/include_subdir]

// Test basic sleep functionality
static void test_sleep_ms() {
  printf("Running test_sleep_ms...\n");
  sleep_ms(500);
  printf("Slept for 500 ms\n");
}

static void test_get_current_us() {
  printf("Running test_get_current_us...\n");
  int64_t start_time = get_current_us();
  sleep_ms(500);
  int64_t end_time = get_current_us();
  printf("Start time: %ld us\n", start_time);
  printf("End time: %ld us\n", end_time);
  printf("Elapsed time: %ld us\n", end_time - start_time);
}

void test_utils_main() {
    printf("Running test_utils_main...\n");
    test_sleep_ms();
    test_get_current_us();
    printf("test_utils_main completed.\n");
}
