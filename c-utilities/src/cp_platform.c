// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#include "cp_platform.h"  // NOLINT(build/include_subdir)

#ifdef _WIN32
void cp_mutex_init(cp_mutex_t *mutex) {
  *mutex = CreateMutex(NULL, FALSE, NULL);
}

void cp_mutex_destroy(cp_mutex_t *mutex) { CloseHandle(*mutex); }

void cp_mutex_lock(cp_mutex_t *mutex) { WaitForSingleObject(*mutex, INFINITE); }

void cp_mutex_unlock(cp_mutex_t *mutex) { ReleaseMutex(*mutex); }

char *cp_strdup(const char *str) {
  size_t len = strlen(str) + 1;
  char *copy = (char *)malloc(len);
  if (copy) {
    memcpy(copy, str, len);
  }
  return copy;
}

struct tm *cp_localtime(const time_t *timep, struct tm *result) {
  localtime_s(result, timep);
  return result;
}

int64_t cp_get_current_us() {
  FILETIME ft;
  GetSystemTimeAsFileTime(&ft);
  uint64_t time = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
  return (int64_t)(time / 10 - 11644473600000000ULL);
}

void cp_get_memory_usage(uint64_t *total_allocated, uint64_t *total_free,
                         uint64_t *total_releasable) {
#ifdef _WIN32
  PROCESS_MEMORY_COUNTERS_EX pmc;
  if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&pmc,
                           sizeof(pmc))) {
    *total_allocated = pmc.WorkingSetSize;
    *total_free = 0;  // Windows does not provide free memory info per process
    *total_releasable = pmc.QuotaPagedPoolUsage;
  } else {
    *total_allocated = *total_free = *total_releasable = 0;
  }
#else
  struct mallinfo2 mi = mallinfo2();
  *total_allocated = mi.uordblks;
  *total_free = mi.fordblks;
  *total_releasable = mi.keepcost;
#endif
}

#else

void cp_mutex_init(cp_mutex_t *mutex) { pthread_mutex_init(mutex, NULL); }

void cp_mutex_destroy(cp_mutex_t *mutex) { pthread_mutex_destroy(mutex); }

void cp_mutex_lock(cp_mutex_t *mutex) { pthread_mutex_lock(mutex); }

void cp_mutex_unlock(cp_mutex_t *mutex) { pthread_mutex_unlock(mutex); }

char *cp_strdup(const char *str) { return strdup(str); }

struct tm *cp_localtime(const time_t *timep, struct tm *result) {
  return localtime_r(timep, result);
}

int64_t cp_get_current_us() {
  struct timeval time;
  gettimeofday(&time, NULL);
  return (int64_t)(time.tv_sec * 1000000 + time.tv_usec);
}

void cp_get_memory_usage(uint64_t *total_allocated, uint64_t *total_free,
                         uint64_t *total_releasable) {
  struct mallinfo mi = mallinfo();
  *total_allocated = mi.uordblks;
  *total_free = mi.fordblks;
  *total_releasable = 0;  // Not available on Linux
}

#endif
