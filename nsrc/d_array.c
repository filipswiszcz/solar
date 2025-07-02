#include "d_array.h"
#include "r_renderer.h"

// MATH

void d_float_array_insert(float_array_t *array, float value) {
    if (array -> size >= array -> capacity) {
        if (array -> capacity == 0) array -> capacity = 256;
        else array -> capacity *= 2;
        array -> values = (float*) realloc(array -> values, array -> capacity * sizeof(*array -> values));
    }
    array -> values[array -> size++] = value;
}

void d_uint32_array_insert(uint32_array_t *array, uint32_t value) {
    if (array -> size >= array -> capacity) {
        if (array -> capacity == 0) array -> capacity = 256;
        else array -> capacity *= 2;
        array -> values = (uint32_t*) realloc(array -> values, array -> capacity * sizeof(*array -> values));
    }
    array -> values[array -> size++] = value;
}

// void d_uint32_array_insert_all(uint32_array_t *array, uint32_t size, uint32_t *values) {
//     while (size <= array -> capacity) {
//         if (array -> capacity == 0) array -> capacity = 256;
//         else array -> capacity *=2;
//     }
//     // append vlaues
//     // append size
// }

// MESH

void d_vertex_array_insert(vertex_array_t *array, vertex_t value) {
    if (array -> size >= array -> capacity) {
        if (array -> capacity == 0) array -> capacity = 256;
        else array -> capacity *= 2;
        array -> values = (vertex_t*) realloc(array -> values, array -> capacity * sizeof(*array -> values));
    }
    array -> values[array -> size++] = value;
}

void d_mesh_array_insert(mesh_array_t *array, mesh_t value) {
    if (array -> size >= array -> capacity) {
        if (array -> capacity == 0) array -> capacity = 256;
        else array -> capacity *= 2;
        array -> values = (mesh_t*) realloc(array -> values, array -> capacity * sizeof(*array -> values));
    }
    array -> values[array -> size++] = value;
}
