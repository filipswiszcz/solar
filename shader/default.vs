#version 410 core

layout (location = 0) in vec3 l_Position;
layout (location = 1) in vec3 l_Normal;
layout (location = 2) in vec2 l_Uv;

out vec3 t_Position;
out vec3 t_Normal;
out vec2 t_Uv;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main() {
    t_Position = vec3(u_Model * vec4(l_Position, 1.0f));
    t_Normal = mat3(transpose(inverse(u_Model))) * l_Normal;
    t_Uv = l_Uv;
    gl_Position = u_Projection * u_View * u_Model * vec4(l_Position, 1.0f);
}