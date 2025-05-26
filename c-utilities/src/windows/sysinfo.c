// Windows version of sysinfo.c
// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifdef _WIN32

#include "sysinfo.h"  // NOLINT(build/include_subdir)

#include <intrin.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <psapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <windows.h>

static void get_cpu_name(char *buf, size_t size) {
  HKEY hKey;
  const char *keyPath = "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";
  const char *valueName = "ProcessorNameString";
  DWORD dataSize = (DWORD)size;

  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_READ, &hKey) ==
      ERROR_SUCCESS) {
    if (RegQueryValueExA(hKey, valueName, NULL, NULL, (LPBYTE)buf, &dataSize) !=
        ERROR_SUCCESS) {
      strncpy_s(buf, size, "Unknown", _TRUNCATE);
    }
    RegCloseKey(hKey);
  } else {
    strncpy_s(buf, size, "Unknown", _TRUNCATE);
  }
}

static int get_cpu_cores() {
  SYSTEM_INFO sysInfo;
  GetSystemInfo(&sysInfo);
  return (int)sysInfo.dwNumberOfProcessors;
}

static int get_physical_cores() {
  DWORD len = 0;
  GetLogicalProcessorInformation(NULL, &len);
  SYSTEM_LOGICAL_PROCESSOR_INFORMATION *buffer =
      (SYSTEM_LOGICAL_PROCESSOR_INFORMATION *)malloc(len);
  if (!GetLogicalProcessorInformation(buffer, &len)) {
    free(buffer);
    return get_cpu_cores();
  }

  DWORD count = len / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
  int physical_cores = 0;
  for (DWORD i = 0; i < count; i++) {
    if (buffer[i].Relationship == RelationProcessorCore) physical_cores++;
  }
  free(buffer);
  return physical_cores > 0 ? physical_cores : get_cpu_cores();
}

static int get_cpu_clock_mhz() {
  HKEY hKey;
  DWORD speed = 0;
  DWORD size = sizeof(DWORD);
  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                    "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0,
                    KEY_READ, &hKey) == ERROR_SUCCESS) {
    RegQueryValueExA(hKey, "~MHz", NULL, NULL, (LPBYTE)&speed, &size);
    RegCloseKey(hKey);
  }
  return (int)speed;
}

static void detect_instruction_sets(char *buf, size_t size) {
  static void detect_instruction_sets(char *buf, size_t size) {
#if defined(_M_ARM64) || defined(_M_ARM)
    // ARM/ARM64 SIMD support (simplified)
    int has_neon =
        IsProcessorFeaturePresent(PF_ARM_NEON_INSTRUCTIONS_AVAILABLE);
    int has_v8 = IsProcessorFeaturePresent(PF_ARM_V8_INSTRUCTIONS_AVAILABLE);
    snprintf(buf, size, "NEON:%s ARMv8:%s", has_neon ? "Yes" : "No",
             has_v8 ? "Yes" : "No");
#elif defined(_M_IX86) || defined(_M_X64)
    // x86/x64 features (basic detection via processor feature API)
    int has_sse = IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE);
    int has_sse2 = IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE);
    int has_sse3 = IsProcessorFeaturePresent(PF_SSE3_INSTRUCTIONS_AVAILABLE);
    int has_avx = IsProcessorFeaturePresent(PF_AVX_INSTRUCTIONS_AVAILABLE);
    int has_avx2 = IsProcessorFeaturePresent(PF_AVX2_INSTRUCTIONS_AVAILABLE);
    snprintf(buf, size, "SSE:%s SSE2:%s SSE3:%s AVX:%s AVX2:%s",
             has_sse ? "Yes" : "No", has_sse2 ? "Yes" : "No",
             has_sse3 ? "Yes" : "No", has_avx ? "Yes" : "No",
             has_avx2 ? "Yes" : "No");
#else
    snprintf(buf, size, "Instruction set detection not supported");
#endif
  }
}

static int is_hyperthreading_supported() {
  int logical = get_cpu_cores();
  int physical = get_physical_cores();
  return (logical > physical) ? 1 : 0;
}

static void get_os_name(char *buf, size_t size) {
  HKEY hKey;
  DWORD major = 0, minor = 0, build = 0;
  DWORD dataSize = sizeof(DWORD);
  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                    "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0,
                    KEY_READ, &hKey) == ERROR_SUCCESS) {
    RegQueryValueExA(hKey, "CurrentMajorVersionNumber", NULL, NULL,
                     (LPBYTE)&major, &dataSize);
    dataSize = sizeof(DWORD);
    RegQueryValueExA(hKey, "CurrentMinorVersionNumber", NULL, NULL,
                     (LPBYTE)&minor, &dataSize);
    char buildStr[32] = {0};
    dataSize = sizeof(buildStr);
    if (RegQueryValueExA(hKey, "CurrentBuildNumber", NULL, NULL,
                         (LPBYTE)buildStr, &dataSize) == ERROR_SUCCESS) {
      build = (DWORD)strtoul(buildStr, NULL, 10);
    }
    RegCloseKey(hKey);
    snprintf(buf, size, "Windows %lu.%lu (Build %lu)", major, minor, build);
  } else {
    strncpy_s(buf, size, "Windows", _TRUNCATE);
  }
}

static void get_architecture(char *buf, size_t size) {
  SYSTEM_INFO sysInfo;
  GetNativeSystemInfo(&sysInfo);
  switch (sysInfo.wProcessorArchitecture) {
    case PROCESSOR_ARCHITECTURE_AMD64:
      strncpy_s(buf, size, "x64", _TRUNCATE);
      break;
    case PROCESSOR_ARCHITECTURE_INTEL:
      strncpy_s(buf, size, "x86", _TRUNCATE);
      break;
    case PROCESSOR_ARCHITECTURE_ARM64:
      strncpy_s(buf, size, "ARM64", _TRUNCATE);
      break;
    default:
      strncpy_s(buf, size, "Unknown", _TRUNCATE);
  }
}

static int is_virtual_machine() {
  HKEY hKey;
  const char *keyPath = "HARDWARE\\DESCRIPTION\\System\\BIOS";
  char systemManufacturer[128] = {0};
  DWORD size = sizeof(systemManufacturer);

  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_READ, &hKey) !=
      ERROR_SUCCESS)
    return 0;

  if (RegQueryValueExA(hKey, "SystemManufacturer", NULL, NULL,
                       (LPBYTE)systemManufacturer, &size) != ERROR_SUCCESS) {
    RegCloseKey(hKey);
    return 0;
  }

  RegCloseKey(hKey);

  // Convert to lowercase for case-insensitive match
  for (char *p = systemManufacturer; *p; ++p) *p = (char)tolower(*p);

  // Common VM manufacturer strings
  const char *vm_names[] = {"vmware", "virtualbox", "qemu",      "kvm",
                            "xen",    "microsoft",  "parallels", "bhyve"};

  for (size_t i = 0; i < sizeof(vm_names) / sizeof(vm_names[0]); ++i) {
    if (strstr(systemManufacturer, vm_names[i]) != NULL) return 1;
  }

  return 0;
}

static float calculate_cpu_usage() {
  FILETIME idleTime1, kernelTime1, userTime1;
  FILETIME idleTime2, kernelTime2, userTime2;
  ULARGE_INTEGER idle1, kernel1, user1, idle2, kernel2, user2;

  GetSystemTimes(&idleTime1, &kernelTime1, &userTime1);
  Sleep(100);
  GetSystemTimes(&idleTime2, &kernelTime2, &userTime2);

  memcpy(&idle1, &idleTime1, sizeof(ULARGE_INTEGER));
  memcpy(&kernel1, &kernelTime1, sizeof(ULARGE_INTEGER));
  memcpy(&user1, &userTime1, sizeof(ULARGE_INTEGER));

  memcpy(&idle2, &idleTime2, sizeof(ULARGE_INTEGER));
  memcpy(&kernel2, &kernelTime2, sizeof(ULARGE_INTEGER));
  memcpy(&user2, &userTime2, sizeof(ULARGE_INTEGER));

  ULONGLONG sys1 = kernel1.QuadPart + user1.QuadPart;
  ULONGLONG sys2 = kernel2.QuadPart + user2.QuadPart;
  ULONGLONG idle_diff = idle2.QuadPart - idle1.QuadPart;
  ULONGLONG total_diff = (sys2 - sys1);

  if (total_diff == 0) return 0.0f;
  return 100.0f * (1.0f - ((float)idle_diff / total_diff));
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

  MEMORYSTATUSEX memStatus;
  memStatus.dwLength = sizeof(memStatus);
  if (GlobalMemoryStatusEx(&memStatus)) {
    info->total_ram_mb = (int64_t)(memStatus.ullTotalPhys / (1024 * 1024));
  }

  get_os_name(info->os_name, sizeof(info->os_name));
  get_architecture(info->architecture, sizeof(info->architecture));
  info->is_virtual_machine = is_virtual_machine();
}

void get_real_time_system_info(RealTimeSystemInfo *info) {
  info->cpu_usage_percent = calculate_cpu_usage();
  MEMORYSTATUSEX mem;
  mem.dwLength = sizeof(mem);
  if (GlobalMemoryStatusEx(&mem)) {
    info->used_ram_mb = (mem.ullTotalPhys - mem.ullAvailPhys) / (1024 * 1024);
    info->ram_usage_percent =
        (float)info->used_ram_mb / (mem.ullTotalPhys / (1024 * 1024)) * 100.0f;
  }
}

#endif  // _WIN32
