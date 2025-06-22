/*
 * otob - convert an obj file to a binary file
 *
 * Usage:
 *      otob <file>
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// math

typedef struct vec2 {
    float x, y;
} vec2_t;

typedef struct vec3 {
    float x, y, z;
} vec3_t;

// arrays

typedef struct uint32_array {
    uint32_t *values;
    size_t size, capacity;
} uint32_array_t;

void uint32_array_insert(uint32_array_t *array, uint32_t value) {
    if (array -> size >= array -> capacity) {
        if (array -> capacity == 0) array -> capacity = 256;
        else array -> capacity *= 2;
        array -> values = (uint32_t*) realloc(array -> values, array -> capacity * sizeof(*array -> values));
    }
    array -> values[array -> size++] = value;
}

typedef struct vec2_array {
    vec2_t *values;
    size_t size, capacity;
} vec2_array_t;

void vec2_array_insert(vec2_array_t *array, vec2_t value) {
    if (array -> size >= array -> capacity) {
        if (array -> capacity == 0) array -> capacity = 256;
        else array -> capacity *= 2;
        array -> values = (vec2_t*) realloc(array -> values, array -> capacity * sizeof(*array -> values));
    }
    array -> values[array -> size++] = value;
}

typedef struct vec3_array {
    vec3_t *values;
    size_t size, capacity;
} vec3_array_t;

void vec3_array_insert(vec3_array_t *array, vec3_t value) {
    if (array -> size >= array -> capacity) {
        if (array -> capacity == 0) array -> capacity = 256;
        else array -> capacity *= 2;
        array -> values = (vec3_t*) realloc(array -> values, array -> capacity * sizeof(*array -> values));
    }
    array -> values[array -> size++] = value;
}

// vertex

typedef struct vertex {
    vec3_t position, normal;
    vec2_t uv;
} vertex_t;

typedef struct vertex_array {
    vertex_t *values;
    size_t size, capacity;
} vertex_array_t;

void vertex_array_insert(vertex_array_t *array, vertex_t value) {
    if (array -> size >= array -> capacity) {
        if (array -> capacity == 0) array -> capacity = 256;
        else array -> capacity *= 2;
        array -> values = (vertex_t*) realloc(array -> values, array -> capacity * sizeof(*array -> values));
    }
    array -> values[array -> size++] = value;
}

// object

void _obj_file_read_material() {}

void _obj_file_read_indices(uint32_array_t *array, char line[128]) {
    uint16_t mode = 0, offset = 0;
    for (size_t i = 0; i < strlen(line); i++) {
        if (line[i] >= '0' && line[i] <= '9') {
            if (mode) {
                offset++;
                if (i == (strlen(line) - 1)) {
                    char *part = (char*) malloc(offset + 1);
                    strncpy(part, &line[i - offset + 1], offset);
                    part[offset] = '\0';
                    uint32_t index = strtoul(part, NULL, 0);
                    uint32_array_insert(array, index);
                    free(part);
                }
            } else {
                mode = 1; offset = 1;
                if (i == (strlen(line) - 1)) {
                    char *part = (char*) malloc(offset + 1);
                    strncpy(part, &line[i - offset + 1], offset);
                    part[offset] = '\0';
                    uint32_t index = strtoul(part, NULL, 0);
                    uint32_array_insert(array, index);
                    free(part);
                }
            }
        } else if (line[i] == ' ' || line[i] == '/' || line[i] == '\n') {
            if (mode) {
                char *part = (char*) malloc(offset + 1);
                strncpy(part, &line[i - offset], offset);
                part[offset] = '\0';
                uint32_t index = strtoul(part, NULL, 0);
                uint32_array_insert(array, index);
                free(part);
                mode = 0; offset = 0;
            }
        }
    }
}

void obj_file_read(vertex_array_t *vertices, char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to read the file: %s\n", filename); return;
    }

    vec3_array_t positions = {0};
    vec3_array_t normals = {0};
    vec2_array_t uvs = {0};

    uint32_array_t indices = {0};

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            vec3_t position;
            sscanf(line, "v %f %f %f", &position.x, &position.y, &position.z);
            vec3_array_insert(&positions, position);
        } else if (strncmp(line, "vn", 2) == 0) {
            vec3_t normal;
            sscanf(line, "vn %f %f %f", &normal.x, &normal.y, &normal.z);
            vec3_array_insert(&normals, normal);
        } else if (strncmp(line, "vt", 2) == 0) {
            vec2_t uv;
            sscanf(line, "vt %f %f", &uv.x, &uv.y);
            vec2_array_insert(&uvs, uv);
        } else if (strncmp(line, "f", 1) == 0) {
            _obj_file_read_indices(&indices, line);
        } else if (strncmp(line, "mtllib", 6) == 0) {
            // char material_filename[128];
            // sscanf(line, "mtllib %s", material_filename);

            // char *part;
            // part = strtok(filename, "/");
            // while (part != NULL) {
            //     printf("%s\n", part);
            //     part = strtok(NULL, "/");
            // }
            // printf("%s\n", part);



            // char material_filepath[256];
            // sprintf(material_filepath, "%s");

            // char title[64];
            // sprintf(title, "%s [%d FPS]", WINDOW_NAME, context.fps.frames);

            // char prepath[256], path[128];
            // sscanf(line, "mtllib %s", path);
            // mesh -> material.name = path;
            // strcpy(prepath, ASSETS_MODEL_DEFAULT_DIR_PATH);
            // strcat(prepath, path);
            // r_mesh_load_material(mesh -> material, prepath);
        }
    }

    // printf("\nsize=%ld\n", indices.size);

    // for (size_t i = 0; i < (indices.size - 1); i += 3) {
    //     printf("f %d/%d/%d\n", indices.values[i], indices.values[i + 1], indices.values[i + 2]);
    // }

    // something here


    for (size_t i = 0; i < indices.size; i += 3) {
        vertex_t vertex = {
            .position = positions.values[indices.values[i]],
            .normal = normals.values[indices.values[i + 1]],
            .uv = uvs.values[indices.values[i + 2]]
        };
        vertex_array_insert(vertices, vertex);
    }
    // probably
    // go through every 3-indices offset
    // create vertice
        // add it to hash map
            // if 1, then add its index to indices arr
            // if 0, then inc index counter and add it to indices arr?? (this is questionable)
    // return vertices arr and indices arr

    free(positions.values);
    free(normals.values);
    free(uvs.values);
    
    fclose(file);
}

void obj_file_write(vertex_array_t *vertices, char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Failed to write the file: %s\n", filename); return;
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: otob <filename>\n"); return 1;
    }

    vertex_array_t vertices = {0};
    obj_file_read(&vertices, argv[1]);

    printf("\nsize=%ld\n", vertices.size);

    // read obj positions, normals, uvs, faces, material
    // for each face
        // if hashmap contains key (pos_id, uv_id, norm_id)
            // reuse vertex from hashmap
        // else
            // create new vertex
    

    return 0;
}