// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#ifndef C_UTILITIES_INCLUDE_TENSORS_STRUCT_H_
#define C_UTILITIES_INCLUDE_TENSORS_STRUCT_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum tensor_data_type {
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
} tensor_data_type;

typedef struct tensors_struct {
  size_t num_tensors;            // Number of tensors
  char** names;                  // Names of the tensors
  tensor_data_type* data_types;  // Data types of the tensors
  size_t* ranks;                 // Ranks of the tensors
  size_t** shapes;               // Shapes of the tensors
  void** data;                   // Data of the tensors
} tensors_struct;

/**
 * @brief Allocates and initializes a tensors_struct object.
 *
 * This function dynamically allocates memory for a tensors_struct object
 * including its fields.
 *
 * @param num_tensors The number of tensors to allocate in the structure.
 * @return A pointer to the allocated tensors_struct object, or nullptr if
 *         the allocation fails.
 */
tensors_struct* allocate_tensors_struct(int num_tensors);

/**
 * @brief Deeply frees all memory associated with a tensors_struct.
 *
 * @param tensors Pointer to the tensors_struct to be freed.
 *
 * @note the tensors parameters is assumed to be a valid pointer allocated using
 * malloc.
 */
void deep_free_tensors_struct(tensors_struct* tensors);

/**
 * @brief Shallowly frees memory associated with a tensors_struct.
 *
 * @param tensors Pointer to the tensors_struct to be freed.
 */
void shallow_free_tensors_struct(tensors_struct* tensors);

/**
 * @brief Creates a deep copy of a tensors_struct.
 *
 * @param src Pointer to the source tensors_struct.
 * @return A pointer to the newly created deep copy of the tensors_struct.
 */
tensors_struct* deep_copy_tensors_struct(const tensors_struct* src);

/**
 * @brief Creates a shallow copy of a tensors_struct.
 *
 * @param src Pointer to the source tensors_struct.
 * @param dst Pointer to the destination tensors_struct.
 * @return A pointer to the newly created shallow copy of the tensors_struct.
 */
void shallow_copy_tensors_struct(const tensors_struct* src,
                                 tensors_struct* dst);

/**
 * @brief Prints a summary of the metadata of tensors in a tensors_struct.
 *
 * @param tensors Pointer to the tensors_struct whose metadata is to be printed.
 */
void print_tensors_metadata(const tensors_struct* tensors);

/**
 * @brief Gets the byte size of a given tensor data type.
 *
 * @param type The tensor_data_type for which the byte size is to be determined.
 * @return The byte size of the specified data type.
 */
int8_t get_data_type_byte_size(tensor_data_type type);

/**
 * @brief Compares two tensors_structs for equality.
 *
 * @param tensors1 Pointer to the first tensors_struct.
 * @param tensors2 Pointer to the second tensors_struct.
 * @return true if the two tensors_structs have the exact same bytes, false
 * otherwise.
 */
bool compare_two_tensors_structs(const tensors_struct* tensors1,
                                 const tensors_struct* tensors2);

tensors_struct* create_sample_tensors_struct(int seed);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // C_UTILITIES_INCLUDE_TENSORS_STRUCT_H_
