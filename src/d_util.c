#include "d_util.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

// TEXTURE

void d_util_texture_read(uint32_t *texture, const char *filepath) {
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int32_t width, height, channels;
    unsigned char *image = stbi_load(filepath, &width, &height, &channels, 0);
    RSSERT(image, "Failed to load texture: %s\n", filepath);

    uint32_t format;
    switch (channels) {
        case 1: {format = GL_RED; break;}
        case 3: {format = GL_RGB; break;}
        case 4: {format = GL_RGBA; break;}
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image);
}

void d_util_cubemap_read(uint32_t *texture, const char *filepaths[6]) {
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);

    int32_t width, height, channels;
    for (uint32_t i = 0; i < 6; i++) {
        unsigned char *image = stbi_load(filepaths[i], &width, &height, &channels, 4);
        RSSERT(image, "Failed to load texture: %s\n", filepaths[i]);

        uint32_t format = (channels == 4) ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
        stbi_image_free(image);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}