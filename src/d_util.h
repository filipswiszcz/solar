#ifndef __D_UTIL_H__
#define __D_UTIL_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <stb/stb_image.h>

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_GLCOREARB
    #include <GLFW/glfw3.h>
#else
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#endif

// #include "r_renderer.h"

#define ASSERT(_e, ...) if (!(_e)) {fprintf(stderr, __VA_ARGS__);}
#define RSSERT(_e, ...) if (!(_e)) {fprintf(stderr, __VA_ARGS__); return;} // return v
#define VSSERT(_e, v, ...) if (!(_e)) {fprintf(stderr, __VA_ARGS__); return v;} // del?
#define QSSERT(_e, ...) if (!(_e)) {fprintf(stderr, __VA_ARGS__); exit(1);}

// TEMPORARY?
static void d_date_gregorian_to_julian(double *date, uint32_t day, uint32_t month, uint32_t year) {
    if (month <= 2) {year -= 1; month += 12;}
    *date = floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) + day + (2 - (year / 100) + ((year / 100) / 4)) - 1524.5;
}

// TEMPORARY?
static void d_date_julian_to_gregorian(uint32_t *day, uint32_t *month, uint32_t *year, double date) {
    uint32_t z = (uint32_t) (date += 0.5);

    uint32_t a;
    if (z < 2299161) a = z;
    else {
        uint32_t t = (uint32_t) ((z - 1867216.25) / 36524.25);
        a = z + 1 + t - (t / 4);
    }

    uint32_t b = a + 1524;
    uint32_t c = (uint32_t) ((b - 122.1) / 365.25);
    uint32_t d = (uint32_t) (365.25 * c);
    uint32_t e = (uint32_t) ((b - d) / 30.6001);

    *day = b - d - (uint32_t) (30.6001 * e) + (date - z);
    *month = (e < 14) ? e - 1 : e - 13;
    *year = (*month > 2) ? c - 4716 : c - 4715;
}

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

// TEMPORARY
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
    ASSERT(img, "Failed to load texture: %s\n", filepath);

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

#endif // !__D_UTIL_H__