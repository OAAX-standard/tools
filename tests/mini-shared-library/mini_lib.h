#ifndef MINI_LIB_H
#define MINI_LIB_H

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

#endif // MINI_LIB_H
