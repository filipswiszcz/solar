#version 410 core

// in vec3 i_Position;
in vec3 o_Normal;
in vec2 o_Uv;

out vec4 o_Color;

uniform sampler2D u_Texture;
uniform vec3 u_Light;

void main() {
    vec3 albedo = texture(u_Texture, o_Uv).rgb;
    float ndot = max(dot(normalize(o_Normal), normalize(-u_Light)), 0.0f);
    vec3 color = albedo * (0.2f + 0.8f * ndot);
    o_Color = vec4(color, 1.0f);
}