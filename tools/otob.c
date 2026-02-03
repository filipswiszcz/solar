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

// UTIL

#define QSSERT(_e, ...) if (!(_e)) {fprintf(stderr,  __VA_ARGS__); exit(1);}

static char *strdup(const char *content) {
    size_t size = strlen(content) + 1;
    char *copy = malloc(size);
    if (copy) {
        memcpy(copy, content, size);
    }
    return copy;
}

static char *file_ext_swap(const char *filename, const char *extension) {
    char *copyname = strdup(filename);
    if (!copyname) return NULL;
    char *mark = strrchr(copyname, '.');
    if (mark) *mark = '\0';
    static char name[512];
    snprintf(name, sizeof(name), "%s.%s", copyname, extension);
    free(copyname);
    return name;
}

// MATH

typedef struct vec2 {
    float x, y;
} vec2_t;

typedef struct vec3 {
    float x, y, z;
} vec3_t;

// MESH

typedef struct vertex {
    vec3_t position, normal;
    vec2_t uv;
} vertex_t;

typedef struct material {
    float shininess;
    vec3_t ambient, diffuse, specular, emissivity;
    float density, transparency;
    int illumination;
} material_t;

// ARRAY

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

// HASHMAP

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
        free((void*) map -> items[i].value);
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
                if (*(int*) key == *(int*) map -> items[index].key) {
                    return map -> items[index].value;
                }
                break;
            // case MAP_KEY_TRIPLET:
            //     if ((*(struct triplet*) key).pix == (*(struct triplet*) map -> items[index].key).pix
            //         && (*(struct triplet*) key).nix == (*(struct triplet*) map -> items[index].key).nix
            //         && (*(struct triplet*) key).uix == (*(struct triplet*) map -> items[index].key).uix) {
            //             return map -> items[index].value;
            //     }
            //     break;
            case MAP_KEY_TRIPLET:
                if ((*(vec3_t*) key).x == (*(vec3_t*) map -> items[index].key).x
                    && (*(vec3_t*) key).y == (*(vec3_t*) map -> items[index].key).y
                    && (*(vec3_t*) key).z == (*(vec3_t*) map -> items[index].key).z) {
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
                if (*(int*) key == *(int*) items[index].key) {
                    items[index].value = value; return 0;
                }
                break;
            // case MAP_KEY_TRIPLET:
            //     if ((*(struct triplet*) key).pix == (*(struct triplet*) items[index].key).pix
            //         && (*(struct triplet*) key).nix == (*(struct triplet*) items[index].key).nix
            //         && (*(struct triplet*) key).uix == (*(struct triplet*) items[index].key).uix) {
            //             items[index].value = value; return 0;
            //     }
            //     break;
            case MAP_KEY_TRIPLET:
                if ((*(vec3_t*) key).x == (*(vec3_t*) items[index].key).x
                    && (*(vec3_t*) key).y == (*(vec3_t*) items[index].key).y
                    && (*(vec3_t*) key).z == (*(vec3_t*) items[index].key).z) {
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

    // value malloc
    void *vcopy = malloc((size_t) type); // doesnt work for chars
    if (!vcopy) {return 1;}

    memcpy(vcopy, value, (size_t) type);
    // end of value malloc

    if (size) {(*size)++;}

    items[index].key = copy;
    // items[index].value = value;
    items[index].value = vcopy;

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

// MESH

void _obj_file_read_material(material_t *material, char *filename) {
    FILE *file = fopen(filename, "r");

    QSSERT(file != NULL, "Failed to read the material file: %s\n", filename);

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "Ns", 2) == 0) {
            sscanf(line, "Ns %f", &material -> shininess);
        } else if (strncmp(line, "Ka", 2) == 0) {
            sscanf(line, "Ka %f %f %f", &material -> ambient.x, &material -> ambient.y, &material -> ambient.z);
        } else if (strncmp(line, "Kd", 2) == 0) {
            sscanf(line, "Kd %f %f %f", &material -> diffuse.x, &material -> diffuse.y, &material -> diffuse.z);
        } else if (strncmp(line, "Ks", 2) == 0) {
            sscanf(line, "Ks %f %f %f", &material -> specular.x, &material -> specular.y, &material -> specular.z);
        } else if (strncmp(line, "Ke", 2) == 0) {
            sscanf(line, "Ke %f %f %f", &material -> emissivity.x, &material -> emissivity.y, &material -> emissivity.z);
        } else if (strncmp(line, "Ni", 2) == 0) {
            sscanf(line, "Ni %f", &material -> density);
        } else if (strncmp(line, "d", 1) == 0) {
            sscanf(line, "d %f", &material -> transparency);
        } else if (strncmp(line, "illum", 5) == 0) {
            sscanf(line, "illum %d", &material -> illumination);
        }
    }

    fclose(file);
}

void _obj_file_read_indices(uint32_array_t *indices, char line[128]) {
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
                    uint32_array_insert(indices, index);
                    free(part);
                }
            } else {
                mode = 1; offset = 1;
                if (i == (strlen(line) - 1)) {
                    char *part = (char*) malloc(offset + 1);
                    strncpy(part, &line[i - offset + 1], offset);
                    part[offset] = '\0';
                    uint32_t index = strtoul(part, NULL, 0);
                    uint32_array_insert(indices, index);
                    free(part);
                }
            }
        } else if (line[i] == ' ' || line[i] == '/' || line[i] == '\n') {
            if (mode) {
                char *part = (char*) malloc(offset + 1);
                strncpy(part, &line[i - offset], offset);
                part[offset] = '\0';
                uint32_t index = strtoul(part, NULL, 0);
                uint32_array_insert(indices, index);
                free(part);
                mode = 0; offset = 0;
            }
        }
    }
}

void obj_file_read(vertex_array_t *vertices, uint32_array_t *indices, material_t *material, char *filepath) {
    FILE *file = fopen(filepath, "r");

    QSSERT(file != NULL, "Failed to read the file: %s\n", filepath);

    vec3_array_t positions = {0};
    vec3_array_t normals = {0};
    vec2_array_t uvs = {0};

    uint32_array_t faces = {0};

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
            _obj_file_read_indices(&faces, line);
        } else if (strncmp(line, "mtllib", 6) == 0) {
            _obj_file_read_material(material, file_ext_swap(filepath, "mtl"));
        }
    }

    map_t *polygons = map_init(MAP_KEY_TRIPLET);
    uint32_t cursor = 0;

    for (size_t i = 0; i < faces.size; i += 3) {

        vec3_t polygon = {faces.values[i], faces.values[i + 1], faces.values[i + 2]};

        void *value = map_get(polygons, &polygon);
        if (value != NULL) {
            uint32_array_insert(indices, *(uint32_t*) value);
        } else {
            vertex_t vertex = {
                .position = positions.values[faces.values[i] - 1],
                .normal = normals.values[faces.values[i + 2] - 1],
                .uv = uvs.values[faces.values[i + 1] - 1]
            };

            map_set(polygons, &polygon, &cursor);

            vertex_array_insert(vertices, vertex);
            uint32_array_insert(indices, cursor++);
        }
    }

    free(positions.values);
    free(normals.values);
    free(uvs.values);
    
    fclose(file);
}

void orb_file_write(vertex_array_t *vertices, uint32_array_t *indices, material_t *material, char *filepath) {
    FILE *file = fopen(filepath, "wb");

    QSSERT(file != NULL, "Failed to write the file: %s\n", filepath);

    fwrite(&vertices -> size, sizeof(uint32_t), 1, file);
    fwrite(&indices -> size, sizeof(uint32_t), 1, file);
    fwrite(vertices -> values, sizeof(vertex_t), vertices -> size, file);
    fwrite(indices -> values, sizeof(uint32_t), indices -> size, file);

    fwrite(&material -> shininess, sizeof(float), 1, file);
    fwrite(&material -> ambient, sizeof(vec3_t), 1, file);
    fwrite(&material -> diffuse, sizeof(vec3_t), 1, file);
    fwrite(&material -> specular, sizeof(vec3_t), 1, file);
    fwrite(&material -> emissivity, sizeof(vec3_t), 1, file);
    fwrite(&material -> density, sizeof(float), 1, file);
    fwrite(&material -> transparency, sizeof(float), 1, file);
    fwrite(&material -> illumination, sizeof(uint32_t), 1, file);

    fclose(file);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: otob <filename>\n"); return 1;
    }

    char *mark = strrchr(argv[1], '.');
    QSSERT(!strcmp(mark, ".obj"), "File has be in OBJ format\n");

    vertex_array_t vertices = {0};
    uint32_array_t indices = {0};
    material_t material = {0};

    obj_file_read(&vertices, &indices, &material, argv[1]);
    orb_file_write(&vertices, &indices, &material, file_ext_swap(argv[1], "orb"));

    free(vertices.values);
    free(indices.values);

    return 0;
}