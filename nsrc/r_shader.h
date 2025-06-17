#ifndef __R_SHADER_H__
#define __R_SHADER_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_GLCOREARB
    #include <GLFW/glfw3.h>
#else
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#endif

#include "d_util.h"
#include "r_math.h"

typedef struct shader {
    uint32_t ids[2]; // [0] = vert, [1] = frag
    uint32_t program;
} shader_t;

void r_create_program(shader_t *shader, char *frag_path, char *vert_path);

void r_set_vec3(shader_t *shader, char *name, vec3_t vec);

void r_set_mat4(shader_t *shader, char *name, mat4_t mat);

#endif // !__R_SHADER_H__
