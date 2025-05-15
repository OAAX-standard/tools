// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#include <stdio.h>
#include "logger.h"  // NOLINT[build/include_subdir]

// Test basic logger functionality: log at different levels
static void test_logger_basic() {
  printf("Running test_logger_basic...\n");
  Logger *logger = create_logger("", "test_log_basic", LOG_INFO, LOG_INFO);
  log_message(logger, LOG_INFO, "This is an info message.");
  log_message(logger, LOG_WARNING, "This is a warning.");
  log_message(logger, LOG_ERROR, "This is an error.");
  close_logger(logger);
}

// Test logger with different log levels for file and console
static void test_logger_levels() {
  printf("Running test_logger_levels...\n");
  Logger *logger = create_logger("", "test_log_levels", LOG_WARNING, LOG_ERROR);
  log_message(logger, LOG_DEBUG, "This debug message should not appear.");
  log_message(logger, LOG_INFO, "This info message should not appear.");
  log_message(logger, LOG_WARNING, "This warning should appear in file only.");
  log_message(logger, LOG_ERROR,
              "This error should appear in file and console.");
  close_logger(logger);
}

// Test logger file rotation by writing many log messages
static void test_logger_rotation() {
  printf("Running test_logger_rotation...\n");
  Logger *logger = create_logger("", "test_log_rotate", LOG_DEBUG, LOG_ERROR);
  for (int i = 0; i < 10000; ++i) {
    log_message(logger, LOG_INFO,
      "Log entry %d: This is a test for rotation.", i);
  }
  close_logger(logger);
}

void test_logger_main() {
  test_logger_basic();
  test_logger_levels();
  test_logger_rotation();
}
