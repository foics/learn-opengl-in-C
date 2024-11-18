#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mesh.h"

Mesh* createMesh(Vertex* vertices, size_t numVertices, unsigned int* indices, size_t numIndices, Texture* textures, size_t numTextures) {
    Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
    mesh->vertices = vertices;
    mesh->numVertices = numVertices;
    mesh->indices = indices;
    mesh->numIndices = numIndices;
    mesh->textures = textures;
    mesh->numTextures = numTextures;

    setupMesh(mesh);

    return mesh;
}

void setupMesh(Mesh* mesh) {
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);

    glBufferData(GL_ARRAY_BUFFER, mesh->numVertices * sizeof(Vertex), mesh->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->numIndices * sizeof(unsigned int), mesh->indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));

    glBindVertexArray(0);
}

void drawMesh(Mesh *mesh, unsigned int shader) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < mesh->numTextures; i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        const char* textureType = mesh->textures[i].type;
        char number[32];

        if (strcmp(textureType, "texture_diffuse") == 0) {
            snprintf(number, sizeof(number), "%d", diffuseNr++);
        } else if (strcmp(textureType, "texture_specular") == 0) {
            snprintf(number, sizeof(number), "%d", specularNr++);
        }

        char uniformName[64];
        snprintf(uniformName, sizeof(uniformName), "material.%s%d", textureType, number);
        glUniform1i(glGetUniformLocation(shader, uniformName), i);

        glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
    }

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_BYTE, 0);
    glBindVertexArray(0);
}
