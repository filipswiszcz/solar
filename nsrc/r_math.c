#include "r_math.h"

// vec

float r_radians(float degrees) {
    return degrees * 0.01745329251994329576923690768489;
}

vec3_t r_normalize(const vec3_t v) {
    float l = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
    if (l < 0.00001f) return vec3(0.0f, 0.0f, 0.0f);
    return vec3(v.x / l, v.y / l, v.z / l);
}

vec3_t r_cross(const vec3_t a, const vec3_t b) {
    return vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

float r_dot(const vec3_t a, const vec3_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// mat

void __r_mat4_read(mat4_t m) {
    for (int i = 0; i < 4; i++) {
        printf("[");
        for (int j = 0; j < 4; j++) {
            printf("%7.2f", m.m[i][j]);
            if (j < 3) printf(" ");
        }
        printf("]\n");
    }
    printf("\n");
}

mat4_t r_perspective(float fovy, float aspect, float znear, float zfar) {
    float rad = fovy;
    float tan_half_fovy = tan(rad / 2);

    mat4_t res = mat4(0);
    res.m[0][0] = 1.0f / (aspect * tan_half_fovy);
    res.m[1][1] = 1.0f / (tan_half_fovy);
    res.m[2][2] = -(zfar + znear) / (zfar - znear);
    res.m[2][3] = -1.0f;
    res.m[3][2] = -(2.0f * zfar * znear) / (zfar - znear);
    res.m[3][3] = 0.0f;

    return res;
}

mat4_t r_translate(mat4_t m, vec3_t v) {
    mat4_t res = m;
    res.m[3][0] += v.x;
    res.m[3][1] += v.y;
    res.m[3][2] += v.z;
    return res;
}

mat4_t r_rotate(mat4_t m, float angle, vec3_t v) {
    float const a = r_radians(angle);
    float const c = cos(a);
    float const s = sin(a);

    vec3_t axis = r_normalize(v);

    mat4_t rot = mat4(0);
    rot.m[0][0] = c + (1 - c) * axis.x * axis.x;
    rot.m[0][1] = (1 - c) * axis.x * axis.y - s * axis.z;
    rot.m[0][2] = (1 - c) * axis.x * axis.z + s * axis.y;

    rot.m[1][0] = (1 - c) * axis.y * axis.x + s * axis.z;
    rot.m[1][1] = c + (1 - c) * axis.y * axis.y;
    rot.m[1][2] = (1 - c) * axis.y * axis.z - s * axis.x;

    rot.m[2][0] = (1 - c) * axis.z * axis.x - s * axis.y;
    rot.m[2][1] = (1 - c) * axis.z * axis.y + s * axis.x;
    rot.m[2][2] = c + (1 - c) * axis.z * axis.z;

    rot.m[3][3] = 1.0f;

    mat4_t res = mat4(0);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res.m[i][j] = 0;
            for (int k = 0; k < 4; k++)
                res.m[i][j] += m.m[i][k] * rot.m[k][j];
        }
    }

    return res;
}

mat4_t r_look_at(vec3_t pos, vec3_t target, vec3_t up) {
    const vec3_t t = r_normalize(vec3_sub(target, pos));
    const vec3_t r = r_normalize(r_cross(t, up));
    const vec3_t u = r_normalize(r_cross(r, t));

    mat4_t res = mat4(1);
    res.m[0][0] = r.x;
    res.m[1][0] = r.y;
    res.m[2][0] = r.z;
    res.m[0][1] = u.x;
    res.m[1][1] = u.y;
    res.m[2][1] = u.z;
    res.m[0][2] = -t.x;
    res.m[1][2] = -t.y;
    res.m[2][2] = -t.z;
    res.m[3][0] = -r_dot(r, pos);
    res.m[3][1] = -r_dot(u, pos);
    res.m[3][2] = r_dot(t, pos);
    res.m[3][3] = 1.0f;

    return res;
}

// quat

mat4_t r_rotate_quat(mat4_t m, quat_t q) {
    mat4_t rot = mat4(0);
    rot.m[0][0] = 1 - 2 * (q.y * q.y + q.z * q.z);
    rot.m[0][1] = 2 * (q.x * q.y - q.z * q.w);
    rot.m[0][2] = 2 * (q.x * q.z + q.y * q.w);
    rot.m[0][3] = 0;

    rot.m[1][0] = 2 * (q.x * q.y + q.z * q.w);
    rot.m[1][1] = 1 - 2 * (q.x * q.x + q.z * q.z);
    rot.m[1][2] = 2 * (q.y * q.z - q.x * q.w);
    rot.m[1][3] = 0;

    rot.m[2][0] = 2 * (q.x * q.z - q.y * q.w);
    rot.m[2][1] = 2 * (q.y * q.z + q.x * q.w);
    rot.m[2][2] = 1 - 2 * (q.x * q.x + q.y * q.y);
    rot.m[2][3] = 0;

    rot.m[3][0] = 0;
    rot.m[3][1] = 0;
    rot.m[3][2] = 0;
    rot.m[3][3] = 1;

    mat4_t res = mat4(0);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res.m[i][j] = 0;
            for (int k = 0; k < 4; k++)
                res.m[i][j] += m.m[i][k] * rot.m[k][j];
        }
    }

    return res;
}