// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.
#ifdef _WIN32

#include <windows.h>

// Loads a dynamic library. Returns a handle, or NULL on failure.
void *load_dynamic_library(const char *path) {
  return LoadLibraryA(path);
}

// Gets the address of a symbol from a library handle. Returns pointer, or NULL
// on failure.
void *get_symbol_address(void *handle, const char *symbol) {
  return GetProcAddress((HMODULE)handle, symbol);
}

// Closes a dynamic library handle.
void close_dynamic_library(void *handle) {
  if (handle != NULL) FreeLibrary((HMODULE)handle);
}

#endif  // _WIN32
