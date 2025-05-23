// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#include <assert.h>
#include <stdio.h>

#include "lib_loader.h"  // NOLINT[build/include_subdir]

// Test basic dynamic library loading and symbol retrieval
void test_lib_loader() {
  const char *libname =
      "./mini_shared_library/libmini.so";  // Adjust this path as needed
  void *handle = load_dynamic_library(libname);
  if (!handle) {
    fprintf(stderr, "Failed to load library: %s\n", libname);
    return;
  }
  // Retrieve the mini_add function
  int (*mini_add)(int, int) =
      (int (*)(int, int))get_symbol_address(handle, "mini_add");
  if (!mini_add) {
    fprintf(stderr, "Failed to find symbol mini_add\n");
    close_dynamic_library(handle);
    return;
  }
  // Call the mini_add function
  int result = mini_add(201, -39);
  printf("mini_add(201, -39) = %d\n", result);
  // Check the result
  assert(result == 201 - 39);  // This should be true
  // Close the library handle
  close_dynamic_library(handle);
  printf("Library closed successfully.\n");
}

// Test accessing a symbol that doesn't exist
void test_lib_loader_symbol_not_found() {
  const char *libname =
      "./mini_shared_library/libmini.so";  // Adjust this path as needed
  void *handle = load_dynamic_library(libname);
  if (!handle) {
    fprintf(stderr, "Failed to load library: %s\n", libname);
    return;
  }
  // Attempt to retrieve a non-existent symbol
  int (*non_existent_func)(int, int) =
      (int (*)(int, int))get_symbol_address(handle, "non_existent_func");
  assert(non_existent_func == NULL);
  printf("Non-existent symbol retrieval failed as expected.\n");
  close_dynamic_library(handle);
}

// Test loading a library that doesn't exist
void test_lib_loader_nonexistent() {
  const char *libname = "./mini_shared_library/libnonexistent.so";
  void *handle = load_dynamic_library(libname);
  assert(handle == NULL);
  printf("Loading nonexistent library failed as expected.\n");
}

void test_lib_loader_main() {
  printf("Running test_lib_loader...\n");
  test_lib_loader();
  test_lib_loader_symbol_not_found();
  test_lib_loader_nonexistent();
  printf("Test completed successfully.\n");
}