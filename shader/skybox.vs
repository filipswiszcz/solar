#version 410 core

layout (location = 0) in vec3 vertex;

out vec3 coords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    coords = vertex;
    vec4 position = projection * view * vec4(vertex, 1.0f);
    gl_Position = position.xyww;
}