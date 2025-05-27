// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifdef _WIN32

#include "logger.h"  // NOLINT(build/include_subdir)

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdint.h>

Logger *create_logger(const char *prefix, const char *filename,
                      LogLevel file_level, LogLevel console_level) {
  Logger *logger = (Logger *)malloc(sizeof(Logger));
  if (logger == NULL) {
    perror("Failed to allocate memory for logger");
    exit(EXIT_FAILURE);
  }

  logger->prefix = _strdup(prefix);
  logger->filename = _strdup(filename);
  logger->file_index = 0;
  logger->file_level = file_level;
  logger->console_level = console_level;

  char log_filename[1024];
  snprintf(log_filename, sizeof(log_filename), "%s.%d", logger->filename,
           logger->file_index);
  FILE *log_file = NULL;
  if (fopen_s(&log_file, log_filename, "a") != 0) {
    perror("Failed to open log file");
    free(logger->filename);
    free(logger->prefix);
    free(logger);
    exit(EXIT_FAILURE);
  }
  logger->log_file = log_file;

  return logger;
}

void close_logger(Logger *logger) {
  if (logger->log_file != NULL) {
    fclose(logger->log_file);
  }
  free(logger->filename);
  free(logger->prefix);
  free(logger);
}

void rotate_log_file(Logger *logger) {
  fclose(logger->log_file);
  logger->file_index++;

  char log_filename[256];
  snprintf(log_filename, sizeof(log_filename), "%s_%d.log", logger->filename,
           logger->file_index);
  FILE *log_file = NULL;
  if (fopen_s(&log_file, log_filename, "a") != 0) {
    perror("Failed to open new log file");
    exit(EXIT_FAILURE);
  }
  logger->log_file = log_file;
}

void log_message(Logger *logger, LogLevel level, const char *format, ...) {
  if (level < logger->file_level && level < logger->console_level) {
    return;
  }

  int64_t file_size = _ftelli64(logger->log_file);
  while (file_size >= MAX_LOG_FILE_SIZE) {
    rotate_log_file(logger);
    file_size = _ftelli64(logger->log_file);
  }
  _fseeki64(logger->log_file, 0, SEEK_END);

  const char *level_str;
  switch (level) {
    case LOG_DEBUG:
      level_str = "DEBUG";
      break;
    case LOG_INFO:
      level_str = "INFO";
      break;
    case LOG_WARNING:
      level_str = "WARNING";
      break;
    case LOG_ERROR:
      level_str = "ERROR";
      break;
    default:
      level_str = "UNKNOWN";
      break;
  }

  SYSTEMTIME st;
  GetLocalTime(&st);
  char finalTimeStr[40];
  snprintf(finalTimeStr, sizeof(finalTimeStr),
           "%04d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay,
           st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

  va_list args;
  va_start(args, format);
  char message[1024];
  vsnprintf(message, sizeof(message), format, args);
  if (level >= logger->file_level && logger->log_file != NULL) {
    fprintf(logger->log_file, "%s - %s (%s): %s\n", finalTimeStr,
            logger->prefix, level_str, message);
  }
  if (level >= logger->console_level) {
    printf("%s - %s (%s): %s\n", finalTimeStr, logger->prefix, level_str,
           message);
  }
  va_end(args);
}

#endif  // _WIN32
