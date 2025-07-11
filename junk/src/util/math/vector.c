#include "util/math/vector.h"

float radians(float degrees) {
    return degrees * 0.01745329251994329576923690768489;
}

vec3 normalize(const vec3 v) {
    float len = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
    if (len < 0.00001f) return vec3(0.0f, 0.0f, 0.0f);
    return vec3(v.x / len, v.y / len, v.z / len);
}

vec3 cross(const vec3 a, const vec3 b) {
    return vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

float dot(const vec3 a, const vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}