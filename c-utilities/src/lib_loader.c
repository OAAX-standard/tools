// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.
#ifndef _WIN32

#include "lib_loader.h"  // NOLINT[build/include_subdir]

#include <dlfcn.h>
#include <stddef.h>

// Loads a dynamic library. Returns a handle, or NULL on failure.
void *load_dynamic_library(const char *path) {
  return dlopen(path, RTLD_LAZY);
}

// Gets the address of a symbol from a library handle. Returns pointer, or NULL
// on failure.
void *get_symbol_address(void *handle, const char *symbol) {
  return dlsym(handle, symbol);
}

// Closes a dynamic library handle.
void close_dynamic_library(void *handle) {
  if (handle != NULL) dlclose(handle);
}

#endif  // _WIN32
