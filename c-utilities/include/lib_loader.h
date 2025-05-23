// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifndef C_UTILITIES_INCLUDE_LIB_LOADER_H_
#define C_UTILITIES_INCLUDE_LIB_LOADER_H_

/**
 * @brief Loads a dynamic library.
 * @param [in] path Path to the dynamic library
 * @return Pointer to the loaded library handle, or NULL on failure.
 */
void *load_dynamic_library(const char *path);

/**
 * @brief Gets the address of a symbol from a library handle.
 * @param [in] handle Pointer to the library handle
 * @param [in] symbol Name of the symbol to retrieve
 * @return Pointer to the symbol address, or NULL on failure.
 */
void *get_symbol_address(void *handle, const char *symbol);

/**
 * @brief Closes a dynamic library handle.
 * @param [in] handle Pointer to the library handle
 */
void close_dynamic_library(void *handle);

#endif  // C_UTILITIES_INCLUDE_LIB_LOADER_H_

