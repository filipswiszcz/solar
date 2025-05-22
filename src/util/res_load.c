#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "util/res_load.h"

void mesh_load(mesh_t *mesh, const char *filepath) {
    FILE *file = fopen(filepath, "r");

    ASSERT(file != NULL, "Failed to open the file: %s\n", filepath);

    char ln[64];
    while (fgets(ln, sizeof(ln), file)) {
        if (strncmp(ln, "v ", 2) == 0) {
            float x, y, z;
            sscanf(ln, "v %f %f %f", &x, &y, &z);
            farray_add(&mesh -> vertices, x);
            farray_add(&mesh -> vertices, y);
            farray_add(&mesh -> vertices, z);
        } else if (strncmp(ln, "vn", 2) == 0) {
            float x, y, z;
            sscanf(ln, "vn %f %f %f", &x, &y, &z);
            // farray_add(&mesh -> normals, x);
            // farray_add(&mesh -> normals, y);
            // farray_add(&mesh -> normals, z);
        } else if (strncmp(ln, "vt", 2) == 0) {
            float u, v;
            sscanf(ln, "vt %f %f", &u, &v);
        } else if (strncmp(ln, "f", 1) == 0) {
            _mesh_get_indices(&mesh -> indices, ln);
        } else if (strncmp(ln, "mtllib", 6) == 0) {
            char prepath[256], path[128];
            sscanf(ln, "mtllib %s", path);
            // mesh -> material.name = path;
            strcpy(prepath, ASSETS_DEFAULT_DIR_PATH);
            strcat(prepath, path);
            _mesh_get_material(mesh -> material, prepath);
        }
    }
    fclose(file);
}

void _mesh_get_indices(iarray_t *arr, char ln[64]) { // it is super cool now!
    int g = 0, c = 0;
    for (int i = 0; i < 64; i++) {
        if (ln[i] == ' ') g = 1;
        else if (ln[i] == '/' && g == 1) {
            int l = (i + c) - i;
            char *sub = (char*) malloc(l + 1);
            strncpy(sub, &ln[i - c], l);
            sub[l] = '\0';
            uint32_t val = strtoul(sub, NULL, 0);
            iarray_add(arr, val - 1);
            free(sub);
            g = 0; c = 0;
        } else if (g == 1) c += 1;
    }
}

void _mesh_get_material(material_t material, const char *filepath) {
    FILE *file = fopen(filepath, "r");
    
    ASSERT(file != NULL, "Failed to get the file's material: %s\n", filepath);

    char ln[64];
    while (fgets(ln, sizeof(ln), file)) {
        if (strncmp(ln, "Ns", 2) == 0) {
            sscanf(ln, "Ns %f", &material.shininess);
        } else if (strncmp(ln, "Ka", 2) == 0) {
            sscanf(ln, "Ka %f %f %f", &material.ambient.x, &material.ambient.y, &material.ambient.z);
        } else if (strncmp(ln, "Kd", 2) == 0) {
            sscanf(ln, "Kd %f %f %f", &material.diffuse.x, &material.diffuse.y, &material.diffuse.z);
        } else if (strncmp(ln, "Ks", 2) == 0) {
            sscanf(ln, "Ks %f %f %f", &material.specular.x, &material.specular.y, &material.specular.z);
        } else if (strncmp(ln, "Ke", 2) == 0) {
            sscanf(ln, "Ke %f %f %f", &material.emissibity.x, &material.emissibity.y, &material.emissibity.z);
        } else if (strncmp(ln, "Ni", 2) == 0) {
            sscanf(ln, "Ni %f", &material.density);
        } else if (strncmp(ln, "d", 1) == 0) {
            sscanf(ln, "d %f", &material.transparency);
        } else if (strncmp(ln, "illum", 5) == 0) {
            sscanf(ln, "illum %d", &material.illumination);
        }
    }
    fclose(file);
}

void texture_load(uint32_t texture, const char *filepath) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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
}