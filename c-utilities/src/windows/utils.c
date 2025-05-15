// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifdef _WIN32
#include <stdint.h>
#include <windows.h>  // For Sleep and FILETIME

#include "utils.h"  // NOLINT(build/include_subdir)

void sleep_ms(int64_t ms) {
  if (ms < 0) {
    return;
  }
  Sleep((DWORD)ms);  // Sleep takes milliseconds
}

int64_t get_current_us() {
  FILETIME ft;
  GetSystemTimeAsFileTime(&ft);
  uint64_t time = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
  // FILETIME is in 100-nanosecond intervals since Jan 1, 1601
  // Convert to microseconds since Jan 1, 1601
  return (int64_t)(time / 10);
}

#endif  // _WIN32
