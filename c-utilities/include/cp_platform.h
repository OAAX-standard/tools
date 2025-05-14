// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifndef C_UTILITIES_INCLUDE_CP_PLATFORM_H_
#define C_UTILITIES_INCLUDE_CP_PLATFORM_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
typedef HANDLE cp_mutex_t;

// Condition variable for Windows
typedef struct {
  CONDITION_VARIABLE cond;
} cp_cond_t;
#else
#include <malloc.h>
#include <pthread.h>
#include <sys/time.h>
typedef pthread_mutex_t cp_mutex_t;

// Condition variable for POSIX
typedef pthread_cond_t cp_cond_t;
#endif

// Cross-platform mutex functions
void cp_mutex_init(cp_mutex_t *mutex);
void cp_mutex_destroy(cp_mutex_t *mutex);
void cp_mutex_lock(cp_mutex_t *mutex);
void cp_mutex_unlock(cp_mutex_t *mutex);

// Cross-platform condition variable functions
void cp_cond_init(cp_cond_t *cond);
void cp_cond_destroy(cp_cond_t *cond);
int cp_cond_wait(cp_cond_t *cond, cp_mutex_t *mutex);
int cp_cond_timedwait(cp_cond_t *cond, cp_mutex_t *mutex, int64_t timeout_ms);
void cp_cond_signal(cp_cond_t *cond);
void cp_cond_broadcast(cp_cond_t *cond);

// Cross-platform strdup
char *cp_strdup(const char *str);

// Cross-platform localtime
struct tm *cp_localtime(const time_t *timep, struct tm *result);

// Cross-platform current time in microseconds
int64_t cp_get_current_us();

// Cross-platform memory usage retrieval
void cp_get_memory_usage(uint64_t *total_allocated, uint64_t *total_free,
                         uint64_t *total_releasable);

// Cross-platform sleep function (milliseconds)
void cp_sleep_ms(uint32_t milliseconds);

#endif  // C_UTILITIES_INCLUDE_CP_PLATFORM_H_
