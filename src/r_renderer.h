#ifndef __R_RENDERER_H__
#define __R_RENDERER_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "d_array.h"
#include "r_math.h"

#define ASSETS_DIR_PATH "assets/"
#define ASSETS_DEFAULT_DIR_PATH "assets/default/"
#define ASSETS_TEXTURE_DIR_PATH "assets/texture/"

struct vertex {
    vec3_t position, normal;
    vec2_t uv;
};

typedef struct material {
    char *name;
    float shininess;
    vec3_t ambient, diffuse, specular, emissibity;
    float density, transparency;
    int illumination;
} material_t;

typedef struct mesh {
    char *filename;
    vertex_array_t vertices;
    uint32_array_t indices;
    material_t material;
} mesh_t;

void r_mesh_load(mesh_t *mesh, char *filepath);

typedef struct object {
    uint32_t vao, vbo;
    mesh_t *meshes;
    size_t size;
} object_t;

typedef struct transformation {
    uint16_t id;
    vec3_t scale;
    vec3_t rotation_origin;
    quat_t rotation;
    vec3_t translation;
} transformation_t;

typedef struct instance {
    object_t *object;
    transformation_t transformation;
} instance_t;

#endif // !__R_RENDERER_H__