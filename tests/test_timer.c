// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#include <stdio.h>
#include "timer.h"  // NOLINT[build/include_subdir]
#include "utils.h"  // NOLINT[build/include_subdir]

// Test basic timer functionality: start, wait, stop, and print stats
static void test_timer_basic() {
  printf("Running test_timer_basic...\n");
  Timer timer;
  start_recording(&timer);
  sleep_ms(500);
  stop_recording(&timer);
  print_human_readable_stats(&timer, 1);
}

// Test timer with multiple inferences (simulate 4 inferences in 1.2s)
static void test_timer_multiple_inferences() {
  printf("Running test_timer_multiple_inferences...\n");
  Timer timer;
  start_recording(&timer);
  sleep_ms(1200);
  stop_recording(&timer);
  print_human_readable_stats(&timer, 4);
}

// Test timer stats output without stopping the timer
static void test_timer_without_stop() {
  printf("Running test_timer_without_stop...\n");
  Timer timer;
  start_recording(&timer);
  print_human_readable_stats(&timer, 1);
}

// Test fps rate calculation
static void test_timer_fps_rate() {
  printf("Running test_timer_fps_rate...\n");
  Timer timer;
  start_recording(&timer);
  sleep_ms(1000);  // Simulate 1 second of processing
  stop_recording(&timer);
  float fps_rate = get_fps_rate(&timer, 10);  // Simulate 10 inferences
  printf("FPS Rate: %f\n", fps_rate);
}

void test_timer_main() {
  test_timer_basic();
  test_timer_multiple_inferences();
  test_timer_without_stop();
  test_timer_fps_rate();
}
