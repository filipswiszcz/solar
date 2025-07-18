#version 410 core

layout (location = 0) in vec3 vertices;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(vertices.x, vertices.y, vertices.z, 1.0f);
}