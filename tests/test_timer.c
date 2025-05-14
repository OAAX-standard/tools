#include <stdio.h>
#include "cp_platform.h"
#include "timer.h"

// Test basic timer functionality: start, wait, stop, and print stats
static void test_timer_basic() {
  printf("Running test_timer_basic...\n");
  Timer timer;
  start_recording(&timer);
  cp_sleep_ms(500);
  stop_recording(&timer);
  print_human_readable_stats(&timer, 1);
}

// Test timer with multiple inferences (simulate 4 inferences in 1.2s)
static void test_timer_multiple_inferences() {
  printf("Running test_timer_multiple_inferences...\n");
  Timer timer;
  start_recording(&timer);
  cp_sleep_ms(1200);
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

void test_timer_main() {
  test_timer_basic();
  test_timer_multiple_inferences();
  test_timer_without_stop();
}
