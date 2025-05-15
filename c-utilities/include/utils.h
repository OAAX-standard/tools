// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifndef C_UTILITIES_INCLUDE_UTILS_H_
#define C_UTILITIES_INCLUDE_UTILS_H_

#include <stdint.h>

/**
 * @brief Sleep for a specified number of milliseconds.
 * 
 * @param ms The number of milliseconds to sleep.
 */
void sleep_ms(int64_t ms);


/**
 * @brief Get the current time in microseconds since the epoch.
 * 
 * @return The current time in microseconds.
 */
int64_t get_current_us();

#endif  // C_UTILITIES_INCLUDE_UTILS_H_
