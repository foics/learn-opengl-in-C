#pragma once

#include <linmath.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"

typedef struct aiString aiString;

typedef struct vertex {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
} Vertex;

typedef struct texture {
    unsigned int id;
    char* type;
    aiString path;
} Texture;

typedef struct mesh {
    Vertex *vertices;
    size_t numVertices;
    unsigned int *indices;
    size_t numIndices;
    Texture *textures;
    size_t numTextures;

    unsigned int VAO, VBO, EBO;
} Mesh;

void setupMesh(Mesh* mesh);
void drawMesh(Mesh* mesh, unsigned int shader);
unsigned int initTexture(const char* imageName);
