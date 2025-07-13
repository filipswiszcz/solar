#version 410 core

out vec4 o_Color;

uniform vec3 u_Color;
uniform float u_Brightness;

void main() {
    o_Color = vec4(u_Color * u_Brightness, 1.0f);
}