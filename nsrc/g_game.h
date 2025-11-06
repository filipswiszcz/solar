#ifndef __G_GAME_H__
#define __G_GAME_H__

#include <string.h>

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>

#include "d_util.h"
#include "r_math.h"

// xycida

static struct {
    GLFWwindow *window;

    struct {
      vec3_t position;
      vec3_t target_position, head_position;
      float mouse_x, mouse_y;
      float yaw, pitch;
      float speed, sensitivity;
    } camera;

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

void g_game_init();

void g_game_loop();

void g_game_stop();

#endif // !__G_GAME_H__
