// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifndef OAAX_RUNTIME_UTILS_H
#define OAAX_RUNTIME_UTILS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------
// Types — mirror of the OAAX v2.0.0 interface
// ---------------------------------------------------------------------------

typedef enum RuntimeStatus {
    RUNTIME_STATUS_SUCCESS = 0,
    RUNTIME_STATUS_ERROR = 1,
    RUNTIME_STATUS_NOT_INITIALIZED = 2,
    RUNTIME_STATUS_ALREADY_INITIALIZED = 3,
    RUNTIME_STATUS_MODEL_NOT_LOADED = 4,
    RUNTIME_STATUS_INVALID_ARGUMENT = 5,
    RUNTIME_STATUS_INVALID_MODEL = 6,
    RUNTIME_STATUS_FILE_NOT_FOUND = 7,
    RUNTIME_STATUS_OUT_OF_MEMORY = 8,
    RUNTIME_STATUS_INVALID_TENSOR = 9,
    RUNTIME_STATUS_TENSOR_SHAPE_MISMATCH = 10,
    RUNTIME_STATUS_TENSOR_TYPE_MISMATCH = 11,
    RUNTIME_STATUS_NO_OUTPUT_AVAILABLE = 12,
    RUNTIME_STATUS_INFERENCE_ERROR = 13,
    RUNTIME_STATUS_NOT_IMPLEMENTED = 14,
    RUNTIME_STATUS_TIMEOUT = 15,
    RUNTIME_STATUS_DEVICE_ERROR = 16,
    RUNTIME_STATUS_UNKNOWN_ERROR = 17,
    RUNTIME_STATUS_INVALID_MODEL_ID = 18
} RuntimeStatus;

typedef enum TensorElementType {
    DATA_TYPE_UNDEFINED = 0,
    DATA_TYPE_FLOAT = 1,
    DATA_TYPE_UINT8 = 2,
    DATA_TYPE_INT8 = 3,
    DATA_TYPE_UINT16 = 4,
    DATA_TYPE_INT16 = 5,
    DATA_TYPE_INT32 = 6,
    DATA_TYPE_INT64 = 7,
    DATA_TYPE_STRING = 8,
    DATA_TYPE_BOOL = 9,
    DATA_TYPE_FLOAT16 = 10,
    DATA_TYPE_DOUBLE = 11,
    DATA_TYPE_UINT32 = 12,
    DATA_TYPE_UINT64 = 13,
    DATA_TYPE_COMPLEX64 = 14,
    DATA_TYPE_COMPLEX128 = 15,
    DATA_TYPE_BFLOAT16 = 16,
    DATA_TYPE_FLOAT8E4M3FN = 17,
    DATA_TYPE_FLOAT8E4M3FNUZ = 18,
    DATA_TYPE_FLOAT8E5M2 = 19,
    DATA_TYPE_FLOAT8E5M2FNUZ = 20,
    DATA_TYPE_UINT4 = 21,
    DATA_TYPE_INT4 = 22,
    DATA_TYPE_FLOAT4E2M1 = 23,
    DATA_TYPE_FLOAT8E8M0 = 24,
    DATA_TYPE_UINT2 = 25,
    DATA_TYPE_INT2 = 26
} TensorElementType;

/**
 * @brief Descriptor for a single tensor.
 *
 * All metadata for one tensor is co-located here, fitting in a single cache
 * line (~40 bytes on 64-bit). Caller owns name, shape, and data; see
 * tensors_free() for the expected free order.
 */
typedef struct TensorDescriptor {
    char             *name;      // null-terminated tensor name
    TensorElementType data_type; // element type
    int               rank;      // number of dimensions
    int              *shape;     // array of rank dimension sizes
    size_t            data_size; // byte size of data buffer
    void             *data;      // tensor data buffer
} TensorDescriptor;

/**
 * @brief Set of tensors for one inference request.
 *
 * id: caller-assigned correlation tag; the runtime must echo it unchanged on
 * the corresponding output Tensors so requests can be matched.
 */
typedef struct Tensors {
    int               id;          // caller-assigned request ID
    int               num_tensors; // number of descriptors
    TensorDescriptor *tensors;     // array of num_tensors descriptors
} Tensors;

/**
 * @brief Key-value configuration. All values are null-terminated strings.
 */
typedef struct Config {
    int          length; // number of entries
    const char **keys;
    const char **values;
} Config;

/**
 * @brief Model loading configuration.
 *
 * Provide either file_path or (model_data + model_size), not both.
 */
typedef struct ModelConfig {
    const char          *file_path;  // path to model file (optional)
    const unsigned char *model_data; // in-memory model blob (optional)
    size_t               model_size; // byte size of model_data (optional)
    Config               config;     // model-specific key-value config
} ModelConfig;

// ---------------------------------------------------------------------------
// Tensor lifecycle helpers
// ---------------------------------------------------------------------------

/**
 * @brief Allocate a Tensors with num_tensors zero-initialised descriptors.
 *
 * Returns NULL on allocation failure.
 * Free with tensors_free().
 */
Tensors *tensors_alloc(int num_tensors);

/**
 * @brief Deep-free a Tensors and all its descriptors.
 *
 * Frees name, shape, and data for every descriptor, then the descriptor
 * array, then the Tensors struct itself. Safe to call with NULL.
 */
void tensors_free(Tensors *tensors);

/**
 * @brief Deep-copy a Tensors.
 *
 * Allocates fresh memory for every field. Returns NULL on failure.
 * The copy must be freed with tensors_free().
 */
Tensors *tensors_deep_copy(const Tensors *src);

/**
 * @brief Fill one descriptor inside a Tensors.
 *
 * Copies name (strdup) and shape (malloc + memcpy). Stores data as-is
 * (no copy — data may be large). data_size must be set by the caller;
 * use compute_data_size() if needed.
 *
 * Any previously allocated name or shape at index is freed before
 * overwriting. Does nothing if tensors is NULL or index is out of range.
 */
void tensors_set(Tensors *tensors, int index,
                 const char *name,
                 TensorElementType data_type,
                 int rank, const int *shape,
                 void *data, size_t data_size);

/**
 * @brief Compare two Tensors for equality (metadata + data).
 *
 * Returns false if either pointer is NULL or if any field differs.
 */
bool tensors_compare(const Tensors *a, const Tensors *b);

/**
 * @brief Print a human-readable summary of a Tensors to stdout.
 */
void tensors_print(const Tensors *tensors);

// ---------------------------------------------------------------------------
// Data size helpers
// ---------------------------------------------------------------------------

/**
 * @brief Compute the byte size of a tensor buffer from its type and shape.
 *
 * Handles sub-byte types (INT4/UINT4/FLOAT4: packed two per byte;
 * INT2/UINT2: packed four per byte) correctly.
 * Returns 0 for DATA_TYPE_UNDEFINED or unknown types.
 */
size_t compute_data_size(TensorElementType data_type, int rank, const int *shape);

/**
 * @brief Return the byte size of one element for a given type.
 *
 * Returns 0 for sub-byte types (use compute_data_size instead) and for
 * DATA_TYPE_UNDEFINED / unknown types.
 */
size_t element_byte_size(TensorElementType data_type);

// ---------------------------------------------------------------------------
// Config helpers
// ---------------------------------------------------------------------------

/**
 * @brief Build a Config view from caller-owned parallel arrays.
 *
 * Does not copy anything. The caller must keep keys/values alive as long as
 * the Config is used. Do NOT pass the result to config_free().
 */
Config config_create(int length, const char **keys, const char **values);

/**
 * @brief Allocate an empty heap-owned Config.
 *
 * Must be freed with config_free(). Returns NULL on OOM.
 */
Config *config_alloc(void);

/**
 * @brief Free a heap-owned Config created by config_alloc().
 *
 * Frees every copied key and value string, the internal arrays, and the Config
 * struct itself. Safe to call with NULL.
 */
void config_free(Config *c);

/**
 * @brief Set or update a key in a heap-owned Config.
 *
 * Copies key and value (strdup). If the key already exists its value is
 * replaced. Returns false on OOM or invalid arguments.
 */
bool config_set(Config *c, const char *key, const char *value);

/**
 * @brief Return the value for a key, or NULL if not found.
 *
 * Linear scan. The returned pointer is owned by the Config — do not free it.
 */
const char *config_get(const Config *c, const char *key);


/**
 * @brief Remove an entry by key from a heap-owned Config.
 *
 * Returns true if the key was found and removed, false otherwise.
 */
bool config_delete(Config *c, const char *key);

// ---------------------------------------------------------------------------
// Status helper
// ---------------------------------------------------------------------------

/**
 * @brief Return a human-readable name for a RuntimeStatus value.
 *
 * Returns "RUNTIME_STATUS_UNKNOWN_ERROR" for unrecognised values.
 * The returned string is a string literal — never free it.
 */
const char *runtime_status_string(RuntimeStatus status);

#ifdef __cplusplus
}
#endif

#endif // OAAX_RUNTIME_UTILS_H
