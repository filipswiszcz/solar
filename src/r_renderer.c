#include "r_renderer.h"

// OBJECT

void r_renderer_object_read(arena_t *arena, object_t *object, char *filepath) {
    FILE *file = fopen(filepath, "rb");

    QSSERT(file != NULL, "Failed to open the mesh file: %s", filepath);

    object -> filepath = filepath;

    fread(&object -> mesh.vertices.size, sizeof(uint32_t), 1, file);
    fread(&object -> mesh.indices.size, sizeof(uint32_t), 1, file);

    // object -> mesh.vertices.values = malloc(object -> mesh.vertices.size * sizeof(vertex_t));
    object -> mesh.vertices.values = d_arena_alloc(arena, object -> mesh.vertices.size * sizeof(vertex_t));
    fread(object -> mesh.vertices.values, sizeof(vertex_t), object -> mesh.vertices.size, file);

    // object -> mesh.indices.values = malloc(object -> mesh.indices.size * sizeof(uint32_t));
    object -> mesh.indices.values = d_arena_alloc(arena, object -> mesh.indices.size * sizeof(uint32_t));
    fread(object -> mesh.indices.values, sizeof(uint32_t), object -> mesh.indices.size, file);

    // read materials

    fclose(file);

    object -> visible = 1;
}

void r_renderer_object_upload(object_t *object) {
    glGenVertexArrays(1, &object -> mesh.vao);
    glGenBuffers(1, &object -> mesh.vbo);
    glGenBuffers(1, &object -> mesh.ibo);

    glBindVertexArray(object -> mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, object -> mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, object -> mesh.vertices.size * sizeof(vertex_t), object -> mesh.vertices.values, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object -> mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, object -> mesh.indices.size * sizeof(uint32_t), object -> mesh.indices.values, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) offsetof(vertex_t, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) offsetof(vertex_t, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) offsetof(vertex_t, uv));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
}

void r_renderer_object_draw(object_t *object) {
    if (!object -> visible) return;
    glBindVertexArray(object -> mesh.vao);
    glDrawElements(GL_TRIANGLES, object -> mesh.indices.size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}