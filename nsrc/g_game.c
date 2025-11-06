#include "d_util.h"
#include "g_game.h"
#include "r_renderer.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define WINDOW_NAME "SOLAR (Build v0.0.8)"

// INPUTS

int IS_FIRST_PLAY = 1;

void g_mouse_input() {
    double mouse_x, mouse_y;
    glfwGetCursorPos(context.window, &mouse_x, &mouse_y);

    if (IS_FIRST_PLAY) {
        context.camera.mouse_x = mouse_x;
        context.camera.mouse_y = mouse_y;
        IS_FIRST_PLAY = 0;
    }

    float offset_x = mouse_x - context.camera.mouse_x;
    float offset_y = context.camera.mouse_y - mouse_y;

    context.camera.mouse_x = mouse_x;
    context.camera.mouse_y = mouse_y;

    offset_x *= context.camera.sensitivity;
    offset_y *= context.camera.sensitivity;

    context.camera.yaw = (context.camera.yaw + offset_x);
    context.camera.pitch = (context.camera.pitch + offset_y);

    if (context.camera.pitch > 89.0f) context.camera.pitch = 89.0f;
    if (context.camera.pitch < -89.0f) context.camera.pitch = -89.0f;

    vec3_t target = {
        cos(r_radians(context.camera.yaw)) * cos(r_radians(context.camera.pitch)),
        sin(r_radians(context.camera.pitch)),
        sin(r_radians(context.camera.yaw)) * cos(r_radians(context.camera.pitch))
    };

    context.camera.target_position = r_normalize(target);
}

float CAMERA_TIME_BETWEEN_FRAMES = 0.0f;
float CAMERA_TIME_OF_LAST_FRAME = 0.0f;

void g_keyboard_input() {
    if (glfwGetKey(context.window, GLFW_KEY_W) == GLFW_PRESS)
        context.camera.position = vec3_add(context.camera.position, vec3_mul(context.camera.target_position, (context.camera.speed * CAMERA_TIME_BETWEEN_FRAMES))); 
    if (glfwGetKey(context.window, GLFW_KEY_S) == GLFW_PRESS)
        context.camera.position = vec3_sub(context.camera.position, vec3_mul(context.camera.target_position, (context.camera.speed * CAMERA_TIME_BETWEEN_FRAMES))); 
    if (glfwGetKey(context.window, GLFW_KEY_A) == GLFW_PRESS) 
        context.camera.position = vec3_sub(context.camera.position, vec3_mul(r_normalize(r_cross(context.camera.target_position, context.camera.head_position)), (context.camera.speed * CAMERA_TIME_BETWEEN_FRAMES)));
    if (glfwGetKey(context.window, GLFW_KEY_D) == GLFW_PRESS)
        context.camera.position = vec3_add(context.camera.position, vec3_mul(r_normalize(r_cross(context.camera.target_position, context.camera.head_position)), (context.camera.speed * CAMERA_TIME_BETWEEN_FRAMES)));
    if (glfwGetKey(context.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        context.camera.speed = 8.0f;
    if (glfwGetKey(context.window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        context.camera.speed = 2.0f;
    if (glfwGetKey(context.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(context.window, 1);
}

// FRAMES

float TIME_OF_LAST_FRAME = 0.0f;
int FRAMES_PER_SECOND = 0;

void g_game_frames() {
    float current_time = (float) glfwGetTime();
    FRAMES_PER_SECOND++;
    if (current_time - TIME_OF_LAST_FRAME >= 1.0) {
        char subtitle[16], title[64];
        strcpy(title, WINDOW_NAME);
        sprintf(subtitle, " [%d FPS]", (int) FRAMES_PER_SECOND);
        strcat(title, subtitle);
        glfwSetWindowTitle(context.window, title);
        FRAMES_PER_SECOND = 0;
        TIME_OF_LAST_FRAME = 1.0f;
    }
}

// GAME

void g_game_init() {
    ASSERT(glfwInit(), "Failed to initialize OpenGL");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    context.window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
    ASSERT(context.window, "Failed to initialize OpenGL window");

    glfwMakeContextCurrent(context.window);
    glfwSetInputMode(context.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    mesh_t mesh = {0};
    r_mesh_load(&mesh, "assets/default/cube.obj");

    //..
}

void g_game_loop() {
    while (!glfwWindowShouldClose(context.window)) {
        // inputs
        g_mouse_input();
        g_keyboard_input();

        // frames updater
        g_game_frames();

        // background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(context.window);
        glfwPollEvents();
    }
}

void g_game_stop() {
    glfwTerminate();
}
