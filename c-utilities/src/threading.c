// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifndef _WIN32

#include "threading.h"  // NOLINT[build/include_subdir]

#include <stdlib.h>

int thread_create(ThreadHandle *handle, ThreadFunc func, void *arg) {
  return pthread_create(&handle->thread, NULL, func, arg);
}

int thread_join(ThreadHandle *handle) {
  return pthread_join(handle->thread, NULL);
}

#endif  // _WIN32
