#pragma once

#include <stdbool.h>

typedef struct shader {

    const char *vertexPath;
    const char *fragmentPath;

} Shader;

unsigned int RenderShaderCreate(const char *path_vert, const char *path_frag);
