#ifndef __R_MATH_H__
#define __R_MATH_H__

#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define R_PI 3.14159265358979323846

// vec2
#define vec2(x, y) ((vec2_t) {x, y})
#define vec2_add(a, b) ((vec2_t) {a.x + b.x, a.y + b.y})
#define vec2_sub(a, b) ((vec2_t) {a.x - b.x, a.y - b.y})
#define vec2_mul(v, s) ((vec2_t) {(v).x * (s), (v).y * (s)})
// vec3
#define vec3(x, y, z) ((vec3_t) {x, y, z})
#define vec3_add(a, b) ((vec3_t) {a.x + b.x, a.y + b.y, a.z + b.z})
#define vec3_sub(a, b) ((vec3_t) {a.x - b.x, a.y - b.y, a.z - b.z})
#define vec3_mul(v, s) ((vec3_t) {(v).x * (s), (v).y * (s), (v).z * (s)})
// vec4
#define vec4(x, y, z, w) ((vec4_t) {x, y, z, w})
#define vec4_add(a, b) ((vec4_t) {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w})
#define vec4_sub(a, b) ((vec4_t) {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w})
#define vec4_mul(v, s) ((vec4_t) {(v).x * (s), (v).y * (s), (v).z * (s), (v).w * (s)})

typedef struct {
    float x, y;
} vec2_t;

typedef struct {
    float x, y, z;
} vec3_t;

typedef struct {
    float x, y, z, w;
} vec4_t;

float r_radians(float degrees);

vec3_t r_normalize(const vec3_t v);

vec3_t r_cross(const vec3_t a, const vec3_t b);

float r_dot(const vec3_t a, const vec3_t b);

// mat4
#define mat4(v) ((mat4_t) {.m = { \
    {v, 0, 0, 0}, \
    {0, v, 0, 0}, \
    {0, 0, v, 0}, \
    {0, 0, 0, v} \
}})

typedef struct {
    float m[4][4];
} mat4_t;

void __r_mat4_read(mat4_t m);

mat4_t r_orthographic(float l, float r, float b, float t, float znear, float zfar);

mat4_t r_perspective(float fovy, float aspect, float znear, float zfar);

mat4_t r_translate(mat4_t m, vec3_t v);

mat4_t r_rotate(mat4_t m, float angle, vec3_t v);

mat4_t r_scale(mat4_t m, vec3_t s);

mat4_t r_look_at(vec3_t pos, vec3_t target, vec3_t up);

// quat
typedef vec4_t quat_t;

mat4_t r_rotate_quat(mat4_t m, quat_t q);

#endif // !__R_MATH_H__