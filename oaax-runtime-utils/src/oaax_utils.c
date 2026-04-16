// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#include "oaax_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------------------------------
// Data type helpers
// ---------------------------------------------------------------------------

const char *data_type_string(TensorElementType data_type) {
    switch (data_type) {
        case DATA_TYPE_UNDEFINED:      return "DATA_TYPE_UNDEFINED";
        case DATA_TYPE_FLOAT:          return "DATA_TYPE_FLOAT";
        case DATA_TYPE_UINT8:          return "DATA_TYPE_UINT8";
        case DATA_TYPE_INT8:           return "DATA_TYPE_INT8";
        case DATA_TYPE_UINT16:         return "DATA_TYPE_UINT16";
        case DATA_TYPE_INT16:          return "DATA_TYPE_INT16";
        case DATA_TYPE_INT32:          return "DATA_TYPE_INT32";
        case DATA_TYPE_INT64:          return "DATA_TYPE_INT64";
        case DATA_TYPE_STRING:         return "DATA_TYPE_STRING";
        case DATA_TYPE_BOOL:           return "DATA_TYPE_BOOL";
        case DATA_TYPE_FLOAT16:        return "DATA_TYPE_FLOAT16";
        case DATA_TYPE_DOUBLE:         return "DATA_TYPE_DOUBLE";
        case DATA_TYPE_UINT32:         return "DATA_TYPE_UINT32";
        case DATA_TYPE_UINT64:         return "DATA_TYPE_UINT64";
        case DATA_TYPE_COMPLEX64:      return "DATA_TYPE_COMPLEX64";
        case DATA_TYPE_COMPLEX128:     return "DATA_TYPE_COMPLEX128";
        case DATA_TYPE_BFLOAT16:       return "DATA_TYPE_BFLOAT16";
        case DATA_TYPE_FLOAT8E4M3FN:   return "DATA_TYPE_FLOAT8E4M3FN";
        case DATA_TYPE_FLOAT8E4M3FNUZ: return "DATA_TYPE_FLOAT8E4M3FNUZ";
        case DATA_TYPE_FLOAT8E5M2:     return "DATA_TYPE_FLOAT8E5M2";
        case DATA_TYPE_FLOAT8E5M2FNUZ: return "DATA_TYPE_FLOAT8E5M2FNUZ";
        case DATA_TYPE_UINT4:          return "DATA_TYPE_UINT4";
        case DATA_TYPE_INT4:           return "DATA_TYPE_INT4";
        case DATA_TYPE_FLOAT4E2M1:     return "DATA_TYPE_FLOAT4E2M1";
        case DATA_TYPE_FLOAT8E8M0:     return "DATA_TYPE_FLOAT8E8M0";
        case DATA_TYPE_UINT2:          return "DATA_TYPE_UINT2";
        case DATA_TYPE_INT2:           return "DATA_TYPE_INT2";
        default:                       return "DATA_TYPE_UNDEFINED";
    }
}

// ---------------------------------------------------------------------------
// Data size helpers
// ---------------------------------------------------------------------------

size_t element_byte_size(TensorElementType data_type) {
    switch (data_type) {
        case DATA_TYPE_BOOL:
        case DATA_TYPE_INT8:
        case DATA_TYPE_UINT8:
        case DATA_TYPE_FLOAT8E4M3FN:
        case DATA_TYPE_FLOAT8E4M3FNUZ:
        case DATA_TYPE_FLOAT8E5M2:
        case DATA_TYPE_FLOAT8E5M2FNUZ:
        case DATA_TYPE_FLOAT8E8M0:    return 1;
        case DATA_TYPE_FLOAT16:
        case DATA_TYPE_BFLOAT16:
        case DATA_TYPE_INT16:
        case DATA_TYPE_UINT16:        return 2;
        case DATA_TYPE_FLOAT:
        case DATA_TYPE_INT32:
        case DATA_TYPE_UINT32:        return 4;
        case DATA_TYPE_DOUBLE:
        case DATA_TYPE_INT64:
        case DATA_TYPE_UINT64:
        case DATA_TYPE_COMPLEX64:     return 8;
        case DATA_TYPE_COMPLEX128:    return 16;
        // sub-byte types: caller must use compute_data_size()
        case DATA_TYPE_INT4:
        case DATA_TYPE_UINT4:
        case DATA_TYPE_FLOAT4E2M1:
        case DATA_TYPE_INT2:
        case DATA_TYPE_UINT2:
        // string and undefined: no fixed size
        case DATA_TYPE_STRING:
        case DATA_TYPE_UNDEFINED:
        default:                      return 0;
    }
}

size_t compute_data_size(TensorElementType data_type, int rank, const int *shape) {
    if (rank < 0 || (rank > 0 && shape == NULL))
        return 0;

    size_t num_elements = 1;
    for (int i = 0; i < rank; i++) {
        if (shape[i] <= 0)
            return 0;
        num_elements *= (size_t)shape[i];
    }

    switch (data_type) {
        // sub-byte types need special packing arithmetic
        case DATA_TYPE_INT4:
        case DATA_TYPE_UINT4:
        case DATA_TYPE_FLOAT4E2M1:   return (num_elements + 1) / 2; // 2 per byte
        case DATA_TYPE_INT2:
        case DATA_TYPE_UINT2:        return (num_elements + 3) / 4; // 4 per byte
        case DATA_TYPE_UNDEFINED:
        case DATA_TYPE_STRING:       return 0;
        default: {
            size_t esz = element_byte_size(data_type);
            return esz == 0 ? 0 : num_elements * esz;
        }
    }
}

// ---------------------------------------------------------------------------
// Tensor lifecycle
// ---------------------------------------------------------------------------

Tensors *tensors_alloc(int num_tensors) {
    if (num_tensors < 0)
        return NULL;

    Tensors *t = (Tensors *)calloc(1, sizeof(Tensors));
    if (t == NULL)
        return NULL;

    t->num_tensors = num_tensors;

    if (num_tensors == 0)
        return t;

    t->tensors = (TensorDescriptor *)calloc((size_t)num_tensors, sizeof(TensorDescriptor));
    if (t->tensors == NULL) {
        free(t);
        return NULL;
    }

    return t;
}

void tensors_free(Tensors *tensors) {
    if (tensors == NULL)
        return;

    if (tensors->tensors != NULL) {
        for (int i = 0; i < tensors->num_tensors; i++) {
            free(tensors->tensors[i].name);
            free(tensors->tensors[i].shape);
            free(tensors->tensors[i].data);
        }
        free(tensors->tensors);
    }

    free(tensors);
}

Tensors *tensors_deep_copy(const Tensors *src) {
    if (src == NULL)
        return NULL;

    Tensors *dst = tensors_alloc(src->num_tensors);
    if (dst == NULL)
        return NULL;

    dst->id = src->id;

    for (int i = 0; i < src->num_tensors; i++) {
        const TensorDescriptor *s = &src->tensors[i];
        TensorDescriptor       *d = &dst->tensors[i];

        d->data_type = s->data_type;
        d->rank      = s->rank;
        d->data_size = s->data_size;

        if (s->name != NULL) {
            d->name = strdup(s->name);
            if (d->name == NULL)
                goto oom;
        }

        if (s->rank > 0 && s->shape != NULL) {
            d->shape = (int *)malloc((size_t)s->rank * sizeof(int));
            if (d->shape == NULL)
                goto oom;
            memcpy(d->shape, s->shape, (size_t)s->rank * sizeof(int));
        }

        if (s->data_size > 0 && s->data != NULL) {
            d->data = malloc(s->data_size);
            if (d->data == NULL)
                goto oom;
            memcpy(d->data, s->data, s->data_size);
        }
    }

    return dst;

oom:
    tensors_free(dst);
    return NULL;
}

void tensors_set(Tensors *tensors, int index,
                 const char *name,
                 TensorElementType data_type,
                 int rank, const int *shape,
                 void *data, size_t data_size) {
    if (tensors == NULL || index < 0 || index >= tensors->num_tensors)
        return;

    TensorDescriptor *d = &tensors->tensors[index];

    // free any previously set fields
    free(d->name);
    free(d->shape);

    d->data_type = data_type;
    d->rank      = rank;
    d->data_size = data_size;
    d->data      = data;

    d->name = (name != NULL) ? strdup(name) : NULL;

    if (rank > 0 && shape != NULL) {
        d->shape = (int *)malloc((size_t)rank * sizeof(int));
        if (d->shape != NULL)
            memcpy(d->shape, shape, (size_t)rank * sizeof(int));
    } else {
        d->shape = NULL;
    }
}

const TensorDescriptor *tensors_find(const Tensors *tensors, const char *name) {
    if (tensors == NULL || name == NULL)
        return NULL;
    for (int i = 0; i < tensors->num_tensors; i++) {
        if (tensors->tensors[i].name != NULL &&
            strcmp(tensors->tensors[i].name, name) == 0)
            return &tensors->tensors[i];
    }
    return NULL;
}

bool tensors_validate(const Tensors *tensors) {
    if (tensors == NULL) {
        printf("tensors_validate: NULL pointer\n");
        return false;
    }
    if (tensors->num_tensors < 0) {
        printf("tensors_validate: num_tensors is negative (%d)\n", tensors->num_tensors);
        return false;
    }

    for (int i = 0; i < tensors->num_tensors; i++) {
        const TensorDescriptor *d = &tensors->tensors[i];

        if (d->name == NULL || d->name[0] == '\0')
            printf("tensors_validate: WARNING descriptor %d has no name\n", i);

        if (d->rank < 0) {
            printf("tensors_validate: descriptor %d has negative rank (%d)\n", i, d->rank);
            return false;
        }
        if (d->rank > 0 && d->shape == NULL) {
            printf("tensors_validate: descriptor %d has rank %d but shape is NULL\n", i, d->rank);
            return false;
        }
        for (int j = 0; j < d->rank; j++) {
            if (d->shape[j] <= 0) {
                printf("tensors_validate: descriptor %d shape[%d] is %d (must be > 0)\n", i, j, d->shape[j]);
                return false;
            }
        }
        if (d->data_size > 0 && d->data == NULL) {
            printf("tensors_validate: descriptor %d has data_size %zu but data is NULL\n", i, d->data_size);
            return false;
        }
    }

    // warn on duplicate names
    for (int i = 0; i < tensors->num_tensors; i++) {
        if (tensors->tensors[i].name == NULL)
            continue;
        for (int j = i + 1; j < tensors->num_tensors; j++) {
            if (tensors->tensors[j].name != NULL &&
                strcmp(tensors->tensors[i].name, tensors->tensors[j].name) == 0)
                printf("tensors_validate: WARNING descriptors %d and %d share name '%s'\n",
                       i, j, tensors->tensors[i].name);
        }
    }

    return true;
}

bool tensors_compare(const Tensors *a, const Tensors *b) {
    if (a == NULL || b == NULL)
        return false;
    if (a->id != b->id || a->num_tensors != b->num_tensors)
        return false;

    for (int i = 0; i < a->num_tensors; i++) {
        const TensorDescriptor *ta = &a->tensors[i];
        const TensorDescriptor *tb = &b->tensors[i];

        if (ta->data_type != tb->data_type)
            return false;
        if (ta->rank != tb->rank)
            return false;
        if (ta->data_size != tb->data_size)
            return false;

        // compare names
        if (ta->name != tb->name) {
            if (ta->name == NULL || tb->name == NULL)
                return false;
            if (strcmp(ta->name, tb->name) != 0)
                return false;
        }

        // compare shapes
        for (int j = 0; j < ta->rank; j++) {
            if (ta->shape[j] != tb->shape[j])
                return false;
        }

        // compare data
        if (ta->data_size > 0) {
            if (ta->data == NULL || tb->data == NULL)
                return false;
            if (memcmp(ta->data, tb->data, ta->data_size) != 0)
                return false;
        }
    }

    return true;
}

void tensors_print(const Tensors *tensors) {
    if (tensors == NULL) {
        printf("Tensors: NULL\n");
        return;
    }
    printf("Tensors { id=%d, num_tensors=%d }\n", tensors->id, tensors->num_tensors);
    for (int i = 0; i < tensors->num_tensors; i++) {
        const TensorDescriptor *d = &tensors->tensors[i];
        printf("  [%d] name='%s'  type=%s  rank=%d  data_size=%zu  shape=[",
               i,
               d->name ? d->name : "(null)",
               data_type_string(d->data_type),
               d->rank,
               d->data_size);
        for (int j = 0; j < d->rank; j++) {
            if (j > 0) printf(", ");
            printf("%d", d->shape ? d->shape[j] : 0);
        }
        printf("]  data=%p\n", d->data);
    }
}

// ---------------------------------------------------------------------------
// Config helpers
// ---------------------------------------------------------------------------

Config *config_alloc(void) {
    return (Config *)calloc(1, sizeof(Config));
}

void config_free(Config *c) {
    if (c == NULL)
        return;
    char **keys   = (char **)c->keys;
    char **values = (char **)c->values;
    for (int i = 0; i < c->length; i++) {
        free(keys[i]);
        free(values[i]);
    }
    free(keys);
    free(values);
    free(c);
}

bool config_set(Config *c, const char *key, const char *value) {
    if (c == NULL || key == NULL)
        return false;

    char **keys   = (char **)c->keys;
    char **values = (char **)c->values;

    // Update existing key
    for (int i = 0; i < c->length; i++) {
        if (strcmp(keys[i], key) == 0) {
            char *v = strdup(value != NULL ? value : "");
            if (v == NULL)
                return false;
            free(values[i]);
            values[i] = v;
            return true;
        }
    }

    // New entry — duplicate strings first, then grow arrays
    char *k = strdup(key);
    char *v = strdup(value != NULL ? value : "");
    if (k == NULL || v == NULL) {
        free(k);
        free(v);
        return false;
    }

    int    new_len    = c->length + 1;
    char **new_keys   = (char **)realloc(keys,   (size_t)new_len * sizeof(char *));
    char **new_values = (char **)realloc(values, (size_t)new_len * sizeof(char *));

    if (new_keys   != NULL) c->keys   = (const char **)new_keys;
    if (new_values != NULL) c->values = (const char **)new_values;

    if (new_keys == NULL || new_values == NULL) {
        free(k);
        free(v);
        return false;
    }

    new_keys[c->length]   = k;
    new_values[c->length] = v;
    c->length = new_len;
    return true;
}

const char *config_get(const Config *c, const char *key) {
    if (c == NULL || key == NULL)
        return NULL;
    for (int i = 0; i < c->length; i++) {
        if (strcmp(c->keys[i], key) == 0)
            return c->values[i];
    }
    return NULL;
}


bool config_delete(Config *c, const char *key) {
    if (c == NULL || key == NULL)
        return false;
    char **keys   = (char **)c->keys;
    char **values = (char **)c->values;
    for (int i = 0; i < c->length; i++) {
        if (strcmp(keys[i], key) == 0) {
            free(keys[i]);
            free(values[i]);
            for (int j = i; j < c->length - 1; j++) {
                keys[j]   = keys[j + 1];
                values[j] = values[j + 1];
            }
            c->length--;
            return true;
        }
    }
    return false;
}

void config_print(const Config *c) {
    if (c == NULL) {
        printf("Config: NULL\n");
        return;
    }
    printf("Config { length=%d }\n", c->length);
    for (int i = 0; i < c->length; i++)
        printf("  [%d] '%s' = '%s'\n", i, c->keys[i], c->values[i]);
}

// ---------------------------------------------------------------------------
// Status helper
// ---------------------------------------------------------------------------

const char *runtime_status_string(RuntimeStatus status) {
    switch (status) {
        case RUNTIME_STATUS_SUCCESS:              return "RUNTIME_STATUS_SUCCESS";
        case RUNTIME_STATUS_ERROR:                return "RUNTIME_STATUS_ERROR";
        case RUNTIME_STATUS_NOT_INITIALIZED:      return "RUNTIME_STATUS_NOT_INITIALIZED";
        case RUNTIME_STATUS_ALREADY_INITIALIZED:  return "RUNTIME_STATUS_ALREADY_INITIALIZED";
        case RUNTIME_STATUS_MODEL_NOT_LOADED:     return "RUNTIME_STATUS_MODEL_NOT_LOADED";
        case RUNTIME_STATUS_INVALID_ARGUMENT:     return "RUNTIME_STATUS_INVALID_ARGUMENT";
        case RUNTIME_STATUS_INVALID_MODEL:        return "RUNTIME_STATUS_INVALID_MODEL";
        case RUNTIME_STATUS_FILE_NOT_FOUND:       return "RUNTIME_STATUS_FILE_NOT_FOUND";
        case RUNTIME_STATUS_OUT_OF_MEMORY:        return "RUNTIME_STATUS_OUT_OF_MEMORY";
        case RUNTIME_STATUS_INVALID_TENSOR:       return "RUNTIME_STATUS_INVALID_TENSOR";
        case RUNTIME_STATUS_TENSOR_SHAPE_MISMATCH:return "RUNTIME_STATUS_TENSOR_SHAPE_MISMATCH";
        case RUNTIME_STATUS_TENSOR_TYPE_MISMATCH: return "RUNTIME_STATUS_TENSOR_TYPE_MISMATCH";
        case RUNTIME_STATUS_NO_OUTPUT_AVAILABLE:  return "RUNTIME_STATUS_NO_OUTPUT_AVAILABLE";
        case RUNTIME_STATUS_INFERENCE_ERROR:      return "RUNTIME_STATUS_INFERENCE_ERROR";
        case RUNTIME_STATUS_NOT_IMPLEMENTED:      return "RUNTIME_STATUS_NOT_IMPLEMENTED";
        case RUNTIME_STATUS_TIMEOUT:              return "RUNTIME_STATUS_TIMEOUT";
        case RUNTIME_STATUS_DEVICE_ERROR:         return "RUNTIME_STATUS_DEVICE_ERROR";
        case RUNTIME_STATUS_UNKNOWN_ERROR:        return "RUNTIME_STATUS_UNKNOWN_ERROR";
        case RUNTIME_STATUS_INVALID_MODEL_ID:     return "RUNTIME_STATUS_INVALID_MODEL_ID";
        default:                                  return "RUNTIME_STATUS_UNKNOWN_ERROR";
    }
}
