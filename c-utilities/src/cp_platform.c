// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#include "cp_platform.h"  // NOLINT(build/include_subdir)

#ifdef _WIN32
#include <psapi.h>  // Required for PROCESS_MEMORY_COUNTERS_EX

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
  PROCESS_MEMORY_COUNTERS_EX pmc;
  if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&pmc,
                           sizeof(pmc))) {
    *total_allocated = pmc.WorkingSetSize;
    *total_free = 0;  // Windows does not provide free memory info per process
    *total_releasable = pmc.QuotaPagedPoolUsage;
  } else {
    *total_allocated = *total_free = *total_releasable = 0;
  }
}

void cp_sleep_ms(uint32_t milliseconds) { Sleep(milliseconds); }

// Condition variable functions for Windows
void cp_cond_init(cp_cond_t *cond) {
  InitializeConditionVariable(&cond->cond);
}
void cp_cond_destroy(cp_cond_t *cond) {
  // No-op for Windows CONDITION_VARIABLE
  (void)cond;
}
void cp_cond_wait(cp_cond_t *cond, cp_mutex_t *mutex) {
  SleepConditionVariableMutex(&cond->cond, *mutex, INFINITE);
}
int cp_cond_timedwait(cp_cond_t *cond, cp_mutex_t *mutex, int64_t timeout_ms) {
  return SleepConditionVariableMutex(&cond->cond, *mutex, (DWORD)timeout_ms) ? 0 : 1;
}
void cp_cond_signal(cp_cond_t *cond) {
  WakeConditionVariable(&cond->cond);
}
void cp_cond_broadcast(cp_cond_t *cond) {
  WakeAllConditionVariable(&cond->cond);
}

// Thread functions for Windows
int cp_thread_create(cp_thread_t *thread, cp_thread_func_t func, void *arg) {
  *thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)func, arg, 0, NULL);
  return *thread ? 0 : -1;
}
int cp_thread_join(cp_thread_t thread) {
  DWORD res = WaitForSingleObject(thread, INFINITE);
  CloseHandle(thread);
  return (res == WAIT_OBJECT_0) ? 0 : -1;
}

#else

void cp_sleep_ms(uint32_t milliseconds) {
  struct timespec ts;
  ts.tv_sec = milliseconds / 1000;
  ts.tv_nsec = (milliseconds % 1000) * 1000000;
  nanosleep(&ts, NULL);
}

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
  struct mallinfo2 mi = mallinfo2();
  *total_allocated = mi.uordblks;
  *total_free = mi.fordblks;
  *total_releasable = mi.keepcost;
}

// Condition variable functions for POSIX
void cp_cond_init(cp_cond_t *cond) {
  pthread_cond_init(cond, NULL);
}
void cp_cond_destroy(cp_cond_t *cond) {
  pthread_cond_destroy(cond);
}
int cp_cond_wait(cp_cond_t *cond, cp_mutex_t *mutex) {
  return pthread_cond_wait(cond, mutex);
}
int cp_cond_timedwait(cp_cond_t *cond, cp_mutex_t *mutex, int64_t timeout_ms) {
  if (timeout_ms <= 0) {
    return pthread_cond_wait(cond, mutex);
  }
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  ts.tv_sec += timeout_ms / 1000;
  ts.tv_nsec += (timeout_ms % 1000) * 1000000;
  if (ts.tv_nsec >= 1000000000) {
    ts.tv_sec += 1;
    ts.tv_nsec -= 1000000000;
  }
  return pthread_cond_timedwait(cond, mutex, &ts);
}
void cp_cond_signal(cp_cond_t *cond) {
  pthread_cond_signal(cond);
}
void cp_cond_broadcast(cp_cond_t *cond) {
  pthread_cond_broadcast(cond);
}

// Thread functions for POSIX
int cp_thread_create(cp_thread_t *thread, cp_thread_func_t func, void *arg) {
  return pthread_create(thread, NULL, func, arg);
}
int cp_thread_join(cp_thread_t thread) {
  return pthread_join(thread, NULL);
}

#endif
