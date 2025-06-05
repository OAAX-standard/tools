// Windows version of timer.c
// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifdef _WIN32
#include "timer.h"  // NOLINT(build/include_subdir)

#include <stdio.h>
#include <windows.h>

#include "utils.h"  // NOLINT(build/include_subdir)

// Helper function to get current time in microseconds

void start_recording(Timer *timer) {
  timer->start = get_current_us();
  timer->end = -1;
  timer->elapsed_time = -1;
}

void stop_recording(Timer *timer) {
  timer->end = get_current_us();
  timer->elapsed_time = timer->end - timer->start;
}

void print_human_readable_stats(const Timer *timer,
                                int64_t number_of_inferences) {
  if (timer->end == -1) {
    printf("Warning: Timer has not been stopped yet.\n");
    return;
  }
  int64_t elapsed_time = timer->elapsed_time;
  int64_t hours = elapsed_time / 3600000000;
  elapsed_time -= hours * 3600000000;
  int64_t minutes = elapsed_time / 60000000;
  elapsed_time -= minutes * 60000000;
  int64_t seconds = elapsed_time / 1000000;
  elapsed_time -= seconds * 1000000;
  int64_t milliseconds = elapsed_time / 1000;
  elapsed_time -= milliseconds * 1000;
  int64_t microseconds = elapsed_time;
  printf(
      "\n\n----------------------------------------------------------------\n");
  printf("Benchmark results:\n");
  printf("Elapsed time: %lldh %lldm %llds %lldms %lldus\n", hours, minutes,
         seconds, milliseconds, microseconds);
  if (number_of_inferences > 0) {
    float avg_latency_ms =
        (float)timer->elapsed_time / number_of_inferences / 1000;
    printf("Average time per inference: %f ms\n", avg_latency_ms);
    printf("Average FPS rate: %f\n", 1000.0f / avg_latency_ms);
  }
  printf(
      "----------------------------------------------------------------\n\n");
}

float get_fps_rate(const Timer *timer, int64_t number_of_inferences) {
  if (timer->elapsed_time <= 0 || number_of_inferences <= 0) {
    return 0.0f;
  }
  float avg_latency_ms =
      (float)timer->elapsed_time / number_of_inferences / 1000;
  return 1000.0f / avg_latency_ms;
}

#endif  // _WIN32
