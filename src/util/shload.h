#ifndef SHADER_LOAD_H
#define SHADER_LOAD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>

#include "common.h"

void shread(char *filepath, char **shader) {
    FILE *file = fopen(filepath, "r");

    ASSERT(file != NULL, "Failed to open the file: %s\n", filepath);

    fseek(file, 0, SEEK_END);
    size_t fsize = ftell(file);
    rewind(file);

    char *cont = (char*) malloc(fsize + 1);
    fread(cont, 1, fsize, file);
    cont[fsize] = '\0';

    *shader = strdup(cont);

    free(cont);
    fclose(file);
}

uint32_t shcompile(uint32_t type, char *code) {
    uint32_t shader = glCreateShader(type);
    const char *ccode = code;

    glShaderSource(shader, 1, &ccode, NULL);
    glCompileShader(shader);

    char info[512];
    int32_t status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(shader, 512, NULL, info);
        printf("Shader did not compile: %s", info); return -1;
    }

    return shader;
}

uint32_t shlink(uint32_t vshader, uint32_t fshader) {
    uint32_t prog = glCreateProgram();
    glAttachShader(prog, vshader);
    glAttachShader(prog, fshader);
    glLinkProgram(prog);

    char info[512];
    int32_t status;
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        glGetProgramInfoLog(prog, 512, NULL, info);
        printf("Shader did not link: %s", info); return -1;
    }

    return prog;
}

#endif /* SHADER_LOAD_H */