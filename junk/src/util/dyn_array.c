#include "util/dyn_array.h"

// void hash_table_add(hash_table_t *tab, char *key, shape_t val) {}

void farray_add(farray_t *arr, float val) {
    do {
        if (arr -> k >= arr -> capacity) {
            if (arr -> capacity == 0) arr -> capacity = 64;
            else arr -> capacity *= 2;
            arr -> values = (float*) realloc(arr -> values, arr -> capacity * sizeof(*arr -> values));
        }
        arr -> values[arr -> k++] = val;
    } while (0);
}

void iarray_add(iarray_t *arr, int val) {
    do {
        if (arr -> k >= arr -> capacity) {
            if (arr -> capacity == 0) arr -> capacity = 64;
            else arr -> capacity *= 2;
            arr -> values = (int*) realloc(arr -> values, arr -> capacity * sizeof(*arr -> values));
        }
        arr -> values[arr -> k++] = val;
    } while (0);
}