#include "g_game.h"

// UI

void r_ui_date_init(void) {}

void r_ui_labels_init(void) {}

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