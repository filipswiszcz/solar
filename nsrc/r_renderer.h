#ifndef __R_RENDERER_H__
#define __R_RENDERER_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_GLCOREARB
    #include <GLFW/glfw3.h>
#else
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#endif

#include "d_array.h"
// #include "d_util.h"
#include "r_math.h"
#include "r_shader.h"

#define R_RENDERER_ASSETS_DIR_PATH "assets/"
#define R_RENDERER_ASSETS_MODEL_DIR_PATH "assets/model/"
#define R_RENDERER_ASSETS_TEXTURE_DIR_PATH "assets/texture/"

// OBJECT

typedef struct vertex {
    vec3_t position, normal;
    vec2_t uv;
} vertex_t;

typedef struct mesh {
    struct {
        vertex_t *values;
        uint32_t size;
    } vertices;
    struct {
        uint32_t *values;
        uint32_t size;
    } indices;
    uint32_t vao, vbo, ibo;
} mesh_t;

typedef struct material {
    float shininess;
    vec3_t ambient, diffuse, specular, emissivity;
    float density, transparency;
    uint32_t illumination;
} material_t;

typedef struct transform {
    vec3_t postion;
    quat_t rotation;
    vec3_t scale;
} transform_t;

typedef struct object {
    const char *filepath;
    mesh_t mesh;
    material_t material;
    transform_t transform;
    uint32_t texture;
    shader_t shader;
    uint8_t visible;
    void *params;
} object_t;

void r_renderer_object_read(object_t *object, char *filepath);

void r_renderer_object_upload(object_t *object);

void r_renderer_object_draw(object_t *object);

#endif // !__R_RENDERER_H__