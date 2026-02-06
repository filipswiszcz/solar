#include "r_shader.h"

char *_r_read_shader(char *path) {
    FILE *file = fopen(path, "r");

    VSSERT(file != NULL, NULL, "Failed to open shader file: %s\n", path);

    fseek(file, 0, SEEK_END);
    size_t fsize = ftell(file);
    rewind(file);

    char *code = (char*) malloc(fsize + 1);
    fread(code, 1, fsize, file);
    code[fsize] = '\0';
    fclose(file);

    return code;
}

uint32_t _r_compile_shader(uint32_t type, char *code) {
    uint32_t shader = glCreateShader(type);
    const char *codecopy = code;
    glShaderSource(shader, 1, &codecopy, NULL);
    glCompileShader(shader); free(code);
#ifdef DEBUG
    char info[256];
    int32_t status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(shader, 256, NULL, info);
        printf("Shader did not compile: %s\n", info); return -1;
    }
#endif
    return shader;
}

void r_create_program(shader_t *shader, char *vert_path, char *frag_path) {
    char *vert_code = _r_read_shader(vert_path);
    char *frag_code = _r_read_shader(frag_path);

    shader -> ids[0] = _r_compile_shader(GL_VERTEX_SHADER, vert_code);
    shader -> ids[1] = _r_compile_shader(GL_FRAGMENT_SHADER, frag_code);

    shader -> program = glCreateProgram();
    glAttachShader(shader -> program, shader -> ids[0]);
    glAttachShader(shader -> program, shader -> ids[1]);
    glLinkProgram(shader -> program);

#ifdef DEBUG
    char info[256];
    int32_t status;
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        glGetProgramInfoLog(prog, 256, NULL, info);
        printf("Shader did not link: %s\n", info); return -1;
    }
#endif
}

void r_set_int(shader_t *shader, char *name, int val) {
    glUniform1i(glGetUniformLocation(shader -> program, name), val);
}

void r_set_float(shader_t *shader, char *name, float val) {
    glUniform1f(glGetUniformLocation(shader -> program, name), val);
}

void r_set_vec3(shader_t *shader, char *name, vec3_t vec) {
    glUniform3f(glGetUniformLocation(shader -> program, name), vec.x, vec.y, vec.z);
}

void r_set_mat4(shader_t *shader, char *name, mat4_t mat) {
    glUniformMatrix4fv(glGetUniformLocation(shader -> program, name), 1, GL_FALSE, &mat.m[0][0]);
}