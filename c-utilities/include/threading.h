// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifndef C_UTILITIES_INCLUDE_THREADING_H_
#define C_UTILITIES_INCLUDE_THREADING_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

typedef void *(*ThreadFunc)(void *);

typedef struct ThreadHandle {
#ifdef _WIN32
  HANDLE handle;
#else
  pthread_t thread;
#endif
} ThreadHandle;

// Create and start a thread
int thread_create(ThreadHandle *handle, ThreadFunc func, void *arg);

// Wait for a thread to finish
int thread_join(ThreadHandle *handle);

#ifdef __cplusplus
}
#endif

#endif  // C_UTILITIES_INCLUDE_THREADING_H_
