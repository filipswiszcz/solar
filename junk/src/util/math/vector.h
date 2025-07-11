#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

#define vec4(x, y, z, w) ((vec4) {x, y, z, w})

#define vec3(x, y, z) ((vec3) {x, y, z})
#define vec3_mul(v, s) ((vec3) {(v).x * (s), (v).y * (s), (v).z * (s)})
#define vec3_add(a, b) ((vec3) {a.x + b.x, a.y + b.y, a.z + b.z})
#define vec3_sub(a, b) ((vec3) {a.x - b.x, a.y - b.y, a.z - b.z})

typedef struct vec4 {
    float x, y, z, w;
} vec4;

typedef struct vec3 {
    float x, y, z;
} vec3;

float radians(float degrees);

vec3 normalize(const vec3 v);

vec3 cross(const vec3 a, const vec3 b);

float dot(const vec3 a, const vec3 b);

#endif