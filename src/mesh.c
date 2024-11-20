#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stb/stb_image.h>

#include "mesh.h"

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

        Texture *texture = &mesh->textures[i];
        int number = 0;

        if (strcmp("texture_diffuse", texture->type) == 0) {
            number = diffuseNr;
            ++diffuseNr;
        } else if (strcmp("texture_specular", texture->type) == 0) {
            number = specularNr;
            ++specularNr;
        }

        char uniformName[256];
        sprintf(uniformName, "material.%s%i", texture->type, number);
        glUniform1i(glGetUniformLocation(shader, uniformName), i);

        glBindTexture(GL_TEXTURE_2D, texture->id);
    }

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

unsigned int initTexture(const char* imageName) {
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(imageName, &width, &height, &nrChannels, 0);
    if (!data) {
        printf("Error: Failed to load texture %s\n", imageName);
        return 0;
    }

    GLenum format = GL_RGB;
    if (nrChannels == 1) {
        format = GL_RED;
    } else if (nrChannels == 3) {
        format = GL_RGB;
    } else if (nrChannels == 4) {
        format = GL_RGBA;
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return texture;
}
