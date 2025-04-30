// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifndef C_UTILITIES_INCLUDE_LOGGER_H_
#define C_UTILITIES_INCLUDE_LOGGER_H_

#include <stdarg.h>  // For va_list, va_start, va_end
#include <stdio.h>
#include <stdlib.h>  // For malloc, free
#include <string.h>  // For snprintf

#include "cp_platform.h"  // NOLINT(build/include_subdir)

// Maximum log file size (4MB)
#define MAX_LOG_FILE_SIZE (4 * 1024 * 1024)
#define log_debug(logger, format, ...) \
  log_message(logger, LOG_DEBUG, format, ##__VA_ARGS__)
#define log_info(logger, format, ...) \
  log_message(logger, LOG_INFO, format, ##__VA_ARGS__)
#define log_warning(logger, format, ...) \
  log_message(logger, LOG_WARNING, format, ##__VA_ARGS__)
#define log_error(logger, format, ...) \
  log_message(logger, LOG_ERROR, format, ##__VA_ARGS__)

// Enum for log levels
typedef enum { LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR } LogLevel;

// Logger structure definition
typedef struct Logger {
  FILE *log_file;
  char *filename;
  LogLevel file_level;
  LogLevel console_level;
  cp_mutex_t lock;  // Use cross-platform mutex
  int file_index;
} Logger;

// Function declarations
Logger *create_logger(const char *filename, LogLevel file_level,
                      LogLevel console_level);
void close_logger(Logger *logger);
void rotate_log_file(Logger *logger);
void log_message(Logger *logger, LogLevel level, const char *format, ...);

#endif  // C_UTILITIES_INCLUDE_LOGGER_H_
