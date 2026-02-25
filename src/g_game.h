#ifndef __G_GAME_H__
#define __G_GAME_H__

#include <string.h>

#include "d_arena.h"
#include "r_physics.h"
#include "r_shader.h"
#include "r_ui.h"

#define GAME_MEMORY_CAPACITY (128 * 1024 * 1024) // 128 MB
extern uint8_t GAME_MEMORY[GAME_MEMORY_CAPACITY];

// xycida

typedef struct game_context {
    GLFWwindow *window;

    struct {
        double time_of_last_frame;
        double time_between_frames;
        double time_accumulated;
        uint32_t frames;
    } fps;

    struct {
        vec3_t position;
        vec3_t target_position, head_position;
        float mouse_x, mouse_y;
        float yaw, pitch;
        float speed, sensitivity;
    } camera;

    arena_t arena;

    struct {
        object_t *objects;
        shader_t *shaders;
        uint32_t *textures;
    } renderer;

    struct {
        struct {
            double time;
            double *speeds;
            uint8_t cursor;
            struct {
                int h, j, k;
            } keys;
        } clock;

        planet_t *planets;

        struct {
            object_t object;
        } skybox;

        ui_t ui;
    } scene;

} game_context_t;

extern game_context_t context;

void g_game_init(void);

void g_game_update(void);

void g_game_stop(void);

#endif // !__G_GAME_H__