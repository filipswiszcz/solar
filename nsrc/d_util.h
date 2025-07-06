#ifndef __D_UTIL_H__
#define __D_UTIL_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <stb/stb_image.h>

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_GLCOREARB
    #include <GLFW/glfw3.h>
#else
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#endif

#include "r_renderer.h"

#define ASSERT(_e, ...) if (!(_e)) {fprintf(stderr, __VA_ARGS__);}
#define RSSERT(_e, ...) if (!(_e)) {fprintf(stderr, __VA_ARGS__); return;}
#define VSSERT(_e, v, ...) if (!(_e)) {fprintf(stderr, __VA_ARGS__); return v;}
#define QSSERT(_e, ...) if (!(_e)) {fprintf(stderr, __VA_ARGS__); exit(1);}

// TEMPORARY
static uint32_t d_cubemap_read(char *faces[6]) {
    uint32_t id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    int width, height, channels;
    for (uint32_t i = 0; i < 6; i++) {
        unsigned char *image = stbi_load(faces[i], &width, &height, &channels, 4);
        ASSERT(image, "Failed to load texture: %s\n", faces[i]);

        uint32_t format = (channels == 4) ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
        stbi_image_free(image);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return id;
}

static uint32_t d_texture_read(const char *filepath) {
    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int w, h, c;
    unsigned char *img = stbi_load(filepath, &w, &h, &c, 0);
    ASSERT(img, "Failed to load texture");

    int frmt, ifrmt;
    switch (c) {
        case 1:
            frmt = GL_RED;
            ifrmt = GL_RED;
            break;
        case 3:
            frmt = GL_RGB;
            ifrmt = GL_RGB;
            break;
        case 4:
            frmt = GL_RGBA;
            ifrmt = GL_RGBA;
            break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, ifrmt, w, h, 0, frmt, GL_UNSIGNED_BYTE, img);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(img);

    return texture;
}

static void d_mesh_so_read(mesh_t *mesh, char *filepath) {
    FILE *file = fopen(filepath, "r");

    ASSERT(file != NULL, "Failed to open the mesh file: %s", filepath);

    // uint32_t vertices, indices;
    // fread(&vertices, sizeof(uint32_t), 1, file);
    // fread(&indices, sizeof(uint32_t), 1, file);

    // mesh -> vertices.values = (vertex_t*) malloc(vertices * sizeof(vertex_t));
    // mesh -> indices.values = (uint32_t*) malloc(indices * sizeof(uint32_t));

    // would memory arena help here?
        // 

    // fread(mesh -> vertices.values, sizeof(vertex_t), vertices, file);
    // fread(mesh -> indices.values, sizeof(uint32_t), indices, file);

    fclose(file);
}

#endif // !__D_UTIL_H__