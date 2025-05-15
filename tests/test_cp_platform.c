#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>
#include "cp_platform.h"

static void test_cp_strdup() {
    printf("Running test_cp_strdup...\n");
    const char *orig = "hello world";
    char *copy = cp_strdup(orig);
    assert(copy != NULL);
    assert(strcmp(orig, copy) == 0);
    free(copy);
    printf("test_cp_strdup passed.\n");
}

static void test_cp_localtime() {
    printf("Running test_cp_localtime...\n");
    time_t now = time(NULL);
    struct tm result;
    struct tm *tm_ptr = cp_localtime(&now, &result);
    assert(tm_ptr == &result);
    assert(result.tm_year > 100); // Should be after year 2000
    printf("test_cp_localtime passed.\n");
}

static void test_cp_get_current_us() {
    printf("Running test_cp_get_current_us...\n");
    int64_t t1 = cp_get_current_us();
    cp_sleep_ms(10);
    int64_t t2 = cp_get_current_us();
    assert(t2 > t1);
    printf("test_cp_get_current_us passed.\n");
}

static void test_cp_get_memory_usage() {
    printf("Running test_cp_get_memory_usage...\n");
    uint64_t total_alloc, total_free, total_rel;
    cp_get_memory_usage(&total_alloc, &total_free, &total_rel);
    // Just check that the function runs and returns something
    printf("Memory usage: allocated=%llu free=%llu releasable=%llu\n",
           (unsigned long long)total_alloc,
           (unsigned long long)total_free,
           (unsigned long long)total_rel);
    printf("test_cp_get_memory_usage passed.\n");
}

static void test_cp_sleep_ms() {
    printf("Running test_cp_sleep_ms...\n");
    int64_t t1 = cp_get_current_us();
    cp_sleep_ms(50);
    int64_t t2 = cp_get_current_us();
    assert(t2 - t1 >= 40000); // At least 40ms
    printf("test_cp_sleep_ms passed.\n");
}

void test_cp_platform_main() {
    printf("==== Testing cp_platform ====\n");
    test_cp_strdup();
    test_cp_localtime();
    test_cp_get_current_us();
    test_cp_get_memory_usage();
    test_cp_sleep_ms();
    printf("All cp_platform tests passed!\n");
}
