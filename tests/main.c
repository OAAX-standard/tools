// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "logger.h"  // NOLINT[build/include_subdir]

// Declare test entry points from new files
void test_timer_main();
void test_logger_main();
void test_memory_main();
void test_tensors_struct_main();
void test_queue_main();
void test_utils_main();

Logger *logger;

int main() {
#ifdef _WIN32
  // Set output mode to handle UTF-8 on Windows terminals (optional)
  SetConsoleOutputCP(CP_UTF8);
#endif
  logger = create_logger("C Utilities", "test.log", LOG_DEBUG, LOG_DEBUG);

  test_logger_main();
  test_timer_main();
  test_memory_main();
  test_tensors_struct_main();
  test_queue_main();
  test_utils_main();

  // Close the logger at the end of the tests
  close_logger(logger);
#ifdef _WIN32
  // Pause the console on Windows so the user can see the output
  system("pause");
#endif
  return 0;
}
