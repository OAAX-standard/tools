// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifndef _WIN32

#define _GNU_SOURCE

#include "sysinfo.h"  // NOLINT(build/include_subdir)

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <unistd.h>

static void get_cpu_name(char *buf, size_t size) {
  FILE *f = fopen("/proc/cpuinfo", "r");
  if (!f) {
    strncpy(buf, "Unknown", size);
    return;
  }
  char line[256];
  while (fgets(line, sizeof(line), f)) {
    if (strncmp(line, "model name", 10) == 0) {
      char *colon = strchr(line, ':');
      if (colon) {
        strncpy(buf, colon + 2, size);
        buf[size - 1] = '\0';
        size_t len = strlen(buf);
        if (len && buf[len - 1] == '\n') buf[len - 1] = 0;
        fclose(f);
        return;
      }
    }
  }
  fclose(f);
  strncpy(buf, "Unknown", size);
}

static int get_cpu_cores() {
  long n = sysconf(_SC_NPROCESSORS_ONLN);
  return (int)(n > 0 ? n : 1);
}

static int get_physical_cores() {
  int cores = 0;
  FILE *f = fopen("/proc/cpuinfo", "r");
  if (!f) return get_cpu_cores();
  char line[256];
  int last_physical_id = -1, last_core_id = -1;
  int ids[256][2], id_count = 0;
  while (fgets(line, sizeof(line), f)) {
    int physical_id = -1, core_id = -1;
    if (sscanf(line, "physical id\t: %d", &physical_id) == 1)
      last_physical_id = physical_id;
    if (sscanf(line, "core id\t: %d", &core_id) == 1) last_core_id = core_id;
    if (strstr(line, "core id")) {
      int found = 0;
      for (int i = 0; i < id_count; ++i) {
        if (ids[i][0] == last_physical_id && ids[i][1] == last_core_id) {
          found = 1;
          break;
        }
      }
      if (!found && last_physical_id != -1 && last_core_id != -1) {
        ids[id_count][0] = last_physical_id;
        ids[id_count][1] = last_core_id;
        id_count++;
      }
    }
  }
  fclose(f);
  return id_count > 0 ? id_count : get_cpu_cores();
}

static int get_cpu_clock_mhz() {
  FILE *f = fopen("/proc/cpuinfo", "r");
  if (!f) return 0;
  char line[256];
  while (fgets(line, sizeof(line), f)) {
    float mhz;
    if (sscanf(line, "cpu MHz\t: %f", &mhz) == 1) {
      fclose(f);
      return (int)mhz;
    }
  }
  fclose(f);
  return 0;
}

static void detect_instruction_sets(char *buf, size_t size) {
  FILE *f = fopen("/proc/cpuinfo", "r");
  if (!f) {
    strncpy(buf, "Unknown", size);
    return;
  }
  char line[1024 * 10];
  while (fgets(line, sizeof(line), f)) {
    if (strncmp(line, "flags", 5) == 0) {
      char *colon = strchr(line, ':');
      if (colon) {
        strncpy(buf, colon + 2, size);
        buf[size - 1] = '\0';
        size_t len = strlen(buf);
        if (len && buf[len - 1] == '\n') buf[len - 1] = 0;
        fclose(f);
        return;
      }
    }
  }
  fclose(f);
  strncpy(buf, "Unknown", size);
}

static int is_hyperthreading_supported() {
  int logical = get_cpu_cores();
  int physical = get_physical_cores();
  return (logical > physical) ? 1 : 0;
}

static void get_os_name(char *buf, size_t size) {
  FILE *f = fopen("/etc/os-release", "r");
  if (!f) {
    strncpy(buf, "Linux", size);
    return;
  }
  char line[256];
  while (fgets(line, sizeof(line), f)) {
    if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
      char *start = strchr(line, '"');
      char *end = strrchr(line, '"');
      if (start && end && end > start) {
        size_t len = end - start - 1;
        if (len >= size) len = size - 1;
        strncpy(buf, start + 1, len);
        buf[len] = 0;
        fclose(f);
        return;
      }
    }
  }
  fclose(f);
  strncpy(buf, "Linux", size);
}

static void get_architecture(char *buf, size_t size) {
  struct utsname uts;
  if (uname(&uts) == 0) {
    strncpy(buf, uts.machine, size);
    buf[size - 1] = 0;
  } else {
    strncpy(buf, "Unknown", size);
  }
}

static int is_virtual_machine() {
  FILE *f = fopen("/proc/cpuinfo", "r");
  if (!f) return 0;
  char line[256];
  while (fgets(line, sizeof(line), f)) {
    if (strstr(line, "hypervisor")) {
      fclose(f);
      return 1;
    }
  }
  fclose(f);
  return 0;
}

static double calculate_cpu_usage() {
  // Read /proc/stat twice, 100ms apart
  FILE *f;
  unsigned long long user1, nice1, system1, idle1, iowait1, irq1, softirq1,
      steal1;
  unsigned long long user2, nice2, system2, idle2, iowait2, irq2, softirq2,
      steal2;
  f = fopen("/proc/stat", "r");
  if (!f) return 0.0;
  fscanf(f, "cpu  %llu %llu %llu %llu %llu %llu %llu %llu", &user1, &nice1,
         &system1, &idle1, &iowait1, &irq1, &softirq1, &steal1);
  fclose(f);
  usleep(100000);
  f = fopen("/proc/stat", "r");
  if (!f) return 0.0;
  fscanf(f, "cpu  %llu %llu %llu %llu %llu %llu %llu %llu", &user2, &nice2,
         &system2, &idle2, &iowait2, &irq2, &softirq2, &steal2);
  fclose(f);
  unsigned long long idle_diff = (idle2 + iowait2) - (idle1 + iowait1);
  unsigned long long total1 =
      user1 + nice1 + system1 + idle1 + iowait1 + irq1 + softirq1 + steal1;
  unsigned long long total2 =
      user2 + nice2 + system2 + idle2 + iowait2 + irq2 + softirq2 + steal2;
  unsigned long long total_diff = total2 - total1;
  if (total_diff == 0) return 0.0;
  return 100.0 * (1.0 - ((double)idle_diff / total_diff));
}

void get_system_info(SystemInfo *info) {
  memset(info, 0, sizeof(SystemInfo));
  get_cpu_name(info->cpu_name, sizeof(info->cpu_name));
  info->logical_processors = get_cpu_cores();
  info->cpu_cores = get_physical_cores();
  info->cpu_clock_mhz = get_cpu_clock_mhz();
  info->hyperthreading_supported = is_hyperthreading_supported();
  detect_instruction_sets(info->instruction_sets,
                          sizeof(info->instruction_sets));

  struct sysinfo meminfo;
  if (sysinfo(&meminfo) == 0)
    info->total_ram_mb = meminfo.totalram / 1024 / 1024;

  get_os_name(info->os_name, sizeof(info->os_name));
  get_architecture(info->architecture, sizeof(info->architecture));
  info->is_virtual_machine = is_virtual_machine();
}

void get_real_time_system_info(RealTimeSystemInfo *info) {
  memset(info, 0, sizeof(RealTimeSystemInfo));
  info->cpu_usage_percent = calculate_cpu_usage();

  struct sysinfo meminfo;
  if (sysinfo(&meminfo) == 0) {
    info->used_ram_mb = meminfo.totalram - meminfo.freeram;
    info->used_ram_mb /= 1024 * 1024;  // Convert to MB
    info->ram_usage_percent =
        (double)info->used_ram_mb / (meminfo.totalram / 1024 / 1024) * 100.0;
  }
}

#endif  // _WIN32
