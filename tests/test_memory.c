#include <stdio.h>
#include <stdint.h>
#include "memory.h"

#ifdef _WIN32
#ifdef _MSC_VER
#define UINT64_FORMAT "%I64u"
#else
#define UINT64_FORMAT "%llu"
#endif
#else
#define UINT64_FORMAT "%lu"
#endif

// Test human-readable memory size formatting
static void test_human_memory_size() {
  printf("Running test_human_memory_size...\n");
  uint64_t test_values[] = {0, 512, 1024, 1048576, 1073741824, 1099511627776ULL};
  for (int i = 0; i < 6; ++i) {
    printf(UINT64_FORMAT " bytes -> %s\n", (uint64_t)test_values[i],
           human_memory_size(test_values[i]));
  }
}

// Test printing current memory usage
static void test_print_memory_usage() {
  printf("Running test_print_memory_usage...\n");
  print_memory_usage("test_print_memory_usage");
}

void test_memory_main() {
  test_human_memory_size();
  test_print_memory_usage();
}
