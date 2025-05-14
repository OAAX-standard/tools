#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tensors_struct.h"


void test_deep_and_shallow_copy_and_compare() {
    tensors_struct* t1 = create_sample_tensors_struct(0);
    tensors_struct* t2 = deep_copy_tensors_struct(t1);
    printf("Comparing original and deep copied tensors_struct...\n");
    assert(compare_two_tensors_structs(t1, t2) == true);

    // Mutate t2 and check inequality
    ((float*)t2->data[0])[0] = 999.0f;
    printf("Comparing original and mutated deep copied tensors_struct...\n");
    assert(compare_two_tensors_structs(t1, t2) == false);

    // Restore and check equality again
    ((float*)t2->data[0])[0] = 0.0f;
    printf("Restoring t2 and comparing again...\n");
    assert(compare_two_tensors_structs(t1, t2) == true);

    // Test shallow copy
    tensors_struct t3;
    t3.num_tensors = t1->num_tensors;
    t3.names = (char**)malloc(2 * sizeof(char*));
    t3.data_types = (tensor_data_type*)malloc(2 * sizeof(tensor_data_type));
    t3.ranks = (size_t*)malloc(2 * sizeof(size_t));
    t3.shapes = (size_t**)malloc(2 * sizeof(size_t*));
    t3.data = (void**)malloc(2 * sizeof(void*));
    shallow_copy_tensors_struct(t1, &t3);
    printf("Comparing original and shallow copied tensors_struct...\n");
    compare_two_tensors_structs(t1, &t3);
    // Clean up
    free(t3.names);
    free(t3.data_types);
    free(t3.ranks);
    free(t3.shapes);
    free(t3.data);

    deep_free_tensors_struct(t1);
    deep_free_tensors_struct(t2);
}

void test_tensors_struct_print_metadata() {
    tensors_struct* t = create_sample_tensors_struct(3);
    print_tensors_metadata(t);
    // deep_free_tensors_struct(t);
}

int test_tensors_struct_main() {
    printf("Running tests for tensors_struct...\n");
    test_deep_and_shallow_copy_and_compare();
    test_tensors_struct_print_metadata();
    printf("All tests passed!\n");
    return 0;
}
