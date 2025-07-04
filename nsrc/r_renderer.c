#include "r_renderer.h"

// MESH

static void r_mesh_load_indices(uint32_array_t *array, char line[64]) {
    int gate = 0, offset = 0;
    for (int i = 0; i < 64; i++) {
        if (line[i] == ' ') gate = 1;
        else if (line[i] == '/' && gate == 1) {
            int size = (i + offset) - i;
            char *sub = (char*) malloc(size + 1);
            strncpy(sub, &line[i - offset], size);
            sub[size] = '\0';
            uint32_t value = strtoul(sub, NULL, 0);
            d_uint32_array_insert(array, value - 1);
            free(sub);
            gate = 0; offset = 0;
        } else if (gate == 1) offset = 1;
    }
}

void r_mesh_load_material(material_t material, char *filepath) {
    FILE *file = fopen(filepath, "r");

    ASSERT(file != NULL, "Failed to open the file: %s\n", filepath);

    char line[64];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "Ns", 2) == 0) {
            sscanf(line, "Ns %f", &material.shininess);
        } else if (strncmp(line, "Ka", 2) == 0) {
            sscanf(line, "Ka %f %f %f", &material.ambient.x, &material.ambient.y, &material.ambient.z);
        } else if (strncmp(line, "Kd", 2) == 0) {
            sscanf(line, "Kd %f %f %f", &material.diffuse.x, &material.diffuse.y, &material.diffuse.z);
        } else if (strncmp(line, "Ks", 2) == 0) {
            sscanf(line, "Ks %f %f %f", &material.specular.x, &material.specular.y, &material.specular.z);
        } else if (strncmp(line, "Ke", 2) == 0) {
            sscanf(line, "Ke %f %f %f", &material.emissibity.x, &material.emissibity.y, &material.emissibity.z);
        } else if (strncmp(line, "Ni", 2) == 0) {
            sscanf(line, "Ni %f", &material.density);
        } else if (strncmp(line, "d", 1) == 0) {
            sscanf(line, "d %f", &material.transparency);
        } else if (strncmp(line, "illum", 5) == 0) {
            sscanf(line, "illum %d", &material.illumination);
        }
    }
    fclose(file);
}

// fractal loading?
void r_mesh_read(mesh_t *mesh, char *filepath) { // rewrite reader to handle all obj elements (or change format to something more modern)
    FILE *file = fopen(filepath, "r");

    ASSERT(file != NULL, "Failed to open the file: %s\n", filepath);

    char line[64];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            vertex_t vertex = {0};
            vec3_t position;
            sscanf(line, "v %f %f %f", &position.x, &position.y, &position.z);
            vertex.position = position;
            d_vertex_array_insert(&mesh -> vertices, vertex);
        } else if (strncmp(line, "vn", 2) == 0) {
            vec3_t normal;
            sscanf(line, "vn %f %f %f", &normal.x, &normal.y, &normal.z);
            // vertex.normal = normal;
        } else if (strncmp(line, "vt", 2) == 0) {
            vec2_t uv;
            sscanf(line, "vt %f %f", &uv.x, &uv.y);
            // vertex.uv = uv;
        } else if (strncmp(line, "f", 1) == 0) {
            r_mesh_load_indices(&mesh -> indices, line);
        } else if (strncmp(line, "mtllib", 6) == 0) {
            char prepath[256], path[128];
            sscanf(line, "mtllib %s", path);
            mesh -> material.name = path;
            strcpy(prepath, ASSETS_MODEL_DIR_PATH);
            strcat(prepath, path);
            r_mesh_load_material(mesh -> material, prepath);
        }
    }
    
    fclose(file);
}

void r_object_insert(object_t *object, mesh_t mesh) {
    d_mesh_array_insert(&object -> meshes, mesh);
}

void r_object_upload(object_t *object) {
    glGenVertexArrays(1, &object -> vao);
    glGenBuffers(1, &object -> vbo);
    glGenBuffers(1, &object -> ibo);

    glBindVertexArray(object -> vao);

    // uint32_t vertices_size = 0, indices_size = 0;
    // for (int i = 0; i < object -> meshes.size; i++) {
    //     vertices_size += object -> meshes.values[i].vertices.size;
    //     indices_size += object -> meshes.values[i].indices.size;
    // }

    glBindBuffer(GL_ARRAY_BUFFER, object -> vbo);
    glBufferData(GL_ARRAY_BUFFER, object -> meshes.values[0].vertices.size * sizeof(vertex_t), object -> meshes.values[0].vertices.values, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object -> ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, object -> meshes.values[0].indices.size * sizeof(uint32_t), object -> meshes.values[0].indices.values, GL_STATIC_DRAW);

    // size_t vertex_offset = 0, index_offset = 0, vertex_base = 0;
    // for (int i = 0; i < object -> meshes.size; i++) {
    //     mesh_t *mesh = &object -> meshes.values[i];

    //     if (mesh -> vertices.size > 0 && mesh -> vertices.values) {
    //         glBufferSubData(GL_ARRAY_BUFFER, vertex_offset, mesh -> vertices.size * sizeof(vertex_t), mesh -> vertices.values);
    //         vertex_offset += mesh -> vertices.size * sizeof(vertex_t);
    //     }

    //     if (mesh -> indices.size > 0 && mesh -> indices.values) {
    //         uint32_t *indices = malloc(mesh -> indices.size * sizeof(uint32_t));

    //         for (size_t j = 0; j < mesh -> indices.size; j++) {
    //             indices[j] = mesh -> indices.values[j] + vertex_base;
    //         }

    //         glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, index_offset, mesh -> indices.size * sizeof(uint32_t), indices);
    //         free(indices);

    //         mesh -> offset = index_offset;
    //         index_offset += mesh -> indices.size * sizeof(uint32_t);
    //     }

    //     vertex_base += mesh -> vertices.size;
    // }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) offsetof(vertex_t, position));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void r_object_draw(object_t *object) {
    glBindVertexArray(object -> vao);
    glDrawElements(GL_TRIANGLES, object -> meshes.values[0].indices.size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// void r_object_upload(object_t *object) {
//     glGenVertexArrays(1, &object -> vao);
//     glGenBuffers(1, &object -> vbo);
//     glGenBuffers(1, &object -> ibo);

//     glBindVertexArray(object -> vao);

//     uint32_t vertices_size = 0, indices_size = 0;
//     for (int i = 0; i < object -> meshes.size; i++) {
//         vertices_size += object -> meshes.values[i].vertices.size;
//         indices_size += object -> meshes.values[i].indices.size;
//     }

//     glBindBuffer(GL_ARRAY_BUFFER, object -> vbo);
//     glBufferData(GL_ARRAY_BUFFER, vertices_size * sizeof(vertex_t), NULL, GL_STATIC_DRAW);
    
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object -> ibo);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size * sizeof(uint32_t), NULL, GL_STATIC_DRAW);

//     size_t vertex_offset = 0, index_offset = 0, vertex_base = 0;
//     for (int i = 0; i < object -> meshes.size; i++) {
//         mesh_t *mesh = &object -> meshes.values[i];

//         if (mesh -> vertices.size > 0 && mesh -> vertices.values) {
//             glBufferSubData(GL_ARRAY_BUFFER, vertex_offset, mesh -> vertices.size * sizeof(vertex_t), mesh -> vertices.values);
//             vertex_offset += mesh -> vertices.size * sizeof(vertex_t);
//         }

//         if (mesh -> indices.size > 0 && mesh -> indices.values) {
//             uint32_t *indices = malloc(mesh -> indices.size * sizeof(uint32_t));

//             for (size_t j = 0; j < mesh -> indices.size; j++) {
//                 indices[j] = mesh -> indices.values[j] + vertex_base;
//             }

//             glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, index_offset, mesh -> indices.size * sizeof(uint32_t), indices);
//             free(indices);

//             mesh -> offset = index_offset;
//             index_offset += mesh -> indices.size * sizeof(uint32_t);
//         }

//         vertex_base += mesh -> vertices.size;
//     }

//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) offsetof(vertex_t, position));
//     glEnableVertexAttribArray(0);

//     glBindVertexArray(0);
// }

// void r_object_draw(object_t *object) {
//     if (!object || !object -> meshes.values || object -> meshes.size == 0) return;

//     glBindVertexArray(object -> vao);

//     for (size_t i = 0; i < object -> meshes.size; i++) {
//         if (object -> meshes.values[i].indices.size == 0) continue;
//         glDrawElements(GL_TRIANGLES, object -> meshes.values[i].indices.size, GL_UNSIGNED_INT, (void*) object -> meshes.values[i].offset);
//     }

//     glBindVertexArray(0);
// }

// SCENE

void r_scene_init(scene_t *scene) {}