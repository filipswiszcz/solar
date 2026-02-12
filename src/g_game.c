#include "g_game.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define WINDOW_NAME "SOLAR (Build v0.9.8)"

// FPS

void _g_game_fps_record(void) {
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

// INPUTS

void _g_game_mouse_handle(void) {
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

void _g_game_keyboard_handle(void) {
    int clock_key_h = glfwGetKey(context.window, GLFW_KEY_H);
    int clock_key_j = glfwGetKey(context.window, GLFW_KEY_J);
    int clock_key_k = glfwGetKey(context.window, GLFW_KEY_K);

    if (glfwGetKey(context.window, GLFW_KEY_W) == GLFW_PRESS)
        context.camera.position = vec3_add(context.camera.position, vec3_mul(context.camera.target_position, (context.camera.speed * context.fps.time_between_frames))); 
    if (glfwGetKey(context.window, GLFW_KEY_S) == GLFW_PRESS)
        context.camera.position = vec3_sub(context.camera.position, vec3_mul(context.camera.target_position, (context.camera.speed * context.fps.time_between_frames))); 
    if (glfwGetKey(context.window, GLFW_KEY_A) == GLFW_PRESS) 
        context.camera.position = vec3_sub(context.camera.position, vec3_mul(r_normalize(r_cross(context.camera.target_position, context.camera.head_position)), (context.camera.speed * context.fps.time_between_frames)));
    if (glfwGetKey(context.window, GLFW_KEY_D) == GLFW_PRESS)
        context.camera.position = vec3_add(context.camera.position, vec3_mul(r_normalize(r_cross(context.camera.target_position, context.camera.head_position)), (context.camera.speed * context.fps.time_between_frames)));
    
    if (clock_key_h == GLFW_PRESS && context.scene.clock.keys.h == GLFW_RELEASE) {
        if (context.scene.clock.cursor > 0) context.scene.clock.cursor--;
    }
    if (clock_key_j == GLFW_PRESS && context.scene.clock.keys.j == GLFW_RELEASE) {
        context.scene.clock.cursor = 17;
    }
    if (clock_key_k == GLFW_PRESS && context.scene.clock.keys.k == GLFW_RELEASE) {
        if (context.scene.clock.cursor < 21) context.scene.clock.cursor++;
    }

    if (glfwGetKey(context.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        context.camera.speed = 128.0f;
    if (glfwGetKey(context.window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) // release is called every loop, so thats why it breaks
        context.camera.speed = 8.0f;
    // if (glfwGetKey(context.window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    //     context.camera.speed = 2.0f;
    // if (glfwGetKey(context.window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
    //     context.camera.speed = 16.0f;

    if (glfwGetKey(context.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(context.window, 1);

    context.scene.clock.keys.h = clock_key_h;
    context.scene.clock.keys.j = clock_key_j;
    context.scene.clock.keys.k = clock_key_k;
}

// SCENE
void _g_game_clock_update(void) {
    context.scene.clock.time += context.fps.time_between_frames * context.scene.clock.speeds[context.scene.clock.cursor];
    struct tm current_date = r_physics_clock_to_tm(context.scene.clock.time);
    static double time_of_last_log = 0.0;
    time_of_last_log += context.fps.time_between_frames;
    if (time_of_last_log >= 1.0) {
        // printf("simulation_date=[%04d-%02d-%02d %02d:%02d:%02d], cursor=%d\n", 
        //     current_date.tm_year + 1900, current_date.tm_mon + 1, current_date.tm_mday, current_date.tm_hour, current_date.tm_min, current_date.tm_sec, context.scene.clock.cursor);
        time_of_last_log = 0.0;
    }
}

void _g_game_ui_orbits_init(void) {
    vec3_t vertices[10 * context.scene.ui.orbits.size];
    uint32_t cursor = 0;
    for (uint32_t i = 1; i < 10; i++) {
        for (uint32_t j = 0; j < context.scene.ui.orbits.size; j++) {
            double angle = (2.0 * R_PI * j) / context.scene.ui.orbits.size;
            vec3_t position = {
                cos(angle) * context.scene.planets[i].orbit.radius * R_PHYSICS_ORBIT_SCALE,
                0.0f,
                sin(angle) * context.scene.planets[i].orbit.radius * R_PHYSICS_ORBIT_SCALE
            };
            vertices[cursor++] = r_physics_orbit_to_local(&context.scene.planets[i], position);
        }
    }

    glGenVertexArrays(1, &context.scene.ui.orbits.vao);
    glGenBuffers(1, &context.scene.ui.orbits.vbo);

    glBindVertexArray(context.scene.ui.orbits.vao);

    glBindBuffer(GL_ARRAY_BUFFER, context.scene.ui.orbits.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3_t), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void _g_game_ui_markers_init(void) {
    vec3_t vertices[context.scene.ui.markers.size];
    for (size_t i = 0; i < context.scene.ui.markers.size; i++) {
        double angle = (2.0 * R_PI * i) / 128;
        vertices[i] = vec3(cos(angle), 0.0f, sin(angle));
    }

    glGenVertexArrays(1, &context.scene.ui.markers.vao);
    glGenBuffers(1, &context.scene.ui.markers.vbo);

    glBindVertexArray(context.scene.ui.markers.vao);

    glBindBuffer(GL_ARRAY_BUFFER, context.scene.ui.markers.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3_t), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

// GAME

void g_game_init(void) {
    // GLFW
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

    // OPENGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // ICON
    //..
    
    // MEMORY
    d_arena_init(&context.arena, GAME_MEMORY, GAME_MEMORY_CAPACITY);
    d_arena_stats(&context.arena);

    // SHADERS
    // context.renderer.shaders = calloc(3, sizeof(shader_t));
    context.renderer.shaders = d_arena_alloc(&context.arena, 3 * sizeof(shader_t));
    r_create_program(&context.renderer.shaders[0], "shader/default.vs", "shader/default.fs");
    r_create_program(&context.renderer.shaders[1], "shader/orbit.vs", "shader/orbit.fs");
    r_create_program(&context.renderer.shaders[2], "shader/skybox.vs", "shader/skybox.fs");

    // TEXTURES
    // context.renderer.textures = calloc(11, sizeof(uint32_t));
    context.renderer.textures = d_arena_alloc(&context.arena, 11 * sizeof(shader_t));
    d_util_texture_read(&context.renderer.textures[0], "assets/texture/model/sun.jpg");
    d_util_texture_read(&context.renderer.textures[1], "assets/texture/model/mercury.jpg");
    d_util_texture_read(&context.renderer.textures[2], "assets/texture/model/venus.jpg");
    d_util_texture_read(&context.renderer.textures[3], "assets/texture/model/earth.jpg");
    d_util_texture_read(&context.renderer.textures[4], "assets/texture/model/mars.jpg");
    d_util_texture_read(&context.renderer.textures[5], "assets/texture/model/jupiter.jpg");
    d_util_texture_read(&context.renderer.textures[6], "assets/texture/model/saturn.jpg");
    d_util_texture_read(&context.renderer.textures[7], "assets/texture/model/uranus.jpg");
    d_util_texture_read(&context.renderer.textures[8], "assets/texture/model/neptune.jpg");
    d_util_texture_read(&context.renderer.textures[9], "assets/texture/model/pluto.jpg");

    const char *skybox_texture_filepaths[6] = {
        "assets/texture/skybox/right.png",
        "assets/texture/skybox/left.png",
        "assets/texture/skybox/top.png",
        "assets/texture/skybox/bottom.png",
        "assets/texture/skybox/front.png",
        "assets/texture/skybox/back.png"
    };
    d_util_cubemap_read(&context.renderer.textures[10], skybox_texture_filepaths);

    // CAMERA
    context.camera.position = vec3(0.0f, 128.0f, 512.0f);
    context.camera.target_position = vec3(0.0f, 0.0f, -1.0f);
    context.camera.head_position = vec3(0.0f, 1.0f, 0.0f);
    context.camera.yaw = -90.0f;
    context.camera.pitch = 0.0f;
    context.camera.speed = 8.0f;
    context.camera.sensitivity = 0.1f;

    // SCENE
    context.scene.clock.time = 0.0; // 1 january 2000
    context.scene.clock.cursor = 17; // default 24h = 1s (- reverses time, 0 pauses, + moves forward)

    static double speeds[] = {
        -R_PHYSICS_YEAR_SECONDS, -10 * R_PHYSICS_MONTH_SECONDS, -R_PHYSICS_MONTH_SECONDS,
        -10 * R_PHYSICS_DAY_SECONDS, -R_PHYSICS_DAY_SECONDS,
        -10 * R_PHYSICS_HOUR_SECONDS, -R_PHYSICS_HOUR_SECONDS,
        -10 * R_PHYSICS_MINUTE_SECONDS, -R_PHYSICS_MINUTE_SECONDS,
        -10.0, -1.0, 1.0, 10.0,
        R_PHYSICS_MINUTE_SECONDS, 10 * R_PHYSICS_MINUTE_SECONDS,
        R_PHYSICS_HOUR_SECONDS, 10 * R_PHYSICS_HOUR_SECONDS,
        R_PHYSICS_DAY_SECONDS, 10 * R_PHYSICS_DAY_SECONDS,
        R_PHYSICS_MONTH_SECONDS, 10 * R_PHYSICS_MONTH_SECONDS, R_PHYSICS_YEAR_SECONDS
    };
    context.scene.clock.speeds = speeds;

    context.scene.clock.keys.h = GLFW_RELEASE;
    context.scene.clock.keys.j = GLFW_RELEASE;
    context.scene.clock.keys.k = GLFW_RELEASE;

    // load orbs (HANDLE ENDIANNESS (and use in the report))
    // context.renderer.objects = calloc(10, sizeof(object_t));
    context.renderer.objects = d_arena_alloc(&context.arena, 10 * sizeof(object_t));
    r_renderer_object_read(&context.arena, &context.renderer.objects[0], "assets/model/sun.orb");
    context.renderer.objects[0].shader = &context.renderer.shaders[0];
    context.renderer.objects[0].texture = &context.renderer.textures[0];
    r_renderer_object_upload(&context.renderer.objects[0]);

    r_renderer_object_read(&context.arena, &context.renderer.objects[1], "assets/model/mercury.orb");
    context.renderer.objects[1].shader = &context.renderer.shaders[0];
    context.renderer.objects[1].texture = &context.renderer.textures[1];
    r_renderer_object_upload(&context.renderer.objects[1]);

    r_renderer_object_read(&context.arena, &context.renderer.objects[2], "assets/model/venus.orb");
    context.renderer.objects[2].shader = &context.renderer.shaders[0];
    context.renderer.objects[2].texture = &context.renderer.textures[2];
    r_renderer_object_upload(&context.renderer.objects[2]);

    r_renderer_object_read(&context.arena, &context.renderer.objects[3], "assets/model/earth.orb");
    context.renderer.objects[3].shader = &context.renderer.shaders[0];
    context.renderer.objects[3].texture = &context.renderer.textures[3];
    r_renderer_object_upload(&context.renderer.objects[3]);

    r_renderer_object_read(&context.arena, &context.renderer.objects[4], "assets/model/mars.orb");
    context.renderer.objects[4].shader = &context.renderer.shaders[0];
    context.renderer.objects[4].texture = &context.renderer.textures[4];
    r_renderer_object_upload(&context.renderer.objects[4]);

    r_renderer_object_read(&context.arena, &context.renderer.objects[5], "assets/model/jupiter.orb");
    context.renderer.objects[5].shader = &context.renderer.shaders[0];
    context.renderer.objects[5].texture = &context.renderer.textures[5];
    r_renderer_object_upload(&context.renderer.objects[5]);

    r_renderer_object_read(&context.arena, &context.renderer.objects[6], "assets/model/saturn.orb");
    context.renderer.objects[6].shader = &context.renderer.shaders[0];
    context.renderer.objects[6].texture = &context.renderer.textures[6];
    r_renderer_object_upload(&context.renderer.objects[6]);

    r_renderer_object_read(&context.arena, &context.renderer.objects[7], "assets/model/uranus.orb");
    context.renderer.objects[7].shader = &context.renderer.shaders[0];
    context.renderer.objects[7].texture = &context.renderer.textures[7];
    r_renderer_object_upload(&context.renderer.objects[7]);

    r_renderer_object_read(&context.arena, &context.renderer.objects[8], "assets/model/neptune.orb");
    context.renderer.objects[8].shader = &context.renderer.shaders[0];
    context.renderer.objects[8].texture = &context.renderer.textures[8];
    r_renderer_object_upload(&context.renderer.objects[8]);

    r_renderer_object_read(&context.arena, &context.renderer.objects[9], "assets/model/pluto.orb");
    context.renderer.objects[9].shader = &context.renderer.shaders[0];
    context.renderer.objects[9].texture = &context.renderer.textures[9];
    r_renderer_object_upload(&context.renderer.objects[9]);

    d_arena_stats(&context.arena);

    // planets

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
        {.name="PLUTO", .radius=1188.3, .orbit={5906380000.0,90560.0*R_PHYSICS_DAY_SECONDS,238.92881*(R_PI/180.0)}, .inclination=17.16*(R_PI/180.0), .node=110.299*(R_PI/180.0), .spin=-6.387*R_PHYSICS_DAY_SECONDS, .tilt=119.61*(R_PI/180.0), .state={vec3(0,0,0),0,0}, .parent=&planets[0]}
    };

    for (int i = 0; i < 10; i++) {
        planets[i].object = &context.renderer.objects[i];
    }
    context.scene.planets = planets;

    // physics
    for (uint32_t i = 0; i < 10; i++) {
        r_physics_planet_state_update(&context.scene.planets[i], context.scene.clock.time);
    }

    // ui
    context.scene.ui.orbits.size = 2048;
    context.scene.ui.orbits.shader = &context.renderer.shaders[1];
    context.scene.ui.orbits.visible = 1;
    _g_game_ui_orbits_init();
    
    context.scene.ui.markers.size = 256;
    context.scene.ui.markers.shader = &context.renderer.shaders[1];
    context.scene.ui.markers.visible = 1;
    _g_game_ui_markers_init();

    // skybox
    r_renderer_object_read(&context.arena, &context.scene.skybox.object, "assets/model/default/cube.orb");
    r_renderer_object_upload(&context.scene.skybox.object);
    context.scene.skybox.object.shader = &context.renderer.shaders[2];
    context.scene.skybox.object.texture = &context.renderer.textures[2];
    r_set_int(context.scene.skybox.object.shader, "u_Map", 0);

    //..
    // orb file contains all mesh and planet info
    // correct planet position to date
    // time fast-forward/fast-reverse 
    // each orbit has own color
    // each planet has own texture
    // each planet has label
    // earth has a moon
    // camera zoom?
}

void g_game_update(void) {
    while (!glfwWindowShouldClose(context.window)) {

        // OPENGL
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // FPS
        _g_game_fps_record();

        // INPUTS
        _g_game_mouse_handle();
        _g_game_keyboard_handle();

        // SCENE
        mat4_t projection = r_perspective(r_radians(60.0f), (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.01f, 1000000.0f);
        mat4_t view = r_look_at(context.camera.position, vec3_add(context.camera.position, context.camera.target_position), context.camera.head_position);

        // clock
        _g_game_clock_update();

        // physics
        for (uint32_t i = 0; i < 10; i++) {
            r_physics_planet_state_update(&context.scene.planets[i], context.scene.clock.time);
        }

        // planets
        for (uint32_t i = 0; i < 10; i++) {
            // glBindVertexArray(context.scene.planets[i].object -> mesh.vao);
            glUseProgram(context.scene.planets[i].object -> shader -> program);

            r_set_mat4(context.scene.planets[i].object -> shader, "u_Projection", projection);
            r_set_mat4(context.scene.planets[i].object -> shader, "u_View", view);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, *context.scene.planets[i].object -> texture);

            r_set_int(context.scene.planets[i].object -> shader, "u_Texture", 0);
            r_set_int(context.scene.planets[i].object -> shader, "u_Emissive", i == 0);

            mat4_t model = mat4(1.0f);
            model = r_rotate(model, context.scene.planets[i].tilt * (180.0f / R_PI), vec3(0.0f, 0.0f, 1.0f));
            model = r_rotate(model, context.scene.planets[i].state.spin_angle * (180.0f / R_PI), vec3(0.0f, 1.0f, 0.0f));
            float scale = (i == 0) ? 0.8f : 1.6f;
            model = r_scale(model, vec3_mul(vec3(context.scene.planets[i].radius * scale, context.scene.planets[i].radius * scale, context.scene.planets[i].radius * scale), R_PHYSICS_PLANET_SCALE));
            model = r_translate(model, vec3_mul(context.scene.planets[i].state.position, R_PHYSICS_ORBIT_SCALE));

            r_set_mat4(context.scene.planets[i].object -> shader, "u_Model", model);

            r_set_vec3(context.scene.planets[i].object -> shader, "u_Light", vec3_mul(context.scene.planets[0].state.position, R_PHYSICS_ORBIT_SCALE));

            r_renderer_object_draw(context.scene.planets[i].object);
            // glDrawElements(GL_TRIANGLES, context.scene.planets[i].object -> mesh.indices.size, GL_UNSIGNED_INT, 0);
        }

        // ui
        glUseProgram(context.scene.ui.orbits.shader -> program);

        r_set_mat4(context.scene.ui.orbits.shader, "u_Projection", projection);
        r_set_mat4(context.scene.ui.orbits.shader, "u_View", view);

        uint32_t offset_orbit = 0;
        for (uint32_t i = 1; i < 10; i++) {
            // orbits
            r_set_mat4(context.scene.ui.orbits.shader, "u_Model", mat4(1.0f));

            r_set_vec3(context.scene.ui.orbits.shader, "u_Color", vec3(1.0f, 1.0f, 1.0f));
            r_set_float(context.scene.ui.orbits.shader, "u_Brightness", 0.2f);
            
            glBindVertexArray(context.scene.ui.orbits.vao);
            glDrawArrays(GL_LINE_LOOP, offset_orbit, context.scene.ui.orbits.size);
            offset_orbit += context.scene.ui.orbits.size;

            // markers
            mat4_t model_marker = mat4(1.0f);
            model_marker = r_translate(model_marker, vec3_mul(context.scene.planets[i].state.position, R_PHYSICS_ORBIT_SCALE));
            model_marker = r_scale(model_marker, vec3(context.scene.planets[6].radius * R_PHYSICS_PLANET_SCALE * 1.6f, 0.0f, context.scene.planets[6].radius * R_PHYSICS_PLANET_SCALE * 1.6f));
            r_set_mat4(context.scene.ui.orbits.shader, "u_Model", model_marker);

            r_set_vec3(context.scene.ui.orbits.shader, "u_Color", vec3(1.0f, 1.0f, 1.0f));
            r_set_float(context.scene.ui.orbits.shader, "u_Brightness", 4.0f);
            
            glBindVertexArray(context.scene.ui.markers.vao);
            glDrawArrays(GL_LINE_LOOP, 0, context.scene.ui.markers.size);
        }

        // skybox
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        glUseProgram(context.scene.skybox.object.shader -> program);

        glBindVertexArray(context.scene.skybox.object.mesh.vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, *context.scene.skybox.object.texture);

        // view = r_look_at(context.camera.position, vec3_add(context.camera.position, context.camera.target_position), context.camera.head_position);
        view.m[3][0] = view.m[3][1] = view.m[3][2] = 0.0f;
        r_set_mat4(context.scene.skybox.object.shader, "u_Projection", projection);
        r_set_mat4(context.scene.skybox.object.shader, "u_View", view);

        r_renderer_object_draw(&context.scene.skybox.object);

        // OPENGL
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        glfwSwapBuffers(context.window);
        glfwPollEvents();

    }
}

void g_game_stop(void) {

    // SCENE
    glDeleteVertexArrays(1, &context.scene.ui.orbits.vao);
    glDeleteBuffers(1, &context.scene.ui.orbits.vbo);

    glDeleteVertexArrays(1, &context.scene.ui.markers.vao);
    glDeleteBuffers(1, &context.scene.ui.markers.vbo);

    // RENDERER
    d_arena_reset(&context.arena);

    // GLFW
    glfwDestroyWindow(context.window);
    glfwTerminate();
    
}