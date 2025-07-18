#version 410 core

in vec3 t_Position;
in vec3 t_Normal;
in vec2 t_Uv;

out vec4 o_Color;

uniform sampler2D u_Texture;
uniform vec3 u_Light;
uniform int u_Emissive;

void main() {
    vec3 albedo = texture(u_Texture, t_Uv).rgb;

    if (u_Emissive == 1) {
        o_Color = vec4(albedo * 8.0f, 1.0f); return;
    }

    float difference = max(dot(normalize(t_Normal), normalize(u_Light - t_Position)), 0.0f);
    vec3 color = albedo * (0.08 + difference) * (1.0f / (1.0f + 0.0000001f * length(u_Light - t_Position) * length(u_Light - t_Position)));
    o_Color = vec4(color, 1.0f);
}