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

// vertex

typedef struct vertex {
    vec3_t position, normal;
    vec2_t uv;
} vertex_t;

struct triplet {
    uint32_t pix, nix, uix;
};

// array

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

// hashmap

#define MAP_DEFAULT_INITIAL_CAPACITY 16

typedef struct map_item {
    const void *key;
    void *value;
} map_item_t;

typedef enum map_type {
    MAP_KEY_CHAR = 0,
    MAP_KEY_INT = 4,
    MAP_KEY_FLOAT = 4,
    MAP_KEY_TRIPLET = 12
} map_type_t;

typedef struct map {
    map_item_t *items;
    map_type_t type;
    size_t size, capacity;
} map_t;

map_t *map_init(map_type_t type) {
    map_t *map = malloc(sizeof(map_t));
    if (map == NULL) {return NULL;}
    map -> items = calloc(MAP_DEFAULT_INITIAL_CAPACITY, sizeof(map_item_t));
    if (map -> items == NULL) {free(map); return NULL;}
    map -> type = type;
    map -> size = 0;
    map -> capacity = MAP_DEFAULT_INITIAL_CAPACITY;
    return map;
}

int map_destroy(map_t *map) {
    for (size_t i = 0; i < map -> size; i++) {
        free((void*) map -> items[i].key);
    }
    free(map -> items); free(map);
}

#define MAP_FNV_OFFSET 14695981039346656037UL
#define MAP_FNV_PRIME 1099511628211UL

uint64_t map_hash_key(map_type_t type, const void *key) {
    uint64_t hash = MAP_FNV_OFFSET;
    const unsigned char *p = (const unsigned char*) key;
    for (size_t i = 0; i < (size_t) type; i++) {
        hash ^= (uint64_t) p[i];
        hash *= MAP_FNV_PRIME;
    }
    return hash;
}

void *map_get(map_t *map, const void *key) {
    uint64_t hash = map_hash_key(map -> type, key);
    size_t index = (size_t) (hash & (uint64_t) (map -> capacity - 1));
    while (map -> items[index].key != NULL) {
        switch (map -> type) {
            case MAP_KEY_CHAR:
                break;
            case MAP_KEY_INT:
                break;
            case MAP_KEY_TRIPLET:
                if ((*(struct triplet*) key).pix == (*(struct triplet*) map -> items[index].key).pix
                    && (*(struct triplet*) key).nix == (*(struct triplet*) map -> items[index].key).nix
                    && (*(struct triplet*) key).uix == (*(struct triplet*) map -> items[index].key).uix) {
                        return map -> items[index].value;
                }
                break;
        }

        index++;
        if (index >= map -> capacity) {index = 0;}
    }
    return NULL;
}

int _map_set_item(map_type_t type, map_item_t *items, size_t *size, size_t capacity, const void *key, void *value) {
    uint64_t hash = map_hash_key(type, key);
    size_t index = (size_t) (hash & (uint64_t) (capacity - 1));
    while (items[index].key != NULL) {
        switch (type) {
            case MAP_KEY_CHAR:
                break;
            case MAP_KEY_INT:
                break;
            case MAP_KEY_TRIPLET:
                if ((*(struct triplet*) key).pix == (*(struct triplet*) items[index].key).pix
                    && (*(struct triplet*) key).nix == (*(struct triplet*) items[index].key).nix
                    && (*(struct triplet*) key).uix == (*(struct triplet*) items[index].key).uix) {
                        items[index].value = value; return 0;
                }
                break;
        }

        index++;
        if (index >= capacity) {index = 0;}
    }

    void *copy = malloc((type == MAP_KEY_CHAR) ? strlen(key) + 1 : (size_t) type);
    if (!copy) {return 1;}

    if (type == MAP_KEY_CHAR) {strcpy(copy, key);}
    else {memcpy(copy, key, (size_t) type);}

    if (size) {(*size)++;}

    items[index].key = copy;
    items[index].value = value;

    return 0; // return index after set? 
}

int _map_resize(map_t *map) {
    size_t capacity = map -> capacity * 2;
    if (capacity < map -> capacity) {return 1;} // overflow
    
    map_item_t *items = calloc(capacity, sizeof(map_item_t));
    if (items == NULL) {return 1;}
    
    for (size_t i = 0; i < map -> capacity; i++) {
        if (map -> items[i].key != NULL) {
            _map_set_item(map -> type, items, NULL, capacity, map -> items[i].key, map -> items[i].value);
        }
    }

    free(map -> items);

    map -> items = items;
    map -> capacity = capacity;

    return 0;
}

int map_set(map_t *map, const void *key, void *value) {
    if (value == NULL) {return 1;}
    if (map -> size >= map -> capacity / 2) {
        if (_map_resize(map)) {return 1;}
    }
    return _map_set_item(map -> type, map -> items, &map -> size, map -> capacity, key, value);
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
            .position = positions.values[indices.values[i] - 1],
            .normal = normals.values[indices.values[i + 1] - 1],
            .uv = uvs.values[indices.values[i + 2] - 1]
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

    fwrite(&vertices -> size, sizeof(uint32_t), 1, file);
    fwrite(vertices -> values, sizeof(vertex_t), vertices -> size, file);

    fclose(file);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: otob <filename>\n"); return 1;
    }

    // read obj
    vertex_array_t obj_vertices = {0};
    obj_file_read(&obj_vertices, argv[1]);

    // write as so
    obj_file_write(&obj_vertices, "../assets/model/cube.so");

    free(obj_vertices.values);

    // read so
    FILE *file = fopen("../assets/model/cube.so", "rb");
    
    uint32_t so_size;
    fread(&so_size, sizeof(uint32_t), 1, file);
    vertex_array_t so_vertices = {0};
    so_vertices.values = (vertex_t*) malloc(256 * sizeof(vertex_t));
    so_vertices.size = so_size;
    so_vertices.capacity = 256;
    fread(so_vertices.values, sizeof(vertex_t), so_size, file);

    for (int i = 0; i < 3 && i < so_size; i++) {
        printf("vertex %d: pos=(%f, %f, %f) normal=(%f, %f, %f) uv=(%f, %f)\n",
            i,
            so_vertices.values[i].position.x, 
            so_vertices.values[i].position.y,
            so_vertices.values[i].position.z,
            so_vertices.values[i].normal.x,
            so_vertices.values[i].normal.y,
            so_vertices.values[i].normal.z,
            so_vertices.values[i].uv.x,
            so_vertices.values[i].uv.y);
    }

    const struct triplet indices = {1, 2, 3};

    const uint32_t v = 2;

    map_t *map = map_init(MAP_KEY_TRIPLET);
    map_set(map, &indices, &v);

    void *vr = map_get(map, &indices);
    printf("v=%d\n", *(uint32_t*) vr);

    free(so_vertices.values);

    fclose(file);

    // read obj positions, normals, uvs, faces, material
    // for each face
        // if hashmap contains key (pos_id, uv_id, norm_id)
            // reuse vertex from hashmap
        // else
            // create new vertex
    

    return 0;
}