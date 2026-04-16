// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "oaax_utils.h"

// ---------------------------------------------------------------------------
// tensors_alloc / tensors_free
// ---------------------------------------------------------------------------

static void test_alloc_free() {
    printf("  test_alloc_free... ");

    // normal allocation
    Tensors* t = tensors_alloc(3);
    assert(t != NULL);
    assert(t->num_tensors == 3);
    assert(t->tensors != NULL);
    tensors_free(t);

    // zero tensors
    Tensors* t0 = tensors_alloc(0);
    assert(t0 != NULL);
    assert(t0->num_tensors == 0);
    tensors_free(t0);

    // NULL safety
    tensors_free(NULL);

    printf("OK\n");
}

// ---------------------------------------------------------------------------
// tensors_set
// ---------------------------------------------------------------------------

static void test_tensors_set() {
    printf("  test_tensors_set... ");

    Tensors* t = tensors_alloc(2);
    assert(t != NULL);

    float data0[6] = {1.f, 2.f, 3.f, 4.f, 5.f, 6.f};
    int shape0[] = {2, 3};
    tensors_set(t, 0, "input", DATA_TYPE_FLOAT, 2, shape0, data0, sizeof(data0));

    int32_t data1[4] = {10, 20, 30, 40};
    int shape1[] = {4};
    tensors_set(t, 1, "labels", DATA_TYPE_INT32, 1, shape1, data1, sizeof(data1));

    assert(strcmp(t->tensors[0].name, "input") == 0);
    assert(t->tensors[0].data_type == DATA_TYPE_FLOAT);
    assert(t->tensors[0].rank == 2);
    assert(t->tensors[0].shape[0] == 2 && t->tensors[0].shape[1] == 3);
    assert(t->tensors[0].data == data0);
    assert(t->tensors[0].data_size == sizeof(data0));

    assert(strcmp(t->tensors[1].name, "labels") == 0);
    assert(t->tensors[1].rank == 1);
    assert(t->tensors[1].shape[0] == 4);

    // overwrite descriptor 0 — old name/shape must be freed cleanly
    tensors_set(t, 0, "input_v2", DATA_TYPE_UINT8, 1, shape1, data1, sizeof(data1));
    assert(strcmp(t->tensors[0].name, "input_v2") == 0);
    assert(t->tensors[0].data_type == DATA_TYPE_UINT8);

    // out-of-range index: should be a no-op
    tensors_set(t, 99, "bad", DATA_TYPE_FLOAT, 0, NULL, NULL, 0);

    // don't free data (it's stack-allocated); NULL it out before tensors_free
    t->tensors[0].data = NULL;
    t->tensors[1].data = NULL;
    tensors_free(t);

    printf("OK\n");
}

// ---------------------------------------------------------------------------
// tensors_deep_copy
// ---------------------------------------------------------------------------

static void test_deep_copy() {
    printf("  test_deep_copy... ");

    Tensors* src = tensors_alloc(2);
    assert(src != NULL);
    src->id = 42;

    float* d0 = (float*)malloc(4 * sizeof(float));
    d0[0] = 1.f;
    d0[1] = 2.f;
    d0[2] = 3.f;
    d0[3] = 4.f;
    int shape0[] = {1, 4};
    tensors_set(src, 0, "out0", DATA_TYPE_FLOAT, 2, shape0, d0, 4 * sizeof(float));

    int32_t* d1 = (int32_t*)malloc(2 * sizeof(int32_t));
    d1[0] = 7;
    d1[1] = 8;
    int shape1[] = {2};
    tensors_set(src, 1, "out1", DATA_TYPE_INT32, 1, shape1, d1, 2 * sizeof(int32_t));

    Tensors* dst = tensors_deep_copy(src);
    assert(dst != NULL);
    assert(dst->id == 42);
    assert(dst->num_tensors == 2);

    // data is independent
    assert(dst->tensors[0].data != src->tensors[0].data);
    assert(memcmp(dst->tensors[0].data, src->tensors[0].data, 4 * sizeof(float)) == 0);

    // names are independent
    assert(dst->tensors[0].name != src->tensors[0].name);
    assert(strcmp(dst->tensors[0].name, "out0") == 0);

    // mutate dst, src unchanged
    ((float*)dst->tensors[0].data)[0] = 999.f;
    assert(((float*)src->tensors[0].data)[0] == 1.f);

    // NULL safety
    assert(tensors_deep_copy(NULL) == NULL);

    tensors_free(src);
    tensors_free(dst);

    printf("OK\n");
}

// ---------------------------------------------------------------------------
// tensors_compare
// ---------------------------------------------------------------------------

static void test_compare() {
    printf("  test_compare... ");

    Tensors* a = tensors_alloc(1);
    float* da = (float*)malloc(3 * sizeof(float));
    da[0] = 1.f;
    da[1] = 2.f;
    da[2] = 3.f;
    int sha[] = {3};
    tensors_set(a, 0, "x", DATA_TYPE_FLOAT, 1, sha, da, 3 * sizeof(float));

    Tensors* b = tensors_deep_copy(a);

    assert(tensors_compare(a, b) == true);

    // mutate b
    ((float*)b->tensors[0].data)[1] = 99.f;
    assert(tensors_compare(a, b) == false);

    assert(tensors_compare(NULL, b) == false);
    assert(tensors_compare(a, NULL) == false);

    tensors_free(a);
    tensors_free(b);

    printf("OK\n");
}

// ---------------------------------------------------------------------------
// compute_data_size
// ---------------------------------------------------------------------------

static void test_compute_data_size() {
    printf("  test_compute_data_size... ");

    int shape1d[] = {8};
    int shape2d[] = {2, 4};

    // float32: 8 elements * 4 bytes = 32
    assert(compute_data_size(DATA_TYPE_FLOAT, 1, shape1d) == 32);
    // float16: 8 elements * 2 bytes = 16
    assert(compute_data_size(DATA_TYPE_FLOAT16, 1, shape1d) == 16);
    // uint8: 8 elements * 1 byte = 8
    assert(compute_data_size(DATA_TYPE_UINT8, 1, shape1d) == 8);
    // int64: 8 elements * 8 bytes = 64
    assert(compute_data_size(DATA_TYPE_INT64, 1, shape1d) == 64);
    // complex128: 8 elements * 16 bytes = 128
    assert(compute_data_size(DATA_TYPE_COMPLEX128, 1, shape1d) == 128);

    // 2D shape: 2*4 = 8 elements, float = 32 bytes
    assert(compute_data_size(DATA_TYPE_FLOAT, 2, shape2d) == 32);

    // sub-byte: INT4, 8 elements → ceil(8/2) = 4 bytes
    assert(compute_data_size(DATA_TYPE_INT4, 1, shape1d) == 4);
    // sub-byte: UINT2, 8 elements → ceil(8/4) = 2 bytes
    assert(compute_data_size(DATA_TYPE_UINT2, 1, shape1d) == 2);
    // sub-byte: FLOAT4E2M1, 7 elements → ceil(7/2) = 4 bytes
    int shape7[] = {7};
    assert(compute_data_size(DATA_TYPE_FLOAT4E2M1, 1, shape7) == 4);

    // undefined → 0
    assert(compute_data_size(DATA_TYPE_UNDEFINED, 1, shape1d) == 0);

    // scalar (rank 0) → element byte size
    assert(compute_data_size(DATA_TYPE_FLOAT, 0, NULL) == 4);

    printf("OK\n");
}

// ---------------------------------------------------------------------------
// element_byte_size
// ---------------------------------------------------------------------------

static void test_element_byte_size() {
    printf("  test_element_byte_size... ");

    assert(element_byte_size(DATA_TYPE_FLOAT) == 4);
    assert(element_byte_size(DATA_TYPE_DOUBLE) == 8);
    assert(element_byte_size(DATA_TYPE_FLOAT16) == 2);
    assert(element_byte_size(DATA_TYPE_BFLOAT16) == 2);
    assert(element_byte_size(DATA_TYPE_INT8) == 1);
    assert(element_byte_size(DATA_TYPE_UINT64) == 8);
    assert(element_byte_size(DATA_TYPE_COMPLEX64) == 8);
    assert(element_byte_size(DATA_TYPE_COMPLEX128) == 16);
    // sub-byte → 0 (use compute_data_size instead)
    assert(element_byte_size(DATA_TYPE_INT4) == 0);
    assert(element_byte_size(DATA_TYPE_UINT2) == 0);
    assert(element_byte_size(DATA_TYPE_UNDEFINED) == 0);

    printf("OK\n");
}

// ---------------------------------------------------------------------------
// data_type_string
// ---------------------------------------------------------------------------

static void test_data_type_string() {
    printf("  test_data_type_string... ");

    assert(strcmp(data_type_string(DATA_TYPE_FLOAT), "DATA_TYPE_FLOAT") == 0);
    assert(strcmp(data_type_string(DATA_TYPE_INT8), "DATA_TYPE_INT8") == 0);
    assert(strcmp(data_type_string(DATA_TYPE_FLOAT16), "DATA_TYPE_FLOAT16") == 0);
    assert(strcmp(data_type_string(DATA_TYPE_BFLOAT16), "DATA_TYPE_BFLOAT16") == 0);
    assert(strcmp(data_type_string(DATA_TYPE_INT4), "DATA_TYPE_INT4") == 0);
    assert(strcmp(data_type_string(DATA_TYPE_UINT2), "DATA_TYPE_UINT2") == 0);
    assert(strcmp(data_type_string(DATA_TYPE_UNDEFINED), "DATA_TYPE_UNDEFINED") == 0);
    // unknown value → fallback
    assert(strcmp(data_type_string((TensorElementType)999), "DATA_TYPE_UNDEFINED") == 0);

    printf("OK\n");
}

// ---------------------------------------------------------------------------
// runtime_status_string
// ---------------------------------------------------------------------------

static void test_status_string() {
    printf("  test_status_string... ");

    assert(strcmp(runtime_status_string(RUNTIME_STATUS_SUCCESS), "RUNTIME_STATUS_SUCCESS") == 0);
    assert(strcmp(runtime_status_string(RUNTIME_STATUS_OUT_OF_MEMORY), "RUNTIME_STATUS_OUT_OF_MEMORY") == 0);
    assert(strcmp(runtime_status_string(RUNTIME_STATUS_DEVICE_ERROR), "RUNTIME_STATUS_DEVICE_ERROR") == 0);
    assert(strcmp(runtime_status_string(RUNTIME_STATUS_INVALID_MODEL_ID), "RUNTIME_STATUS_INVALID_MODEL_ID") == 0);
    // unknown value → fallback
    assert(strcmp(runtime_status_string((RuntimeStatus)999), "RUNTIME_STATUS_UNKNOWN_ERROR") == 0);

    printf("OK\n");
}

// ---------------------------------------------------------------------------
// tensors_find
// ---------------------------------------------------------------------------

static void test_tensors_find() {
    printf("  test_tensors_find... ");

    Tensors* t = tensors_alloc(2);
    float data[4] = {1.f, 2.f, 3.f, 4.f};
    int shape[] = {4};
    tensors_set(t, 0, "input", DATA_TYPE_FLOAT, 1, shape, data, sizeof(data));
    tensors_set(t, 1, "output", DATA_TYPE_FLOAT, 1, shape, data, sizeof(data));

    const TensorDescriptor* d = tensors_find(t, "output");
    assert(d != NULL);
    assert(strcmp(d->name, "output") == 0);

    assert(tensors_find(t, "missing") == NULL);
    assert(tensors_find(NULL, "input") == NULL);
    assert(tensors_find(t, NULL) == NULL);

    t->tensors[0].data = NULL;
    t->tensors[1].data = NULL;
    tensors_free(t);

    printf("OK\n");
}

// ---------------------------------------------------------------------------
// tensors_validate
// ---------------------------------------------------------------------------

static void test_tensors_validate() {
    printf("  test_tensors_validate... ");

    float data[4] = {0};
    int shape[] = {4};

    // valid tensors
    Tensors* t = tensors_alloc(1);
    tensors_set(t, 0, "x", DATA_TYPE_FLOAT, 1, shape, data, sizeof(data));
    assert(tensors_validate(t) == true);
    t->tensors[0].data = NULL;
    tensors_free(t);

    // NULL input
    assert(tensors_validate(NULL) == false);

    // num_tensors < 0
    Tensors* neg_count = tensors_alloc(0);
    neg_count->num_tensors = -1;
    assert(tensors_validate(neg_count) == false);
    neg_count->num_tensors = 0;
    tensors_free(neg_count);

    // num_tensors > 0 but tensors array is NULL
    Tensors* null_arr = tensors_alloc(0);
    null_arr->num_tensors = 2;
    null_arr->tensors = NULL;
    assert(tensors_validate(null_arr) == false);
    null_arr->num_tensors = 0;
    tensors_free(null_arr);

    // negative rank
    Tensors* neg_rank = tensors_alloc(1);
    neg_rank->tensors[0].rank = -1;
    assert(tensors_validate(neg_rank) == false);
    neg_rank->tensors[0].rank = 0;
    tensors_free(neg_rank);

    // rank > 0 but shape == NULL
    Tensors* bad_shape = tensors_alloc(1);
    bad_shape->tensors[0].rank = 2;
    assert(tensors_validate(bad_shape) == false);
    tensors_free(bad_shape);

    // shape dimension <= 0
    int bad_dim[] = {4, 0};
    Tensors* bad_dim_t = tensors_alloc(1);
    tensors_set(bad_dim_t, 0, "x", DATA_TYPE_FLOAT, 2, bad_dim, NULL, 0);
    assert(tensors_validate(bad_dim_t) == false);
    tensors_free(bad_dim_t);

    // data_size > 0 but data == NULL
    Tensors* bad_data = tensors_alloc(1);
    bad_data->tensors[0].data_size = 16;
    assert(tensors_validate(bad_data) == false);
    tensors_free(bad_data);

    // warning: undefined data type (does not fail)
    Tensors* undef_type = tensors_alloc(1);
    tensors_set(undef_type, 0, "x", DATA_TYPE_UNDEFINED, 1, shape, data, sizeof(data));
    assert(tensors_validate(undef_type) == true);
    undef_type->tensors[0].data = NULL;
    tensors_free(undef_type);

    // warning: data != NULL but data_size == 0 (does not fail)
    Tensors* zero_size = tensors_alloc(1);
    tensors_set(zero_size, 0, "x", DATA_TYPE_FLOAT, 1, shape, data, 0);
    assert(tensors_validate(zero_size) == true);
    zero_size->tensors[0].data = NULL;
    tensors_free(zero_size);

    // warning: data_size mismatch vs computed size (does not fail)
    Tensors* bad_size = tensors_alloc(1);
    tensors_set(bad_size, 0, "x", DATA_TYPE_FLOAT, 1, shape, data, 99);
    assert(tensors_validate(bad_size) == true);
    bad_size->tensors[0].data = NULL;
    tensors_free(bad_size);

    // warning: no name (does not fail)
    Tensors* no_name = tensors_alloc(1);
    tensors_set(no_name, 0, NULL, DATA_TYPE_FLOAT, 1, shape, data, sizeof(data));
    assert(tensors_validate(no_name) == true);
    no_name->tensors[0].data = NULL;
    tensors_free(no_name);

    // warning: duplicate names (does not fail)
    Tensors* dup = tensors_alloc(2);
    tensors_set(dup, 0, "out", DATA_TYPE_FLOAT, 1, shape, data, sizeof(data));
    tensors_set(dup, 1, "out", DATA_TYPE_FLOAT, 1, shape, data, sizeof(data));
    assert(tensors_validate(dup) == true);
    dup->tensors[0].data = NULL;
    dup->tensors[1].data = NULL;
    tensors_free(dup);

    printf("OK\n");
}

// ---------------------------------------------------------------------------
// config_print (smoke test — just checks it doesn't crash)
// ---------------------------------------------------------------------------

static void test_config_print() {
    printf("  test_config_print... ");

    Config* c = config_alloc();
    config_set(c, "device", "CPU");
    config_set(c, "log_level", "2");
    config_print(c);
    config_print(NULL);
    config_free(c);

    printf("OK\n");
}

// ---------------------------------------------------------------------------
// tensors_print (smoke test — just checks it doesn't crash)
// ---------------------------------------------------------------------------

static void test_tensors_print() {
    printf("  test_tensors_print... ");

    Tensors* t = tensors_alloc(2);
    float* d = (float*)malloc(4 * sizeof(float));
    d[0] = 1.f;
    d[1] = 2.f;
    d[2] = 3.f;
    d[3] = 4.f;
    int shape[] = {2, 2};
    tensors_set(t, 0, "output", DATA_TYPE_FLOAT, 2, shape, d, 4 * sizeof(float));
    tensors_print(t);
    tensors_print(NULL);

    tensors_free(t);
    printf("OK\n");
}

// ---------------------------------------------------------------------------
// config_alloc / config_set / config_get / config_extend / config_delete
// ---------------------------------------------------------------------------

static void test_config_managed() {
    printf("  test_config_managed... ");

    Config* c = config_alloc();
    assert(c != NULL);
    assert(c->length == 0);

    // set new keys
    assert(config_set(c, "device", "CPU") == true);
    assert(config_set(c, "log_level", "2") == true);
    assert(c->length == 2);

    // get existing and missing
    assert(strcmp(config_get(c, "device"), "CPU") == 0);
    assert(strcmp(config_get(c, "log_level"), "2") == 0);
    assert(config_get(c, "missing") == NULL);
    assert(config_get(NULL, "device") == NULL);
    assert(config_get(c, NULL) == NULL);

    // update existing key — length must not change
    assert(config_set(c, "device", "GPU") == true);
    assert(c->length == 2);
    assert(strcmp(config_get(c, "device"), "GPU") == 0);

    // set new key
    assert(config_set(c, "cache_dir", "/tmp") == true);
    assert(c->length == 3);
    assert(strcmp(config_get(c, "cache_dir"), "/tmp") == 0);

    // delete existing key
    assert(config_delete(c, "log_level") == true);
    assert(c->length == 2);
    assert(config_get(c, "log_level") == NULL);
    // remaining keys must still be accessible
    assert(strcmp(config_get(c, "device"), "GPU") == 0);
    assert(strcmp(config_get(c, "cache_dir"), "/tmp") == 0);

    // delete non-existent key
    assert(config_delete(c, "nonexistent") == false);

    // NULL safety
    assert(config_set(NULL, "k", "v") == false);
    assert(config_set(c, NULL, "v") == false);
    assert(config_delete(NULL, "device") == false);
    assert(config_delete(c, NULL) == false);

    config_free(c);
    config_free(NULL);  // safe

    printf("OK\n");
}

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------

int test_oaax_utils_main(void) {
    printf("Running oaax_utils tests...\n");
    test_alloc_free();
    test_tensors_set();
    test_deep_copy();
    test_compare();
    test_compute_data_size();
    test_element_byte_size();
    test_data_type_string();
    test_status_string();
    test_tensors_find();
    test_tensors_validate();
    test_config_managed();
    test_config_print();
    test_tensors_print();
    printf("All oaax_utils tests passed.\n\n");
    return 0;
}
