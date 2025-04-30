// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#include <math.h>
#include <stdio.h>
#include <unistd.h>

#include "cp_platform.h"  // NOLINT(build/include_subdir)
#include "logger.h"       // NOLINT(build/include_subdir)
#include "memory.h"       // NOLINT(build/include_subdir)
#include "timer.h"        // NOLINT(build/include_subdir)

// Test basic timer functionality: start, wait, stop, and print stats
void test_timer_basic() {
  printf("Running test_timer_basic...\n");
  Timer timer;
  start_recording(&timer);
  cp_sleep_ms(500);  // Cross-platform sleep for 0.5 seconds
  stop_recording(&timer);
  print_human_readable_stats(&timer, 1);
}

// Test timer with multiple inferences (simulate 4 inferences in 1.2s)
void test_timer_multiple_inferences() {
  printf("Running test_timer_multiple_inferences...\n");
  Timer timer;
  start_recording(&timer);
  cp_sleep_ms(1200);  // Cross-platform sleep for 1.2 seconds
  stop_recording(&timer);
  print_human_readable_stats(&timer, 4);
}

// Test timer stats output without stopping the timer
void test_timer_without_stop() {
  printf("Running test_timer_without_stop...\n");
  Timer timer;
  start_recording(&timer);
  // Not calling stop_recording
  print_human_readable_stats(&timer, 1);
}

// Test basic logger functionality: log at different levels
void test_logger_basic() {
  printf("Running test_logger_basic...\n");
  Logger *logger = create_logger("test_log_basic", LOG_INFO, LOG_INFO);
  log_message(logger, LOG_INFO, "This is an info message.");
  log_message(logger, LOG_WARNING, "This is a warning.");
  log_message(logger, LOG_ERROR, "This is an error.");
  close_logger(logger);
}

// Test logger with different log levels for file and console
void test_logger_levels() {
  printf("Running test_logger_levels...\n");
  Logger *logger = create_logger("test_log_levels", LOG_WARNING, LOG_ERROR);
  log_message(logger, LOG_DEBUG, "This debug message should not appear.");
  log_message(logger, LOG_INFO, "This info message should not appear.");
  log_message(logger, LOG_WARNING, "This warning should appear in file only.");
  log_message(logger, LOG_ERROR,
              "This error should appear in file and console.");
  close_logger(logger);
}

// Test logger file rotation by writing many log messages
void test_logger_rotation() {
  // TODO(ayoubassis): this fails when number of messages is > 60k
  printf("Running test_logger_rotation...\n");
  Logger *logger = create_logger("test_log_rotate", LOG_DEBUG, LOG_ERROR);
  // Write enough messages to trigger rotation
  for (int i = 0; i < 10000; ++i) {
    log_message(logger, LOG_INFO, "Log entry %d: This is a test for rotation.",
                i);
  }
  close_logger(logger);
}

// Test human-readable memory size formatting
void test_human_memory_size() {
  printf("Running test_human_memory_size...\n");
  uint64_t test_values[] = {0,       512,        1024,
                            1048576, 1073741824, 1099511627776ULL};
  for (int i = 0; i < 6; ++i) {
    printf("%llu bytes -> %s\n", (uint64_t)test_values[i],
           human_memory_size(test_values[i]));
  }
}

// Test printing current memory usage
void test_print_memory_usage() {
  printf("Running test_print_memory_usage...\n");
  print_memory_usage("test_print_memory_usage");
}

// Main function: run all tests
int main() {
  test_logger_basic();
  test_logger_levels();
  test_logger_rotation();
  test_timer_basic();
  test_timer_multiple_inferences();
  test_timer_without_stop();
  test_human_memory_size();
  test_print_memory_usage();
  return 0;
}
