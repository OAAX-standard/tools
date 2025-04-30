// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#include "memory.h"  // NOLINT(build/include_subdir)

#include <stdio.h>

#include "cp_platform.h"  // NOLINT(build/include_subdir)

const char *human_memory_size(uint64_t bytes) {
  static char result[20];
  const char *sizeNames[] = {"B", "KB", "MB", "GB", "TB"};

  if (bytes == 0) {
    snprintf(result, sizeof(result), "0 B");
    return result;
  }

  int64_t i = (int64_t)floor(log(bytes) / log(1024));
  if (i >= 4) i = 4;
  double humanSize = bytes / pow(1024, i);
  snprintf(result, sizeof(result), "%.2f %s", humanSize, sizeNames[i]);

  return result;
}

void print_memory_usage(const char *name) {
  uint64_t total_allocated = 0;
  uint64_t total_free = 0;
  uint64_t total_releasable = 0;

  cp_get_memory_usage(&total_allocated, &total_free,
                      &total_releasable);  // Use cross-platform function

  printf(
      "\n\n----------------------------------------------------------------\n");
  printf("Memory usage at %s\n", name);
  printf("Total allocated space: %s\n", human_memory_size(total_allocated));
  printf("Total free space: %s\n", human_memory_size(total_free));
  printf("Total releasable space: %s\n", human_memory_size(total_releasable));
  printf(
      "-----------------------------------------------------------------\n\n");
}
