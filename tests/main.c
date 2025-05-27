// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "logger.h"     // NOLINT[build/include_subdir]
#include "threading.h"  // NOLINT[build/include_subdir]

// Declare test entry points from new files
void test_timer_main();
void test_logger_main();
void test_memory_main();
void test_tensors_struct_main();
void test_queue_main();
void test_utils_main();
void test_threading_main();
void test_lib_loader_main();
void test_sysinfo_main();

Logger *logger;

int main() {
  logger = create_logger("C Utilities", "test.log", LOG_DEBUG, LOG_DEBUG);
  printf("Running tests...\n");
  test_logger_main();
  test_timer_main();
  test_memory_main();
  test_tensors_struct_main();
  test_queue_main();
  test_utils_main();
  test_threading_main();
  test_lib_loader_main();
  test_sysinfo_main();
  printf("All tests passed.\n");
  // Close the logger at the end of the tests
  close_logger(logger);
  return 0;
}
