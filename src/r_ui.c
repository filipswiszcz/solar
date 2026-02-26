#include "g_game.h"

#define UI_FONT_WIDTH 6
#define UI_FONT_HEIGHT 6

static char UI_GLYPHS[128][UI_FONT_HEIGHT][UI_FONT_WIDTH] = {
    ['a'] = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
    },
    ['b'] = {
        {1, 0, 0, 0, 0},
        {1, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 1, 1, 0, 0},
    },
    ['c'] = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 0, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
    },
    ['d'] = {
        {0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
    },
    ['e'] = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 1, 1, 1, 0},
        {1, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
    },
    ['f'] = {
        {0, 0, 1, 1, 0},
        {0, 1, 0, 0, 0},
        {1, 1, 1, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 0, 0, 0},
    },
    ['g'] = {
        {0, 1, 1, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
    },
    ['h'] = {
        {1, 0, 0, 0, 0},
        {1, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
    },
    ['i'] = {
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
    },
    ['j'] = {
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {1, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
    },
    ['k'] = {
        {1, 0, 0, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 1, 0, 0},
        {1, 1, 0, 0, 0},
        {1, 0, 1, 0, 0},
        {1, 0, 0, 1, 0},
    },
    ['l'] = {
        {0, 1, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 1, 0},
    },
    ['m'] = {
        {0, 0, 0, 0, 0},
        {0, 1, 0, 1, 1},
        {1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1},
    },
    ['n'] = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
    },
    ['o'] = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
    },
    ['p'] = {
        {1, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 1, 1, 0, 0},
        {1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0},
    },
    ['q'] = {
        {0, 1, 1, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 1, 0},
    },
    ['r'] = {
        {0, 0, 0, 0, 0},
        {1, 0, 1, 1, 0},
        {1, 1, 0, 0, 1},
        {1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0},
    },
    ['s'] = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {1, 1, 1, 0, 0},
    },
    ['t'] = {
        {0, 1, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {1, 1, 1, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 1, 1, 0, 0},
    },
    ['u'] = {
        {0, 0, 0, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
    },
    ['v'] = {
        {0, 0, 0, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
    },
    ['w'] = {
        {0, 0, 0, 0, 0},
        {1, 0, 0, 0, 1},
        {1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1},
        {0, 1, 1, 1, 1},
    },
    ['x'] = {
        {0, 0, 0, 0, 0},
        {1, 0, 1, 0, 0},
        {1, 0, 1, 0, 0},
        {0, 1, 0, 0, 0},
        {1, 0, 1, 0, 0},
        {1, 0, 1, 0, 0},
    },
    ['y'] = {
        {0, 0, 0, 0, 0},
        {1, 0, 1, 0, 0},
        {1, 0, 1, 0, 0},
        {1, 0, 1, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 0, 0, 0},
    },
    ['z'] = {
        {0, 0, 0, 0, 0},
        {1, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 1, 0},
    },

    ['A'] = {0},
    ['B'] = {0},
    ['C'] = {0},
    ['D'] = {0},
    ['E'] = {0},
    ['F'] = {0},
    ['G'] = {0},
    ['H'] = {0},
    ['I'] = {0},
    ['J'] = {0},
    ['K'] = {0},
    ['L'] = {0},
    ['M'] = {0},
    ['N'] = {0},
    ['O'] = {0},
    ['P'] = {0},
    ['Q'] = {0},
    ['R'] = {0},
    ['S'] = {0},
    ['T'] = {0},
    ['U'] = {0},
    ['V'] = {0},
    ['W'] = {0},
    ['X'] = {0},
    ['Y'] = {0},
    ['Z'] = {0},

    ['0'] = {
        {0, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
    },
    ['1'] = {
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 1, 0},
    },
    ['2'] = {
        {0, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 1, 0},
    },
    ['3'] = {
        {0, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
    },
    ['4'] = {
        {0, 0, 1, 1, 0},
        {0, 1, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 1, 0},
    },
    ['5'] = {
        {1, 1, 1, 0, 0},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 0, 0},
        {0, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
    },
    ['6'] = {
        {0, 1, 1, 0, 0},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
    },
    ['7'] = {
        {1, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 0, 0, 0},
    },
    ['8'] = {
        {0, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},

    },
    ['9'] = {
        {0, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
    },
    [','] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 1, 0, 0},
    },
    ['.'] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
    },
    ['-'] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
    }
};

// UI

void r_ui_date_init(void) {}

void r_ui_labels_init(void) {

    uint8_t bitmap[(UI_FONT_WIDTH * 16) * (UI_FONT_HEIGHT * 8)];
    memset(bitmap, 0, sizeof(bitmap));
    
    for (uint32_t i = 0; i < 128; i++) {
        uint32_t cbx = (i % 16) * UI_FONT_WIDTH;
        uint32_t cby = (i / 16) * UI_FONT_HEIGHT;
        for (uint32_t j = 0; j < UI_FONT_HEIGHT; j++) {
            for (uint32_t k = 0; k < UI_FONT_WIDTH; k++) {
                if (UI_GLYPHS[i][j][k]) {
                    uint32_t px = cbx + k;
                    uint32_t py = cby + j;
                    uint32_t idx = (py * (UI_FONT_WIDTH * 16)) + px;
                    bitmap[idx] = 255;
                }
            }
        }
    }

    glGenTextures(1, &context.scene.ui.labels.texture);

    glBindTexture(GL_TEXTURE_2D, context.scene.ui.labels.texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (UI_FONT_WIDTH * 16), (UI_FONT_HEIGHT * 8), 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);

    glGenVertexArrays(1, &context.scene.ui.labels.vao);
    glGenBuffers(1, &context.scene.ui.labels.vbo);

    glBindVertexArray(context.scene.ui.labels.vao);

    glBindBuffer(GL_ARRAY_BUFFER, context.scene.ui.labels.vbo);
    glBufferData(GL_ARRAY_BUFFER, 256 * 6 * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec4_t), (void*) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vec4_t), (void*) (sizeof(vec2_t)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void _r_ui_text_draw(const char *text, float x, float y, float scale) {
    vec4_t vertices[strlen(text) * 6];
    uint32_t id = 0;

    float cx = x;
    for (uint32_t i = 0; i < strlen(text); i++) {
        float col = (float) (text[i] % 16);
        float row = (float) (text[i] / 16);

        float u_min = col / 16.0f;
        float v_min = row / 8.0f;
        float u_max = (col + 1.0f) / 16.0f;
        float v_max = (row + 1.0f) / 8.0f;

        float sx = cx;
        float sy = y;
        float w = UI_FONT_WIDTH * scale;
        float h = UI_FONT_HEIGHT * scale;

        vertices[id++] = vec4(sx, sy + h, u_min, v_max);
        vertices[id++] = vec4(sx, sy, u_min, v_min);
        vertices[id++] = vec4(sx + w, sy, u_max, v_min);

        vertices[id++] = vec4(sx, sy + h, u_min, v_max);
        vertices[id++] = vec4(sx + w, sy, u_max, v_min);
        vertices[id++] = vec4(sx + w, sy + h, u_max, v_max);

        cx += w;
    }

    glBindVertexArray(context.scene.ui.labels.vao);
    glBindBuffer(GL_ARRAY_BUFFER, context.scene.ui.labels.vbo);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, context.scene.ui.labels.texture);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glDrawArrays(GL_TRIANGLES, 0, strlen(text) * 6);

    glBindVertexArray(0);
}

void r_ui_labels_draw(void) {
    // _r_ui_text_draw("00/00/0000", (float) (1920 / 2) - (5 * 16), 16.f, 4.0f);

    char date[128];
    struct tm current_date = r_physics_clock_to_tm(context.scene.clock.time);
    snprintf(date, sizeof(date), 
        "%04d-%02d-%02d %02d:%02d:%02d mode:%d", 
        current_date.tm_year + 1900, 
        current_date.tm_mon + 1, 
        current_date.tm_mday, 
        current_date.tm_hour, 
        current_date.tm_min, 
        current_date.tm_sec, 
        context.scene.clock.cursor
    );

    _r_ui_text_draw(date, (float) (1920 / 2) - ((strlen(date) * 12) / 2), (float) (1080 / 4) * 3, 2.0f);
}

void r_ui_orbits_init(void) {
    vec3_t vertices[10 * context.scene.ui.orbits.size];
    uint32_t cursor = 0;
    for (uint32_t i = 1; i < 10; i++) {
        for (uint32_t j = 0; j < context.scene.ui.orbits.size; j++) {
            double angle = (2.0 * R_PI * j) / context.scene.ui.orbits.size;
            vec3_t position = {
                context.scene.planets[i].orbit.semi_major_axis * R_PHYSICS_ORBIT_SCALE * (cos(angle) - context.scene.planets[i].orbit.eccentricity),
                0.0f,
                context.scene.planets[i].orbit.semi_major_axis * R_PHYSICS_ORBIT_SCALE * sqrt(1.0 - context.scene.planets[i].orbit.eccentricity * context.scene.planets[i].orbit.eccentricity) * sin(angle)
            };
            vertices[cursor++] = r_physics_orbit_to_local(&context.scene.planets[i], position);
        }
    }

    glGenVertexArrays(1, &context.scene.ui.orbits.vao);
    glGenBuffers(1, &context.scene.ui.orbits.vbo);

    glBindVertexArray(context.scene.ui.orbits.vao);

    glBindBuffer(GL_ARRAY_BUFFER, context.scene.ui.orbits.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3_t), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void r_ui_orbits_draw(void) {
    uint32_t offset = 0;
    for (uint32_t i = 1; i < 10; i++) {
        r_set_mat4(context.scene.ui.orbits.shader, "u_Model", mat4(1.0f));

        r_set_vec3(context.scene.ui.orbits.shader, "u_Color", vec3(1.0f, 1.0f, 1.0f));
        // r_set_vec3(context.scene.ui.orbits.shader, "u_Color", context.scene.planets[i].color);
        r_set_float(context.scene.ui.orbits.shader, "u_Brightness", 0.2f);
        
        glBindVertexArray(context.scene.ui.orbits.vao);
        glDrawArrays(GL_LINE_LOOP, offset, context.scene.ui.orbits.size);
        
        offset += context.scene.ui.orbits.size;
    }
}

void r_ui_markers_init(void) {
    vec3_t vertices[context.scene.ui.markers.size];
    for (size_t i = 0; i < context.scene.ui.markers.size; i++) {
        double angle = (2.0 * R_PI * i) / 128;
        vertices[i] = vec3(cos(angle), 0.0f, sin(angle));
    }

    glGenVertexArrays(1, &context.scene.ui.markers.vao);
    glGenBuffers(1, &context.scene.ui.markers.vbo);

    glBindVertexArray(context.scene.ui.markers.vao);

    glBindBuffer(GL_ARRAY_BUFFER, context.scene.ui.markers.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3_t), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void r_ui_markers_draw(void) {
    for (uint32_t i = 1; i < 10; i++) {
        mat4_t model = mat4(1.0f);
        model = r_translate(model, vec3_mul(context.scene.planets[i].state.position, R_PHYSICS_ORBIT_SCALE));
        model = r_scale(model, vec3(context.scene.planets[6].radius_equatorial * R_PHYSICS_PLANET_SCALE * 1.6f, 0.0f, context.scene.planets[6].radius_equatorial * R_PHYSICS_PLANET_SCALE * 1.6f));
        r_set_mat4(context.scene.ui.orbits.shader, "u_Model", model);

        r_set_vec3(context.scene.ui.orbits.shader, "u_Color", context.scene.planets[i].color);
        r_set_float(context.scene.ui.orbits.shader, "u_Brightness", 0.8f);
        
        glBindVertexArray(context.scene.ui.markers.vao);
        glDrawArrays(GL_LINE_LOOP, 0, context.scene.ui.markers.size);
    }
}