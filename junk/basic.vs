#version 410 core

layout (location = 0) in vec3 vertices;
layout (location = 1) in vec3 normals;

out vec3 modif_vertices;
out vec3 modif_normals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    modif_vertices = vec3(model * vec4(vertices, 1.0f));
    modif_normals = normals;
    // modif_normals = mat3(transpose(inverse(model))) * normals;
    gl_Position = projection * view * vec4(modif_vertices, 1.0f);

    // gl_Position = projection * view * model * vec4(vertices.x, vertices.y, vertices.z, 1.0f);
}