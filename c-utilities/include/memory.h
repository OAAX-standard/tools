// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifndef C_UTILITIES_INCLUDE_MEMORY_H_
#define C_UTILITIES_INCLUDE_MEMORY_H_

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Get a human-readable memory size from the byte count
 * @param [in] bytes Memory size in bytes
 * @return String representing human-readable memory size (e.g., KB, MB, GB)
 */
const char *human_memory_size(uint64_t bytes);

/**
 * @brief Print memory usage information for a given process or resource
 * @param [in] name Name of the resource/process being measured
 */
void print_memory_usage(const char *name);

#endif  // C_UTILITIES_INCLUDE_MEMORY_H_
