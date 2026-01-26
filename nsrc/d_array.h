#ifndef __D_ARRAY_H__
#define __D_ARRAY_H__

#include <stdint.h>
#include <stdlib.h>

// MATH

typedef struct float_array {
    float *values;
    size_t size, capacity;
} float_array_t;

void d_float_array_insert(float_array_t *array, float value);

typedef struct uint32_array {
    uint32_t *values;
    size_t size, capacity;
} uint32_array_t;

void d_uint32_array_insert(uint32_array_t *array, uint32_t value);

// void d_uint32_array_insert_all(uint32_array_t *array, uint32_t *values);

// MESH

// typedef struct vertex vertex_t;

// typedef struct vertex_array {
//     vertex_t *values;
//     size_t size, capacity;
// } vertex_array_t;

// void d_vertex_array_insert(vertex_array_t *array, vertex_t value);

// typedef struct mesh mesh_t;

// typedef struct mesh_array {
//     mesh_t *values;
//     size_t size, capacity;
// } mesh_array_t;

// void d_mesh_array_insert(mesh_array_t *array, mesh_t value);

#endif // !__D_ARRAY_H__