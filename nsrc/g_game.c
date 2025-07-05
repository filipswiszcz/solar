#include "d_util.h"
#include "g_game.h"
#include "r_renderer.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define WINDOW_NAME "SOLAR (Build v0.0.8)"

// INPUTS

void g_game_handle_mouse(void) {
    double mouse_x, mouse_y;
    glfwGetCursorPos(context.window, &mouse_x, &mouse_y);

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

void g_game_handle_keyboard(void) {
    if (glfwGetKey(context.window, GLFW_KEY_W) == GLFW_PRESS)
        context.camera.position = vec3_add(context.camera.position, vec3_mul(context.camera.target_position, (context.camera.speed * context.fps.time_between_frames))); 
    if (glfwGetKey(context.window, GLFW_KEY_S) == GLFW_PRESS)
        context.camera.position = vec3_sub(context.camera.position, vec3_mul(context.camera.target_position, (context.camera.speed * context.fps.time_between_frames))); 
    if (glfwGetKey(context.window, GLFW_KEY_A) == GLFW_PRESS) 
        context.camera.position = vec3_sub(context.camera.position, vec3_mul(r_normalize(r_cross(context.camera.target_position, context.camera.head_position)), (context.camera.speed * context.fps.time_between_frames)));
    if (glfwGetKey(context.window, GLFW_KEY_D) == GLFW_PRESS)
        context.camera.position = vec3_add(context.camera.position, vec3_mul(r_normalize(r_cross(context.camera.target_position, context.camera.head_position)), (context.camera.speed * context.fps.time_between_frames)));
    if (glfwGetKey(context.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        context.camera.speed = 8.0f;
    if (glfwGetKey(context.window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        context.camera.speed = 4.0f;
    if (glfwGetKey(context.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(context.window, 1);
}

// FPS

void g_game_record_fps(void) {
    double current_time = glfwGetTime();
    context.fps.frames++;
    if (current_time - context.fps.time_of_last_frame >= 1.0f) {
        char title[64];
        sprintf(title, "%s [%d FPS]", WINDOW_NAME, context.fps.frames);
        glfwSetWindowTitle(context.window, title);
        context.fps.time_of_last_frame = current_time;
        context.fps.frames = 0;
    }
}

// GAME

void g_game_init(void) {
    // OPENGL
    QSSERT(glfwInit(), "Failed to initialize OpenGL");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    context.window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
    QSSERT(context.window, "Failed to initialize OpenGL window");

    glfwMakeContextCurrent(context.window);
    glfwSetInputMode(context.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glfwSwapInterval(0); // add frame limiter

#ifndef __APPLE__
    glewExperimental = 1;
    QSSERT(glewInit(), "Failed to initialize GLEW\n");
#endif

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // ICON
    //..

    // SHADERS
    shader_t default_shader = {0};
    r_create_program(&default_shader, "shader/default.vs", "shader/default.fs");
    context.shader = default_shader;
    shader_t skybox_shader = {0};
    r_create_program(&skybox_shader, "shader/skybox.vs", "shader/skybox.fs");
    context.skybox.shader = skybox_shader;

    // TEXTURES
    char *faces[6] = {
        "assets/texture/skybox/right.png",
        "assets/texture/skybox/left.png",
        "assets/texture/skybox/top.png",
        "assets/texture/skybox/bottom.png",
        "assets/texture/skybox/front.png",
        "assets/texture/skybox/back.png"
    };
    context.skybox.texture = d_cubemap_read(faces);

    // FPS
    context.fps.time_between_frames = 0.16f;

    // CAMERA
    context.camera.position = vec3(0.0f, 1.0f, 3.0f);
    context.camera.target_position = vec3(0.0f, 0.0f, -1.0f);
    context.camera.head_position = vec3(0.0f, 1.0f, 0.0f);
    context.camera.yaw = -90.0f;
    context.camera.pitch = 0.0f;
    context.camera.speed = 0.2f;
    context.camera.sensitivity = 0.2f;

    // UI
    //..

    // SCENE
    // load orbs (HANDLE ENDIANNESS (and use in the report))
    char *filepath = "assets/model/sphere.orb";
    FILE *file = fopen(filepath, "rb");

    ASSERT(file != NULL, "Failed to open the mesh file: %s", filepath);

    uint32_t so_vertices_size, so_indices_size;
    fread(&so_vertices_size, sizeof(uint32_t), 1, file);
    fread(&so_indices_size, sizeof(uint32_t), 1, file);
    
    printf("sol_vertices_size=%d\n", so_vertices_size);
    printf("sol_indices_size=%d\n", so_indices_size);

    context.sol.vertices = malloc(so_vertices_size * sizeof(vertex_t));
    fread(context.sol.vertices, sizeof(vertex_t), so_vertices_size, file);

    context.sol.indices = malloc(so_indices_size * sizeof(uint32_t));
    fread(context.sol.indices, sizeof(uint32_t), so_indices_size, file);
    // printf("v %f, %f, %f\n", context.sol.vertices[0].position.x, context.sol.vertices[0].position.y, context.sol.vertices[0].position.z);

    glGenVertexArrays(1, &context.sol.vao);
    glGenBuffers(1, &context.sol.vbo);
    glGenBuffers(1, &context.sol.ibo);

    glBindVertexArray(context.sol.vao);

    glBindBuffer(GL_ARRAY_BUFFER, context.sol.vbo);
    glBufferData(GL_ARRAY_BUFFER, so_vertices_size * sizeof(vertex_t), context.sol.vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context.sol.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, so_indices_size * sizeof(uint32_t), context.sol.indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) offsetof(vertex_t, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) offsetof(vertex_t, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) offsetof(vertex_t, uv));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);

    // calc planets pos and rot

    // load skybox
    mesh_t mesh = {0};
    r_mesh_read(&mesh, "assets/model/cube.obj");
    object_t object = {0};
    context.object = object;
    r_object_insert(&context.object, mesh);
    r_object_upload(&context.object);

    object_t skybox = {0};
    context.skybox.object = object;
    r_object_insert(&context.skybox.object, mesh);
    r_object_upload(&context.skybox.object);

    r_set_int(&context.skybox.shader, "skybox", 0);

    //..
}

void g_game_update(void) {
    while (!glfwWindowShouldClose(context.window)) {

        // OPENGL
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // FPS
        g_game_record_fps();

        // INPUTS
        g_game_handle_mouse();
        g_game_handle_keyboard();

        // UI
        //.. (triggers) (crosshair) (holos)

        // SCENE
        // .. (calcs) (planets) (skybox)

        // objects
        glUseProgram(context.shader.program);

        mat4_t projection = r_perspective(r_radians(60.0f), (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.0001f, 1000000.0f);
        r_set_mat4(&context.shader, "projection", projection);
        mat4_t view = r_look_at(context.camera.position, vec3_add(context.camera.position, context.camera.target_position), context.camera.head_position);
        r_set_mat4(&context.shader, "view", view);
        // mat4_t model = mat4(1.0f);
        // model = r_translate(model, vec3(-1.0f, 0.0f, 0.0f));
        // r_set_mat4(&context.shader, "model", model);
        // r_object_draw(&context.object);

        // orbs
        glBindVertexArray(context.sol.vao);

        mat4_t sun = mat4(1.0f);
        sun = r_translate(sun, vec3(0.0f, 0.0f, 0.0f));
        sun = r_scale(sun, vec3(10.0f, 10.0f, 10.0f));
        r_set_mat4(&context.shader, "model", sun);
        glDrawElements(GL_TRIANGLES, 2880, GL_UNSIGNED_INT, 0);
        // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        // glDrawElements(GL_TRIANGLES, 26358, GL_UNSIGNED_INT, 0);
        // glDrawArrays(GL_TRIANGLES, 0, 2880);

        // skybox
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        glUseProgram(context.skybox.shader.program);

        glBindVertexArray(context.skybox.object.vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, context.skybox.texture);

        // mat4_t projection = r_perspective(r_radians(45.0f), (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.1f, 100.0f);
        view = r_look_at(context.camera.position, vec3_add(context.camera.position, context.camera.target_position), context.camera.head_position);
        view.m[3][0] = view.m[3][1] = view.m[3][2] = 0.0f;
        r_set_mat4(&context.skybox.shader, "projection", projection);
        r_set_mat4(&context.skybox.shader, "view", view);

        r_object_draw(&context.skybox.object);

        // OPENGL
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        glfwSwapBuffers(context.window);
        glfwPollEvents();

    }
}

void g_game_stop(void) {
    glfwTerminate();
}