// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifndef TESTS_MINI_SHARED_LIBRARY_MINI_LIB_H_
#define TESTS_MINI_SHARED_LIBRARY_MINI_LIB_H_

#ifdef _WIN32
  #define MINI_API __declspec(dllexport)
#else
  #define MINI_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

MINI_API int mini_add(int a, int b);

#ifdef __cplusplus
}
#endif

#endif  // TESTS_MINI_SHARED_LIBRARY_MINI_LIB_H_
