#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

#include "util/res_load.h"
#include "util/shload.h"
#include "context.h"

#include "r_shader.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define WINDOW_NAME "STARNET (BUILD v0.0.7) - Powered by XYCORP Labs"

int IS_FIRST_PLAY = 1;

float TIME_OF_LAST_FRAME_V2 = 0.0f;
int FRAMES_PER_SECOND_V2 = 0;

float TIME_OF_LAST_FRAME = 0.0f;
float TIME_BETWEEN_FRAMES = 0.0f;
float FRAMES_PER_SECOND = 0.0f;
int TEMP_FRAMES_HOLDER = 0;

float CAMERA_TIME_BETWEEN_FRAMES = 0.0f;
float CAMERA_TIME_OF_LAST_FRAME = 0.0f;

void mouse_input() {
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

    vec3 t = {
        cos(radians(context.camera.yaw)) * cos(radians(context.camera.pitch)),
        sin(radians(context.camera.pitch)),
        sin(radians(context.camera.yaw)) * cos(radians(context.camera.pitch))
    };

    context.camera.targpos = normalize(t);
}

void keyboard_input() {
    if (glfwGetKey(context.window, GLFW_KEY_W) == GLFW_PRESS)
        context.camera.pos = vec3_add(context.camera.pos, vec3_mul(context.camera.targpos, (context.camera.speed * CAMERA_TIME_BETWEEN_FRAMES)));
    if (glfwGetKey(context.window, GLFW_KEY_S) == GLFW_PRESS)
        context.camera.pos = vec3_sub(context.camera.pos, vec3_mul(context.camera.targpos, (context.camera.speed * CAMERA_TIME_BETWEEN_FRAMES)));
    if (glfwGetKey(context.window, GLFW_KEY_A) == GLFW_PRESS)
        context.camera.pos = vec3_sub(context.camera.pos, vec3_mul(normalize(cross(context.camera.targpos, context.camera.uppos)), context.camera.speed * CAMERA_TIME_BETWEEN_FRAMES));
    if (glfwGetKey(context.window, GLFW_KEY_D) == GLFW_PRESS)
        context.camera.pos = vec3_add(context.camera.pos, vec3_mul(normalize(cross(context.camera.targpos, context.camera.uppos)), context.camera.speed * CAMERA_TIME_BETWEEN_FRAMES));
    if (glfwGetKey(context.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        context.camera.speed = 5.0f;
    if (glfwGetKey(context.window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        context.camera.speed = 2.0f;
    if (glfwGetKey(context.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(context.window, true);
}

void frames_updater_v2() {
    float curr_tm = (float) glfwGetTime();
    FRAMES_PER_SECOND_V2++;
    if (curr_tm - TIME_OF_LAST_FRAME_V2 >= 1.0) {
        char fps_title[16], full_title[64];
        strcpy(full_title, WINDOW_NAME);
        sprintf(fps_title, " [%d FPS]", (int) FRAMES_PER_SECOND_V2);
        strcat(full_title, fps_title);
        glfwSetWindowTitle(context.window, full_title);
        FRAMES_PER_SECOND_V2 = 0;
        TIME_OF_LAST_FRAME_V2 += 1.0f;
    }
}

void frames_updater() { // it can be done better
    float curr_tm = (float) glfwGetTime();
    TIME_BETWEEN_FRAMES = curr_tm - TIME_OF_LAST_FRAME;
    TEMP_FRAMES_HOLDER++;
    if (TIME_BETWEEN_FRAMES > 1.0) {
        FRAMES_PER_SECOND = TEMP_FRAMES_HOLDER / TIME_BETWEEN_FRAMES;
        TEMP_FRAMES_HOLDER = 0;
        TIME_OF_LAST_FRAME = curr_tm;
        char fps_title[16], full_title[64];
        strcpy(full_title, WINDOW_NAME);
        sprintf(fps_title, " [%d FPS]", (int) FRAMES_PER_SECOND);
        strcat(full_title, fps_title);
        glfwSetWindowTitle(context.window, full_title);
    }
}

int main() {
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
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    shader_t shader;
    r_create_program(&shader, "shader/basic.fs", "shader/basic.vs");

    // shaders
    char *vshcode, *fshcode;
    shread("shader/basic.vs", &vshcode);
    shread("shader/basic.fs", &fshcode);

    uint32_t vshader = shcompile(GL_VERTEX_SHADER, vshcode);
    uint32_t fshader = shcompile(GL_FRAGMENT_SHADER, fshcode);

    uint32_t prog = shlink(vshader, fshader);

    char *lvshcode, *lfshcode;
    shread("shader/light.vs", &lvshcode);
    shread("shader/light.fs", &lfshcode);

    uint32_t lvshader = shcompile(GL_VERTEX_SHADER, lvshcode);
    uint32_t lfshader = shcompile(GL_FRAGMENT_SHADER, lfshcode);

    uint32_t lprog = shlink(lvshader, lfshader);
    // end of shaders

    // texture
    uint32_t texture;
    texture_load(texture, "assets/texture/metal.jpg");
    // end of texture

    // meshes

    // TODO
    // render a lot of meshes and later a lot of instances, check performances (does it really fucking works?)
    mesh_t mesh = {0};
    mesh_load(&mesh, "assets/default/cube.obj");

    transformation_t transf = {0};
    transf.translation = vec3(0.0f, 0.0f, 0.0f);

    instance_t instance;
    instance.mesh = &mesh;
    instance.transf = transf;

    vec3 mercury_orbit = vec3(-10.0f, 0.0f, 0.0f); // del
    float R = 10.0f, angle = 0.0f; // del

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.k * sizeof(float), mesh.vertices.values, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.k * sizeof(uint32_t), mesh.indices.values, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    // glBindVertexArray(0); // why this?
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // and this?
    // glBindBuffer(GL_ARRAY_BUFFER, 0); // and and this?

    unsigned int sun_vao;
    glGenVertexArrays(1, &sun_vao);
    glBindVertexArray(sun_vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    // end of meshes

    // camera
    context.camera.pos = vec3(0.0f, 1.0f, 3.0f);
    context.camera.targpos = vec3(0.0f, 0.0f, -1.0f);
    context.camera.uppos = vec3(0.0f, 1.0f, 0.0f);
    context.camera.yaw = -90.0f;
    context.camera.pitch = 0.0f;
    context.camera.speed = 2.0f;
    context.camera.sensitivity = 0.2f;
    // end of camera

    while (!glfwWindowShouldClose(context.window)) {
        float curr_frame_tm = (double) glfwGetTime();
        CAMERA_TIME_BETWEEN_FRAMES = curr_frame_tm - CAMERA_TIME_OF_LAST_FRAME;
        CAMERA_TIME_OF_LAST_FRAME = curr_frame_tm;

        // frames
        // frames_updater();
        frames_updater_v2();

        // inputs
        keyboard_input();
        mouse_input();

        // background
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glUseProgram(prog);

        glUniform3f(glGetUniformLocation(prog, "object_color"), 1.0f, 0.5f, 0.31f);
        glUniform3f(glGetUniformLocation(prog, "light_color"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(prog, "light_pos"), 5.0f, 0.0f, 5.0f);
        glUniform3f(glGetUniformLocation(prog, "view_pos"), context.camera.pos.x, context.camera.pos.y, context.camera.pos.z);

        mat4 projection = perspective(radians(45.0f), (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(prog, "projection"), 1, GL_FALSE, &projection.m[0][0]);

        mat4 view = get_look_at(context.camera.pos, vec3_add(context.camera.pos, context.camera.targpos), context.camera.uppos);
        glUniformMatrix4fv(glGetUniformLocation(prog, "view"), 1, GL_FALSE, &view.m[0][0]);

        glBindVertexArray(vao);
        mat4 sun_model = mat4(1.0f);
        sun_model = translate(sun_model, instance.transf.translation);
        sun_model = rotate(sun_model, (float) glfwGetTime() * 10.0f, vec3(1.0f, 0.0f, 0.0f));
        sun_model = rotate(sun_model, (float) glfwGetTime() * 5.0f, vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(prog, "model"), 1, GL_FALSE, &sun_model.m[0][0]);
        glDrawElements(GL_TRIANGLES, mesh.indices.k, GL_UNSIGNED_INT, 0);

        // orbit
        angle += (M_PI / 180);
        mercury_orbit.x = sun_model.m[3][0] + R * cos(angle);
        mercury_orbit.z = sun_model.m[3][2] + R * sin(angle);
        // end of orbit

        mat4 mercury_model = mat4(1.0f);
        mercury_model = translate(mercury_model, mercury_orbit);
        mercury_model = rotate(mercury_model, (float) glfwGetTime() * 10.0f, vec3(1.0f, 0.0f, 0.0f));
        mercury_model = rotate(mercury_model, (float) glfwGetTime() * 5.0f, vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(prog, "model"), 1, GL_FALSE, &mercury_model.m[0][0]);
        glDrawElements(GL_TRIANGLES, mesh.indices.k, GL_UNSIGNED_INT, 0);

        // light
        glUseProgram(lprog);
        glUniformMatrix4fv(glGetUniformLocation(lprog, "projection"), 1, GL_FALSE, &projection.m[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(lprog, "view"), 1, GL_FALSE, &view.m[0][0]);
        mat4 light_model = mat4(1.0f);
        light_model = translate(light_model, vec3(5.0f, 0.0f, 5.0f));
        glUniformMatrix4fv(glGetUniformLocation(lprog, "model"), 1, GL_FALSE, &light_model.m[0][0]);
        glDrawElements(GL_TRIANGLES, mesh.indices.k, GL_UNSIGNED_INT, 0);
        // end of light

        glfwSwapBuffers(context.window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(prog);
    glfwTerminate();

    return 0;
}