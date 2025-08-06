// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "threading.h"  // NOLINT[build/include_subdir]

// Declare test entry points from new files
void test_tensors_struct_main();
void test_utils_main();
void test_threading_main();
void test_lib_loader_main();

int main() {
  printf("Running tests...\n");
  test_tensors_struct_main();
  test_utils_main();
  test_threading_main();
  test_lib_loader_main();
  printf("All tests passed.\n");
  return 0;
}
