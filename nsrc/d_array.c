#include "d_array.h"
#include "r_renderer.h"

// float dynamic array

void d_float_array_insert(float_array_t *array, float value) {
    if (array -> size >= array -> capacity) {
        if (array -> capacity == 0) array -> capacity = 256;
        else array -> capacity *= 2;
        array -> values = (float*) realloc(array -> values, array -> capacity * sizeof(*array -> values));
    }
    array -> values[array -> size++] = value;
}

// uint32_t dynamic array

void d_uint32_array_insert(uint32_array_t *array, uint32_t value) {
    if (array -> size >= array -> capacity) {
        if (array -> capacity == 0) array -> capacity = 256;
        else array -> capacity *= 2;
        array -> values = (uint32_t*) realloc(array -> values, array -> capacity * sizeof(*array -> values));
    }
    array -> values[array -> size++] = value;
}

// vertex dynamic array

void d_vertex_array_insert(vertex_array_t *array, vertex_t value) {
    if (array -> size >= array -> capacity) {
        if (array -> capacity == 0) array -> capacity = 256;
        else array -> capacity *= 2;
        array -> values = (vertex_t*) realloc(array -> values, array -> capacity * sizeof(*array -> values));
    }
    array -> values[array -> size++] = value;
}

// instance dynamic array

// void d_instance_array_insert(instance_array_t *array, instance_t value) {
//     if (array -> size >= array -> capacity) {
//         if (array -> capacity == 0) array -> capacity = 256;
//         else array -> capacity *= 2;
//         array -> values = (instance_t*) realloc(array -> values, array -> capacity * sizeof(*array -> values));
//     }
//     array -> values[array -> size++] = value;
// }
