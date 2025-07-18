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
#include "d_util.h"
#include "r_math.h"

#define ASSETS_DIR_PATH "assets/"
#define ASSETS_MODEL_DEFAULT_DIR_PATH "assets/model/default/"
#define ASSETS_MODEL_DIR_PATH "assets/model/"
#define ASSETS_TEXTURE_DIR_PATH "assets/texture/"

// MESH

struct vertex {
    vec3_t position, normal;
    vec2_t uv;
};

typedef struct material {
    float shininess;
    vec3_t ambient, diffuse, specular, emissivity;
    float density, transparency;
    int illumination;
} material_t;

struct mesh {
    char *filename; 
    vertex_array_t vertices;
    uint32_array_t indices;
    size_t offset, size;
    material_t material;
};

void r_mesh_read(mesh_t *mesh, char *filepath);

// typedef struct object {
//     // vertices
//     // indices
//     // material
// } object_t;

typedef struct object {
    mesh_array_t meshes;
    uint32_t vao, vbo, ibo;
} object_t;

void r_object_insert(object_t *object, mesh_t mesh);

void r_object_upload(object_t *object);

void r_object_draw(object_t *object);

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

// SCENE

typedef struct scene {
    // objects
    // skybox
} scene_t;

void r_scene_init(scene_t *scene);

#endif // !__R_RENDERER_H__