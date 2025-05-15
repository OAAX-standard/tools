// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#include "tensors_struct.h"  // NOLINT(build/include_subdir)

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int8_t get_data_type_byte_size(tensor_data_type type) {
  switch (type) {
    case DATA_TYPE_FLOAT:
      return sizeof(float);
    case DATA_TYPE_UINT8:
      return sizeof(uint8_t);
    case DATA_TYPE_INT8:
      return sizeof(int8_t);
    case DATA_TYPE_UINT16:
      return sizeof(uint16_t);
    case DATA_TYPE_INT16:
      return sizeof(int16_t);
    case DATA_TYPE_INT32:
      return sizeof(int32_t);
    case DATA_TYPE_INT64:
      return sizeof(int64_t);
    case DATA_TYPE_STRING:
      return sizeof(char*);
    case DATA_TYPE_BOOL:
      return sizeof(bool);
    case DATA_TYPE_DOUBLE:
      return sizeof(double);
    case DATA_TYPE_UINT32:
      return sizeof(uint32_t);
    case DATA_TYPE_UINT64:
      return sizeof(uint64_t);
    default:
      printf("Error: Unknown data type %d\n", type);
      return 0;  // Return 0 for unknown data types
  }
}

void print_tensors_metadata(const tensors_struct* tensors) {
  // Print number of tensors
  printf("Number of tensors: %zu\n", tensors->num_tensors);
  // Iterate through each tensor
  for (int i = 0; i < tensors->num_tensors; ++i) {
    printf("Tensor id=%d:\n", i);
    printf("  Name: '%s'\n", tensors->names[i]);
    printf("  Data type: %d\n", tensors->data_types[i]);
    printf("  Rank: %zu\n", tensors->ranks[i]);
    printf("  Shape: ");
    for (size_t j = 0; j < tensors->ranks[i]; ++j) {
      printf("%zu ", tensors->shapes[i][j]);
    }
    printf("\n");
    printf("  Data Pointer: %p\n", tensors->data[i]);
  }
  printf("\n");
}

void deep_free_tensors_struct(tensors_struct* tensors) {
  // Check if tensors is NULL
  if (tensors == NULL) {
    printf("Warning: tensors is NULL\n");
    return;
  }
  // check if the names array is NULL
  if (tensors->names != NULL) {
    // Iterate through each tensor
    for (size_t i = 0; i < tensors->num_tensors; ++i) {
      // Free the name of the tensor if not NULL
      if (tensors->names[i] != NULL) {
        free(tensors->names[i]);
      }
    }
    // Free the names array
    free(tensors->names);
  } else {
    // Print a warning if names is NULL
    printf("Warning: names array is NULL\n");
  }

  // Check if the shapes array is NULL
  if (tensors->shapes != NULL) {
    // Iterate through each tensor
    for (size_t i = 0; i < tensors->num_tensors; ++i) {
      // Free the shape array of each tensor if not NULL
      if (tensors->shapes[i] != NULL) {
        free(tensors->shapes[i]);
      }
    }
    // Free the shapes array
    free(tensors->shapes);
  } else {
    // Print a warning if shapes is NULL
    printf("Warning: shapes array is NULL\n");
  }

  // Check if the data array is NULL
  if (tensors->data != NULL) {
    // Iterate through each tensor
    for (size_t i = 0; i < tensors->num_tensors; ++i) {
      // Free the data of each tensor if not NULL
      if (tensors->data[i] != NULL) {
        free(tensors->data[i]);
      }
    }
    // Free the data array
    free(tensors->data);
  } else {
    // Print a warning if data is NULL
    printf("Warning: data array is NULL\n");
  }

  // Check if the data_types array is NULL
  if (tensors->data_types != NULL) {
    // Free the data_types array
    free(tensors->data_types);
  } else {
    // Print a warning if data_types is NULL
    printf("Warning: data_types array is NULL\n");
  }

  // Check if the ranks array is NULL
  if (tensors->ranks != NULL) {
    // Free the ranks array
    free(tensors->ranks);
  } else {
    // Print a warning if ranks is NULL
    printf("Warning: ranks array is NULL\n");
  }

  // Free the tensors struct itself
  free(tensors);
}

void shallow_free_tensors_struct(tensors_struct* tensors) {
  // Check if tensors is NULL
  if (tensors == NULL) {
    printf("Warning: tensors is NULL\n");
    return;
  }
  // check if the names array is NULL
  if (tensors->names != NULL) {
    // Free the names array
    free(tensors->names);
  } else {
    // Print a warning if names is NULL
    printf("Warning: names array is NULL\n");
  }

  // Check if the shapes array is NULL
  if (tensors->shapes != NULL) {
    // Free the shapes array
    free(tensors->shapes);
  } else {
    // Print a warning if shapes is NULL
    printf("Warning: shapes array is NULL\n");
  }

  // Check if the data array is NULL
  if (tensors->data != NULL) {
    // Free the data array
    free(tensors->data);
  } else {
    // Print a warning if data is NULL
    printf("Warning: data array is NULL\n");
  }

  // Check if the data_types array is NULL
  if (tensors->data_types != NULL) {
    // Free the data_types array
    free(tensors->data_types);
  } else {
    // Print a warning if data_types is NULL
    printf("Warning: data_types array is NULL\n");
  }

  // Check if the ranks array is NULL
  if (tensors->ranks != NULL) {
    // Free the ranks array
    free(tensors->ranks);
  } else {
    // Print a warning if ranks is NULL
    printf("Warning: ranks array is NULL\n");
  }

  // Free the tensors struct itself
  free(tensors);
}

tensors_struct* deep_copy_tensors_struct(const tensors_struct* src) {
  // Check if src is NULL
  if (src == NULL) {
    printf("Warning: src is NULL\n");
    return NULL;
  }

  // Allocate memory for the new tensors_struct
  tensors_struct* dst = (tensors_struct*)malloc(sizeof(tensors_struct));
  if (dst == NULL) {
    printf("Error: Memory allocation failed for tensors_struct\n");
    return NULL;
  }

  // Copy the number of tensors
  dst->num_tensors = src->num_tensors;

  // Allocate memory for names
  dst->names = (char**)malloc(dst->num_tensors * sizeof(char*));
  if (dst->names == NULL) {
    printf("Error: Memory allocation failed for names\n");
    free(dst);
    return NULL;
  }
  // Allocate memory for data_types
  dst->data_types =
      (tensor_data_type*)malloc(dst->num_tensors * sizeof(tensor_data_type));
  if (dst->data_types == NULL) {
    printf("Error: Memory allocation failed for data_types\n");
    free(dst->names);
    free(dst);
    return NULL;
  }

  // Allocate memory for ranks
  dst->ranks = (size_t*)malloc(dst->num_tensors * sizeof(size_t));
  if (dst->ranks == NULL) {
    printf("Error: Memory allocation failed for ranks\n");
    free(dst->data_types);
    free(dst->names);
    free(dst);
    return NULL;
  }
  // Allocate memory for shapes
  dst->shapes = (size_t**)malloc(dst->num_tensors * sizeof(size_t*));
  if (dst->shapes == NULL) {
    printf("Error: Memory allocation failed for shapes\n");
    free(dst->ranks);
    free(dst->data_types);
    free(dst->names);
    free(dst);
    return NULL;
  }
  // Allocate memory for data
  dst->data = (void**)malloc(dst->num_tensors * sizeof(void*));
  if (dst->data == NULL) {
    printf("Error: Memory allocation failed for data\n");
    free(dst->shapes);
    free(dst->ranks);
    free(dst->data_types);
    free(dst->names);
    free(dst);
    return NULL;
  }
  // Iterate through each tensor
  for (size_t i = 0; i < dst->num_tensors; ++i) {
    // Copy the name of the tensor
    dst->names[i] = strdup(src->names[i]);

    // Copy the data type
    dst->data_types[i] = src->data_types[i];

    // Copy the rank
    dst->ranks[i] = src->ranks[i];

    // Allocate memory for shapes
    dst->shapes[i] = (size_t*)malloc(src->ranks[i] * sizeof(size_t));
    // Copy the shape
    memcpy(dst->shapes[i], src->shapes[i], src->ranks[i] * sizeof(size_t));

    // Allocate memory for data
    size_t data_size = get_data_type_byte_size(src->data_types[i]);
    size_t total_size = data_size;
    for (size_t j = 0; j < src->ranks[i]; ++j) {
      total_size *= src->shapes[i][j];
    }
    dst->data[i] = malloc(total_size);
    // Copy the data
    memcpy(dst->data[i], src->data[i], total_size);
  }
  return dst;
}

void shallow_copy_tensors_struct(const tensors_struct* src,
                                 tensors_struct* dst) {
  // Check if src or dst is NULL
  if (src == NULL || dst == NULL) {
    printf("Warning: src or dst is NULL\n");
    return;
  }

  // Ensure dst & src have the same number of tensors
  if (src->num_tensors != dst->num_tensors) {
    printf("Error: src and dst have different number of tensors\n");
    return;
  }

  // Iterate through each tensor
  for (size_t i = 0; i < src->num_tensors; ++i) {
    // Copy the name of the tensor
    dst->names[i] = src->names[i];

    // Copy the data type
    dst->data_types[i] = src->data_types[i];

    // Copy the rank
    dst->ranks[i] = src->ranks[i];

    // Copy the shape
    dst->shapes[i] = src->shapes[i];

    // Copy the data
    dst->data[i] = src->data[i];
  }
}

bool compare_two_tensors_structs(const tensors_struct* tensors1,
                                 const tensors_struct* tensors2) {
  // Check if both tensors_structs are NULL
  if (tensors1 == NULL && tensors2 == NULL) {
    return true;  // Both are NULL, considered equal
  }
  // Check if one of them is NULL
  if (tensors1 == NULL || tensors2 == NULL) {
    printf("Warning: One of the tensors_structs is NULL\n");
    return false;  // One is NULL, considered not equal
  }
  // Check if the number of tensors is equal
  if (tensors1->num_tensors != tensors2->num_tensors) {
    printf("Warning: Number of tensors is different\n");
    return false;  // Different number of tensors
  }
  // Iterate through each tensor
  for (int i = 0; i < tensors1->num_tensors; ++i) {
    // Check if names are equal
    if (strcmp(tensors1->names[i], tensors2->names[i]) != 0) {
      printf("Warning: Tensor names at %d are different\n", i);
      return false;  // Different names
    }
    // Check if data types are equal
    if (tensors1->data_types[i] != tensors2->data_types[i]) {
      printf("Warning: Tensor data types at %d are different\n", i);
      return false;  // Different data types
    }
    // Check if ranks are equal
    if (tensors1->ranks[i] != tensors2->ranks[i]) {
      printf("Warning: Tensor ranks at %d are different\n", i);
      return false;  // Different ranks
    }
    // Check if shapes are equal
    for (size_t j = 0; j < tensors1->ranks[i]; ++j) {
      if (tensors1->shapes[i][j] != tensors2->shapes[i][j]) {
        printf("Warning: Tensor shapes at %d are different\n", i);
        return false;  // Different shapes
      }
    }
    // Check if data is equal
    size_t data_size = get_data_type_byte_size(tensors1->data_types[i]);
    size_t total_size = data_size;
    for (size_t j = 0; j < tensors1->ranks[i]; ++j) {
      total_size *= tensors1->shapes[i][j];
    }
    if (memcmp(tensors1->data[i], tensors2->data[i], total_size) != 0) {
      for (int k = 0; k < total_size / data_size; ++k) {
        printf("%f - %f\n", ((float*)tensors1->data[i])[k],
               ((float*)tensors2->data[i])[k]);
      }
      return false;  // Different data
    }
  }
  return true;  // All checks passed, tensors_structs are equal
}

tensors_struct* create_sample_tensors_struct(int seed) {
  // ensure seed is non-negative
  if (seed < 0) {
    printf("Warning: Seed is negative, setting to 0\n");
    seed = 0;
  }
  // Create a sample tensors_struct with dummy data
  tensors_struct* t = (tensors_struct*)malloc(sizeof(tensors_struct));
  if (t == NULL) {
    printf("Error: Memory allocation failed for tensors_struct\n");
    return NULL;
  }
  t->num_tensors = 2;
  t->names = (char**)malloc(2 * sizeof(char*));
  t->names[0] = strdup("tensor1");
  t->names[1] = strdup("tensor2");

  t->data_types = (tensor_data_type*)malloc(2 * sizeof(tensor_data_type));
  t->data_types[0] = DATA_TYPE_FLOAT;
  t->data_types[1] = DATA_TYPE_INT32;

  t->ranks = (size_t*)malloc(2 * sizeof(size_t));
  t->ranks[0] = 2;
  t->ranks[1] = 1;

  t->shapes = (size_t**)malloc(2 * sizeof(size_t*));
  t->shapes[0] = (size_t*)malloc(2 * sizeof(size_t));
  t->shapes[0][0] = 1 + 2 * seed;
  t->shapes[0][1] = 3 + seed;
  t->shapes[1] = (size_t*)malloc(1 * sizeof(size_t));
  t->shapes[1][0] = 5 + 2 * seed;

  t->data = (void**)malloc(2 * sizeof(void*));
  t->data[0] = malloc(t->shapes[0][0] * t->shapes[0][1] * sizeof(float));
  t->data[1] = malloc(t->shapes[1][0] * sizeof(int32_t));
  float* d0 = (float*)t->data[0];
  for (int i = 0; i < t->shapes[0][0] * t->shapes[0][1]; ++i)
    d0[i] = (float)i * seed / 10.34343f;
  int32_t* d1 = (int32_t*)t->data[1];
  for (int i = 0; i < t->shapes[1][0]; ++i) d1[i] = i * 10 + 5 * seed;

  return t;
}
