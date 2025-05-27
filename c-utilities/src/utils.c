// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifndef _WIN32
#include "utils.h"  // NOLINT(build/include_subdir)

#include <sys/time.h>  // For struct timeval and gettimeofday
#include <unistd.h>    // For usleep

void sleep_ms(int64_t ms) {
  if (ms < 0) {
    return;  // Negative sleep time is ignored
  }
  usleep(ms * 1000);  // Convert milliseconds to microseconds
}

int64_t get_current_us() {
  struct timeval time;
  gettimeofday(&time, NULL);
  int64_t total_microseconds = time.tv_sec * 1000000 + time.tv_usec;
  return total_microseconds;
}

#endif  // _WIN32
