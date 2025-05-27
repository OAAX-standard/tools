// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifndef C_UTILITIES_INCLUDE_SYSINFO_H_
#define C_UTILITIES_INCLUDE_SYSINFO_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SystemInfo {
  char cpu_name[512];
  int32_t cpu_cores;
  int32_t logical_processors;
  int32_t cpu_clock_mhz;
  int32_t hyperthreading_supported;
  char instruction_sets[2048];
  int64_t total_ram_mb;
  char os_name[512];
  char architecture[512];
  int32_t is_virtual_machine;
} SystemInfo;

typedef struct RealTimeSystemInfo {
  float cpu_usage_percent;
  int64_t used_ram_mb;
  float ram_usage_percent;
} RealTimeSystemInfo;

void get_system_info(SystemInfo *info);
void get_real_time_system_info(RealTimeSystemInfo *info);

#ifdef __cplusplus
}
#endif

#endif  // C_UTILITIES_INCLUDE_SYSINFO_H_
