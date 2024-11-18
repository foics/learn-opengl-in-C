#include <linmath.h>

#include "shader.h"

typedef struct vertex {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
} Vertex;

typedef struct texture {
    unsigned int id;
    const char* type;
    const char * path;
} Texture;

typedef struct mesh {
    Vertex* vertices;
    unsigned int* indices;
    Texture* textures;
    size_t numVertices;
    size_t numIndices;
    size_t numTextures;

    unsigned int VAO, VBO, EBO;
} Mesh;

Mesh* createMesh(Vertex* vertices, size_t numVertices, unsigned int* indices, size_t numIndices, Texture* textures, size_t numTextures);
void setupMesh(Mesh* mesh);
void drawMesh(Mesh* mesh, unsigned int shader);
