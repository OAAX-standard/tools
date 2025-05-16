// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifdef _WIN32

#include "memory.h"  // NOLINT(build/include_subdir)

#include <math.h>
#include <psapi.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

const char *human_memory_size(uint64_t bytes) {
  static char result[20];
  const char *sizeNames[] = {"B", "KB", "MB", "GB", "TB"};

  if (bytes == 0) {
    snprintf(result, sizeof(result), "0 B");
    return result;
  }

  int64_t i = (int64_t)floor(log((double)bytes) / log(1024.0));
  if (i >= 4) i = 4;
  double humanSize = bytes / pow(1024.0, i);
  snprintf(result, sizeof(result), "%.2f %s", humanSize, sizeNames[i]);

  return result;
}

void print_memory_usage(const char *name) {
  PROCESS_MEMORY_COUNTERS_EX pmc;
  HANDLE hProcess = GetCurrentProcess();
  if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS *)&pmc,
                           sizeof(pmc))) {
    printf(
        "\n\n----------------------------------------------------------------"
        "\n");
    printf("Memory usage at %s\n", name);
    printf("Working Set Size: %s\n",
           human_memory_size((uint64_t)pmc.WorkingSetSize));
    printf("Peak Working Set Size: %s\n",
           human_memory_size((uint64_t)pmc.PeakWorkingSetSize));
    printf("Private Bytes: %s\n",
           human_memory_size((uint64_t)pmc.PrivateUsage));
    printf(
        "-----------------------------------------------------------------"
        "\n\n");
  } else {
    printf("Failed to get process memory info.\n");
  }
  CloseHandle(hProcess);
}

#endif  // _WIN32
