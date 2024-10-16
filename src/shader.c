#include <glad/glad.h>
#include <stdio.h>

#include "shader.h"
#include "io.h"

unsigned int RenderShaderCreate(const char *path_vert, const char *path_frag) {
    int success;
    char log[512];

    File file_vertex = io_file_read(path_vert);
    if (!file_vertex.is_valid) {
        printf("Error reading shader: %s\n", path_vert);
        return -1;
    }

    File file_fragment = io_file_read(path_frag);
    if (!file_vertex.is_valid) {
        printf("Error reading shader: %s\n", path_frag);
        return -1;
    }

    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const char *const *)&file_vertex, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, log);
        printf("Error shader vertex compilation failed! Logs: %s\n", log);
        return -1;
    }

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const char *const *)&file_fragment, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, log);
        printf("Error shader fragment comilation failed! Log: %s\n", log);
        return -1;
    }

    // shader program
    unsigned int shader = glCreateProgram();
    glAttachShader(shader, vertex);
    glAttachShader(shader, fragment);
    glLinkProgram(shader);

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 512, NULL, log);
        printf("Error shader program linking failed. Log: %s\n", log);
        return -1;
    }

    // once shaders are linked to program there no longer needed
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    free(file_vertex.data);
    free(file_fragment.data);

    return shader;
}
