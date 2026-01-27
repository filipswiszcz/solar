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
        // shader_t *shaders;
        // sounds?
    } renderer;

    struct {
        // planets (planet contain pointer to object and to sound)
        planet_t *planets;
        planet_t planet; // temporary
        struct {
            double time;
            double scale;
        } clock;
        // skybox (object_t)
        // ui? (here on in renderer) labels?
    } scene;

    struct {
        object_t object;
        shader_t shader;
        uint32_t texture;
        // object_t object;
        uint32_t vao, vbo;
        // float *vertices;
    } skybox;

    struct {
        vertex_t *vertices;
        uint32_t *indices;
        uint32_t vertices_size, indices_size;
        uint32_t vao, vbo, ibo;
        uint32_t texture, texture_b;
    } planet;

    shader_t shader;
    object_t object;

    struct {
        double time;
        double scale;
    } clock;

    planet_t *planets;

    struct {
        struct {
            // vec3_t segments[128];
            shader_t shader;
            // uint32_t vao, vbo, ibo;
        } orbit;
        uint32_t ring_vao, ring_vbo;
    } ui;

    // viewer
        // speed of movement, distances to other objects
        // camera attached to viewer

    // renderer
        // scenes
            // objects
                // things
                    // meshes, textures, colors
                    // other properties (weight, size, speed)
                // characters
                    // bones
                    // draw
                // particles
                    // sim
                    // draw
} context;

void g_game_init(void);

void g_game_update(void);

void g_game_stop(void);

#endif // !__G_GAME_H__