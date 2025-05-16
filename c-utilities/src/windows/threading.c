// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifdef _WIN32
#include "threading.h"
#include <process.h>

int thread_create(ThreadHandle *handle, ThreadFunc func, void *arg) {
  handle->handle = (HANDLE)_beginthreadex(
      NULL, 0, (unsigned(__stdcall *)(void *))func, arg, 0, NULL);
  return handle->handle ? 0 : -1;
}

int thread_join(ThreadHandle *handle) {
#ifdef _WIN32
  if (WaitForSingleObject(handle->handle, INFINITE) == WAIT_OBJECT_0) {
    CloseHandle(handle->handle);
    return 0;
  }
  return -1;
#else
  return pthread_join(handle->thread, NULL);
#endif
}

#endif  // _WIN32
