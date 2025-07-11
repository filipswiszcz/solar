#ifndef RES_LOAD_H
#define RES_LOAD_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <regex.h>

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>

#include "object/mesh.h"
#include "common.h"

#define ASSETS_DEFAULT_DIR_PATH "assets/default/"

void mesh_load(mesh_t *mesh, const char *filepath);

void _mesh_get_indices(iarray_t *arr, char ln[64]);

void _mesh_get_material(material_t material, const char *filepath);

void texture_load(uint32_t texture, const char *filepath);

#endif /* RES_LOAD_H */