#version 410 core

layout (location = 0) in vec3 i_Position;
layout (location = 1) in vec3 i_Normal;
layout (location = 2) in vec2 i_Uv;

// out vec3 o_Position;
out vec3 o_Normal;
out vec2 o_Uv;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main() {
    // o_Position = vec3(u_Model * vec4(i_Position, 1.0f));
    o_Normal = mat3(transpose(inverse(u_Model))) * i_Normal;
    o_Uv = i_Uv;
    gl_Position = u_Projection * u_View * u_Model * vec4(i_Position, 1.0f);
}