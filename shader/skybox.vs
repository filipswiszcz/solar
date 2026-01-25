#version 410 core

layout (location = 0) in vec3 l_Position;

out vec3 t_Position;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main() {
    t_Position = l_Position;
    vec4 position = u_Projection * u_View * vec4(l_Position, 1.0);
    gl_Position = position.xyww;
}