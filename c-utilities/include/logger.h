// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifndef C_UTILITIES_INCLUDE_LOGGER_H_
#define C_UTILITIES_INCLUDE_LOGGER_H_

#include <stdio.h>

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
typedef enum { LOG_DEBUG = 0, LOG_INFO, LOG_WARNING, LOG_ERROR } LogLevel;

// Logger structure definition
typedef struct Logger {
  char *prefix;            // Prefix for log messages
  FILE *log_file;          // File pointer for the log file
  char *filename;          // Base filename for log file
  LogLevel file_level;     // Log level for file output
  LogLevel console_level;  // Log level for console output
  int file_index;          // File index for log rotation
} Logger;

// Function to create a new logger
Logger *create_logger(const char *prefix, const char *filename,
                      LogLevel file_level, LogLevel console_level);

// Function to close the logger and free memory
void close_logger(Logger *logger);

// Function to rotate the log file when the size limit is reached
void rotate_log_file(Logger *logger);

// Function to log a message at a specified log level
void log_message(Logger *logger, LogLevel level, const char *format, ...);

#endif  // C_UTILITIES_INCLUDE_LOGGER_H_
