#ifndef __R_UI_H__
#define __R_UI_H__

#include "r_shader.h"

// UI

typedef struct ui {
    struct {} date; 

    struct {} labels;

    struct {
        uint32_t vao, vbo;
        uint32_t size;
        shader_t *shader;
        uint8_t visible;
    } orbits;

    struct {
        uint32_t vao, vbo;
        uint32_t size;
        shader_t *shader;
        uint8_t visible;
    } markers;
} ui_t;

void r_ui_date_init(void);

void r_ui_labels_init(void);

void r_ui_orbits_init(void);

void r_ui_orbits_draw(void);

void r_ui_markers_init(void);

void r_ui_markers_draw(void);

#endif // __R_UI_H__