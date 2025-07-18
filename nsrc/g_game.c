#include "g_game.h"

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
        context.camera.speed = 128.0f;
    if (glfwGetKey(context.window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        context.camera.speed = 16.0f;
    // if (glfwGetKey(context.window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    //     context.camera.speed = 2.0f;
    // if (glfwGetKey(context.window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
    //     context.camera.speed = 16.0f;
    if (glfwGetKey(context.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(context.window, 1);
}

// FPS

void g_game_record_fps(void) {
    double current_time = glfwGetTime();
    context.fps.time_between_frames = (float) (current_time - context.fps.time_of_last_frame);
    context.fps.time_of_last_frame = current_time;
    context.fps.time_accumulated += context.fps.time_between_frames;
    context.fps.frames++;
    if (context.fps.time_accumulated >= 1.0f) {
        char title[64];
        sprintf(title, "%s [%d FPS]", WINDOW_NAME, context.fps.frames);
        glfwSetWindowTitle(context.window, title);
        context.fps.time_accumulated = 0.0f;
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
    r_create_program(&context.shader, "shader/default.vs", "shader/default.fs");
    r_create_program(&context.ui.orbit.shader, "shader/orbit.vs", "shader/orbit.fs");
    r_create_program(&context.skybox.shader, "shader/skybox.vs", "shader/skybox.fs");

    // TEXTURES
    context.planet.texture = d_texture_read("assets/texture/model/earth.jpg");
    context.planet.texture_b = d_texture_read("assets/texture/model/sun.png");

    char *faces[6] = {
        "assets/texture/skybox/right.png",
        "assets/texture/skybox/left.png",
        "assets/texture/skybox/top.png",
        "assets/texture/skybox/bottom.png",
        "assets/texture/skybox/front.png",
        "assets/texture/skybox/back.png"
    };
    context.skybox.texture = d_cubemap_read(faces);

    // CAMERA
    context.camera.position = vec3(0.0f, 128.0f, 512.0f);
    context.camera.target_position = vec3(0.0f, 0.0f, -1.0f);
    context.camera.head_position = vec3(0.0f, 1.0f, 0.0f);
    context.camera.yaw = -90.0f;
    context.camera.pitch = 0.0f;
    context.camera.speed = 16.0f;
    context.camera.sensitivity = 0.2f;

    // UI
    //..

    // SCENE
    // load orbs (HANDLE ENDIANNESS (and use in the report))
    char *filepath = "assets/model/earth.orb";
    FILE *file = fopen(filepath, "rb");

    ASSERT(file != NULL, "Failed to open the mesh file: %s", filepath);

    fread(&context.planet.vertices_size, sizeof(uint32_t), 1, file);
    fread(&context.planet.indices_size, sizeof(uint32_t), 1, file);

    context.planet.vertices = malloc(context.planet.vertices_size * sizeof(vertex_t));
    fread(context.planet.vertices, sizeof(vertex_t), context.planet.vertices_size, file);

    context.planet.indices = malloc(context.planet.indices_size * sizeof(uint32_t));
    fread(context.planet.indices, sizeof(uint32_t), context.planet.indices_size, file);

    glGenVertexArrays(1, &context.planet.vao);
    glGenBuffers(1, &context.planet.vbo);
    glGenBuffers(1, &context.planet.ibo); // RENDER ORB WITH TEXTURE

    glBindVertexArray(context.planet.vao);

    glBindBuffer(GL_ARRAY_BUFFER, context.planet.vbo);
    glBufferData(GL_ARRAY_BUFFER, context.planet.vertices_size * sizeof(vertex_t), context.planet.vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context.planet.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, context.planet.indices_size * sizeof(uint32_t), context.planet.indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) offsetof(vertex_t, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) offsetof(vertex_t, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*) offsetof(vertex_t, uv));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);

    // calc planets pos and rot
    context.clock.time = 0.0;
    context.clock.scale = 86400.0;

    static planet_t planets[] = {
        {.name="SUN", .radius=695700.0, .orbit={0,0,0}, .inclination=0, .node=0, .spin=25.38*R_PHYSICS_DAY_SECONDS, .tilt=0, .state={vec3(0,0,0),0,0}, .parent=NULL},
        {.name="MERCURY", .radius=2439.7, .orbit={57909227.0,87.9691*R_PHYSICS_DAY_SECONDS,252.25084*(R_PI/180.0)}, .inclination=7.005*(R_PI/180.0), .node=48.331*(R_PI/180.0), .spin=58.646*R_PHYSICS_DAY_SECONDS, .tilt=0.034*(R_PI/180.0), .state={vec3(0,0,0),0,0}, .parent=&planets[0]},
        {.name="VENUS", .radius=6051.8, .orbit={108209475.0,224.701*R_PHYSICS_DAY_SECONDS,181.97973*(R_PI/180.0)}, .inclination=3.3947*(R_PI/180.0), .node=76.680*(R_PI/180.0), .spin=-243.025*R_PHYSICS_DAY_SECONDS, .tilt=177.36*(R_PI/180.0), .state={vec3(0,0,0),0,0}, .parent=&planets[0]},
        {.name="EARTH", .radius=6371.0, .orbit={149597870.7,365.256*R_PHYSICS_DAY_SECONDS,100.46435*(R_PI/180.0)}, .inclination=0.00005*(R_PI/180.0), .node=-11.26064*(R_PI/180.0), .spin=R_PHYSICS_DAY_SECONDS, .tilt=23.44*(R_PI/180.0), .state={vec3(0,0,0),0,0}, .parent=&planets[0]},
        {.name="MARS", .radius=3389.5, .orbit={227939200.0,686.980*R_PHYSICS_DAY_SECONDS,355.45332*(R_PI/180.0)}, .inclination=1.850*(R_PI/180.0), .node=49.558*(R_PI/180.0), .spin=1.025957*R_PHYSICS_DAY_SECONDS, .tilt=25.19*(R_PI/180.0), .state={vec3(0,0,0),0,0}, .parent=&planets[0]},
        {.name="JUPITER", .radius=69911.0, .orbit={778340821.0,4332.589*R_PHYSICS_DAY_SECONDS,34.40438*(R_PI/180.0)}, .inclination=1.304*(R_PI/180.0), .node=100.464*(R_PI/180.0), .spin=9.925*3600.0, .tilt=3.13*(R_PI/180.0), .state={vec3(0,0,0),0,0}, .parent=&planets[0]},
        {.name="SATURN", .radius=58232.0, .orbit={1426666422.0,10759.22*R_PHYSICS_DAY_SECONDS,49.94432*(R_PI/180.0)}, .inclination=2.485*(R_PI/180.0), .node=113.665*(R_PI/180.0), .spin=10.656*3600.0, .tilt=26.73*(R_PI/180.0), .state={vec3(0,0,0),0,0}, .parent=&planets[0]},
        {.name="URANUS", .radius=25362.0, .orbit={2870658186.0,30685.4*R_PHYSICS_DAY_SECONDS,313.23218*(R_PI/180.0)}, .inclination=0.773*(R_PI/180.0), .node=74.006*(R_PI/180.0), .spin=-17.24*3600.0, .tilt=97.77*(R_PI/180.0), .state={vec3(0,0,0),0,0}, .parent=&planets[0]},
        {.name="NEPTUNE", .radius=24622.0, .orbit={4498396441.0,60189.0*R_PHYSICS_DAY_SECONDS,304.88003*(R_PI/180.0)}, .inclination=1.770*(R_PI/180.0), .node=131.784*(R_PI/180.0), .spin=16.11*3600.0, .tilt=28.32*(R_PI/180.0), .state={vec3(0,0,0),0,0}, .parent=&planets[0]},
        {.name="PLUTO", .radius=24622.0, .orbit={5906380000.0,90560.0*R_PHYSICS_DAY_SECONDS,238.92881*(R_PI/180.0)}, .inclination=17.16*(R_PI/180.0), .node=110.299*(R_PI/180.0), .spin=-6.387*R_PHYSICS_DAY_SECONDS, .tilt=119.61*(R_PI/180.0), .state={vec3(0,0,0),0,0}, .parent=&planets[0]}
    };
    context.planets = planets;

    for (uint32_t i = 0; i < 10; i++) {
        r_physics_state_update(&context.planets[i], context.clock.time);
    }

    // orbits
    for (uint32_t i = 0; i < 128; i++) {
        double r = (2.0 * R_PI * i) / 128;
        context.ui.orbit.segments[i].x = cos(r);
        context.ui.orbit.segments[i].y = 0.0f;
        context.ui.orbit.segments[i].z = sin(r);
    }

    glGenVertexArrays(1, &context.ui.orbit.vao);
    glGenBuffers(1, &context.ui.orbit.vbo);

    glBindVertexArray(context.ui.orbit.vao);

    glBindBuffer(GL_ARRAY_BUFFER, context.ui.orbit.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(context.ui.orbit.segments), context.ui.orbit.segments, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3_t), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

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
    // orb file contains all mesh and planet info
    // correct planet position to date
    // time fast-forward/fast-reverse 
    // each orbit has own color
    // each planet has own texture
    // each planet has label
    // sun casts light
    // camera zoom?
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
        context.clock.time += context.fps.time_between_frames * context.clock.scale;
        for (uint32_t i = 0; i < 10; i++) {
            r_physics_state_update(&context.planets[i], context.clock.time);
        }

        // objects
        glUseProgram(context.shader.program);

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, context.planet.texture);
        // r_set_int(&context.shader, "u_Texture", 0);

        mat4_t projection = r_perspective(r_radians(60.0f), (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.01f, 1000000.0f);
        r_set_mat4(&context.shader, "u_Projection", projection);
        mat4_t view = r_look_at(context.camera.position, vec3_add(context.camera.position, context.camera.target_position), context.camera.head_position);
        r_set_mat4(&context.shader, "u_View", view);
        // mat4_t model = mat4(1.0f);

        glBindVertexArray(context.planet.vao);
        for (uint32_t i = 0; i < 10; i++) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, (i == 0) ? context.planet.texture_b : context.planet.texture);
            r_set_int(&context.shader, "u_Texture", 0);

            r_set_int(&context.shader, "u_Emissive", i == 0);

            mat4_t model = mat4(1.0f);
            model = r_translate(model, vec3_mul(context.planets[i].state.position, R_PHYSICS_ORBIT_SCALE));
            float scale = (i == 0) ? 0.8f : 1.6f;
            model = r_scale(model, vec3_mul(vec3(context.planets[i].radius * scale, context.planets[i].radius * scale, context.planets[i].radius * scale), R_PHYSICS_PLANET_SCALE));
            
            r_set_mat4(&context.shader, "u_Model", model);

            glDrawElements(GL_TRIANGLES, context.planet.indices_size, GL_UNSIGNED_INT, 0);
        }

        r_set_vec3(&context.shader, "u_Light", vec3_mul(context.planets[0].state.position, R_PHYSICS_ORBIT_SCALE));

        // orbit
        glUseProgram(context.ui.orbit.shader.program);

        r_set_mat4(&context.ui.orbit.shader, "u_Projection", projection);
        r_set_mat4(&context.ui.orbit.shader, "u_View", view);

        r_set_vec3(&context.ui.orbit.shader, "u_Color", vec3(1.0f, 1.0f, 1.0f));
        r_set_float(&context.ui.orbit.shader, "u_Brightness", 0.2f);

        glBindVertexArray(context.ui.orbit.vao);
        for (uint32_t i = 1; i < 10; i++) {
            mat4_t model_orbit = mat4(1.0f);
            model_orbit = r_translate(model_orbit, vec3_mul(context.planets[i].parent -> state.position, R_PHYSICS_ORBIT_SCALE));
            // model_orbit = r_rotate(model_orbit, context.planets[i].inclination * 10.0f, vec3(1.0f, 0.0f, 0.0f));
            // model_orbit = r_rotate(model_orbit, context.planets[i].tilt * 10.0f, vec3(1.0f, 0.0f, 0.0f));
            model_orbit = r_scale(model_orbit, vec3(context.planets[i].orbit.radius * R_PHYSICS_ORBIT_SCALE, 1.0f, context.planets[i].orbit.radius * R_PHYSICS_ORBIT_SCALE));
            r_set_mat4(&context.ui.orbit.shader, "u_Model", model_orbit);
            r_set_vec3(&context.ui.orbit.shader, "u_Color", vec3(1.0f, 1.0f, 1.0f));
            r_set_float(&context.ui.orbit.shader, "u_Brightness", 0.2f);
            glDrawArrays(GL_LINE_LOOP, 0, 128);

            mat4_t model_ring = mat4(1.0f);
            model_ring = r_translate(model_ring, vec3_mul(context.planets[i].state.position, R_PHYSICS_ORBIT_SCALE));
            model_ring = r_scale(model_ring, vec3(context.planets[i].radius * R_PHYSICS_PLANET_SCALE * 6.4f, 0.0f, context.planets[i].radius * R_PHYSICS_PLANET_SCALE * 6.4f));
            r_set_mat4(&context.ui.orbit.shader, "u_Model", model_ring);
            r_set_vec3(&context.ui.orbit.shader, "u_Color", vec3(1.0f, 1.0f, 1.0f));
            r_set_float(&context.ui.orbit.shader, "u_Brightness", 2.0f);
            glDrawArrays(GL_LINE_LOOP, 0, 128);

        }

        // skybox
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        glUseProgram(context.skybox.shader.program);

        glBindVertexArray(context.skybox.object.vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, context.skybox.texture);

        // view = r_look_at(context.camera.position, vec3_add(context.camera.position, context.camera.target_position), context.camera.head_position);
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
    glfwDestroyWindow(context.window);
    glfwTerminate();
}