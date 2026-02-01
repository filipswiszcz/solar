#ifndef __G_GAME_H__
#define __G_GAME_H__

#include <string.h>

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_GLCOREARB
    #include <GLFW/glfw3.h>
#else
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#endif

#include "d_util.h"
#include "r_physics.h"
#include "r_renderer.h"
#include "r_shader.h"

// xycida

static struct {
    GLFWwindow *window;

    struct {
        double time_of_last_frame;
        double time_between_frames;
        double time_accumulated;
        uint32_t frames; // change to counter or smth similar
    } fps;

    struct {
      vec3_t position;
      vec3_t target_position, head_position;
      float mouse_x, mouse_y;
      float yaw, pitch;
      float speed, sensitivity;
    } camera;

    struct {
        object_t *objects; // dynamic array
        // shader_t *shaders; // dynamic array
        // sounds?
    } renderer;

    struct {
        struct {
            double time;
            double scale;
        } clock;
        planet_t *planets; // planets (planet contain pointer to object and to sound)
        planet_t planet; // temporary
        struct {
            object_t object;
        } skybox;
        struct {
            struct {
                uint32_t vao, vbo;
                uint32_t size;
                shader_t shader; // make it a pointer
                uint8_t visible;
            } orbits;
            struct {
                uint32_t vao, vbo;
                uint32_t size;
                shader_t shader; // make it a pointer
                uint8_t visible;
            } markers;
            // labels
        } ui; // ui? (here or in renderer struct) labels?
    } scene;
} context;

void g_game_init(void);

void g_game_update(void);

void g_game_stop(void);

#endif // !__G_GAME_H__