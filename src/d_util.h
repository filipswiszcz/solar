#ifndef __D_UTIL_H__
#define __D_UTIL_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <stb/stb_image.h>
#include <stb/stb_easy_font.h>

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_GLCOREARB
    #include <GLFW/glfw3.h>
#else
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#endif

#define ASSERT(_e, ...) if (!(_e)) {fprintf(stderr, __VA_ARGS__);}
#define RSSERT(_e, ...) if (!(_e)) {fprintf(stderr, __VA_ARGS__); return;}
#define VSSERT(_e, v, ...) if (!(_e)) {fprintf(stderr, __VA_ARGS__); return v;}
#define QSSERT(_e, ...) if (!(_e)) {fprintf(stderr, __VA_ARGS__); exit(1);}

// TEXTURE
void d_util_texture_read(uint32_t *texture, const char *filepath);

void d_util_cubemap_read(uint32_t *texture, const char *filepaths[6]);

#endif // !__D_UTIL_H__