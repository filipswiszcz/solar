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

#include "r_renderer.h"
// #include "d_util.h"
// #include "r_math.h"
#include "r_shader.h"

// xycida

static struct {
    GLFWwindow *window;

    struct {
        double time_of_last_frame;
        double time_between_frames;
        uint32_t frames;
    } fps;

    struct {
      vec3_t position;
      vec3_t target_position, head_position;
      float mouse_x, mouse_y;
      float yaw, pitch;
      float speed, sensitivity;
    } camera;

    struct {
        // object_t object;
        shader_t shader;
        uint32_t texture;
        object_t object;
        // uint32_t vao, vbo;
        // float *vertices;
    } skybox;

    struct {
        vertex_t *vertices;
        uint32_t *indices;
        uint32_t vao, vbo, ibo;
    } sol;

    shader_t shader;
    object_t object;

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