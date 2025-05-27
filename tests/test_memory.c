// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#include <stdio.h>
#include <stdint.h>
#include "memory.h"  // NOLINT[build/include_subdir]

// Test human-readable memory size formatting
static void test_human_memory_size() {
  printf("Running test_human_memory_size...\n");
  uint64_t test_values[] = {0, 512, 1024,
    1048576, 1073741824, 1099511627776ULL};
  for (int i = 0; i < 6; ++i) {
    printf("%zu bytes -> %s\n", (uint64_t)test_values[i],
           human_memory_size(test_values[i]));
  }
}

// Test printing current memory usage
static void test_print_memory_usage() {
  printf("Running test_print_memory_usage...\n");
  print_memory_usage("test_print_memory_usage");
}

void test_memory_main() {
  test_human_memory_size();
  test_print_memory_usage();
}
