#version 410 core

in vec3 t_Position;

out vec4 o_Color;

uniform samplerCube u_Map;

void main() {
    o_Color = texture(u_Map, t_Position);
}