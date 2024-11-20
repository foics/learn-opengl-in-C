#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "model.h"

static Texture* cachedTextures = NULL;
static unsigned int numCachedTextures = 0;
static unsigned int sizeCachedTextures = 0;

Model loadModel(char* path) {
    Model model;
    const aiScene* scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        printf("Assimp error: %s\n", aiGetErrorString());
    }

    unsigned int index = strrchr(path, '/') - path;
    model.directory = calloc(index + 1, sizeof(char));
    strncpy(model.directory, path, index);
    model.directory[index] = '\0';

    unsigned int numMeshes = countMeshes(scene->mRootNode);
    model.meshes = calloc(numMeshes, sizeof(Mesh));
    model.numMeshes = numMeshes;

    unsigned int meshIndex = 0;
    processNode(&model, scene->mRootNode, scene, &meshIndex);

    return model;
}

unsigned int countMeshes(const aiNode* node) {
    unsigned int result = node->mNumMeshes;

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        result += countMeshes(node->mChildren[i]);
    }

    return result;
}

void processNode(Model *model, const aiNode *node, const aiScene *scene, unsigned int* meshIndex) {

    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        const aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        model->meshes[*meshIndex] = processMesh(model, mesh, scene);
        (*meshIndex)++;
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(model, node->mChildren[i], scene, meshIndex);
    }
}

Mesh processMesh(Model* model, const aiMesh* mesh, const aiScene* scene) {
    Vertex *vertices = calloc(mesh->mNumVertices, sizeof(Vertex));
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex = {0};

        vertex.Position[0] = mesh->mVertices[i].x;
        vertex.Position[1] = mesh->mVertices[i].y;
        vertex.Position[2] = mesh->mVertices[i].z;

        vertex.Normal[0] = mesh->mNormals[i].x;
        vertex.Normal[1] = mesh->mNormals[i].y;
        vertex.Normal[2] = mesh->mNormals[i].z;

        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords[0] = mesh->mTextureCoords[0][i].x;
            vertex.TexCoords[1] = mesh->mTextureCoords[0][i].y;
        }

        vertices[i] = vertex;
    }

    unsigned int numIndices = 0;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        numIndices += mesh->mFaces[i].mNumIndices;
    }

    unsigned int *indices = calloc(numIndices, sizeof(unsigned int));
    unsigned int iIndices = 0;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices[iIndices] = face.mIndices[j];
            ++iIndices;
        }
    }

    unsigned int numDiffuseMaps = 0;
    unsigned int numSpecularMaps = 0;

    Texture* diffuseMaps = NULL;
    Texture* specularMaps = NULL;

    if (scene->mNumMaterials > 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        diffuseMaps = loadMaterialTextures(model, material, aiTextureType_DIFFUSE, "texture_diffuse", &numDiffuseMaps);
        specularMaps = loadMaterialTextures(model, material, aiTextureType_SPECULAR, "texture_specular", &numSpecularMaps);
    }

    Texture* textures = NULL;
    unsigned int numTextures = numDiffuseMaps + numSpecularMaps;
    if (numTextures > 0) {
        textures = calloc(numTextures, sizeof(Texture));

        if (diffuseMaps != NULL) {
            memcpy(textures, diffuseMaps, sizeof(Texture) * numDiffuseMaps);
            free(diffuseMaps);
        }

        if (specularMaps != NULL) {
            memcpy(&textures[numDiffuseMaps], specularMaps, sizeof(Texture) * numSpecularMaps);
            free(specularMaps);
        }
    }

    Mesh result = {0};
    result.vertices = vertices;
    result.numVertices = mesh->mNumVertices;
    result.indices = indices;
    result.numIndices = numIndices;
    result.textures = textures;
    result.numTextures = numTextures;

    setupMesh(&result);
    return result;
}

Texture* loadMaterialTextures(Model* model, const aiMaterial* mat, unsigned int type, char* typeName, unsigned int* numTextures) {
    *numTextures = aiGetMaterialTextureCount(mat, type);
    if (*numTextures == 0) {
        return NULL;
    }

    if (cachedTextures == NULL) {
        sizeCachedTextures = 2;
        cachedTextures = calloc(sizeCachedTextures, sizeof(Texture));
    }

    Texture* textures = calloc(*numTextures, sizeof(Texture));

    for (unsigned int i = 0; i < *numTextures; i++) {
        aiString str;
        aiGetMaterialTexture(mat, type, i, &str, NULL, NULL, NULL, NULL, NULL, NULL);

        Texture* cachedTexture = NULL;
        for (unsigned int iCache = 0; iCache < numCachedTextures; iCache++) {
            if (strcmp(str.data, cachedTextures[iCache].path.data) == 0) {
                cachedTexture = &cachedTextures[iCache];
            }
        }

        if (cachedTexture == NULL) {

            unsigned int dirLength = strlen(model->directory);
            char* texturePath = calloc(str.length + dirLength + 2, sizeof(char));
            strcpy(texturePath, model->directory);
            strcat(texturePath, "/");
            strcat(texturePath, str.data);

            Texture texture = {0};
            texture.id = initTexture(texturePath);
            texture.type = typeName;
            texture.path = str;
            textures[i] = texture;
            free(texturePath);

            if (numCachedTextures == sizeCachedTextures) {
                sizeCachedTextures = sizeCachedTextures * 2;
                cachedTextures = realloc(cachedTextures, sizeCachedTextures * sizeof(Texture));
            }

            cachedTextures[numCachedTextures] = texture;
            numCachedTextures++;
        } else {
            textures[i] = *cachedTexture;
        }
    }

    return textures;
}

void drawModel(Model *model, unsigned int shader) {

    for (unsigned int i = 0; i < model->numMeshes; i++) {
        drawMesh(&model->meshes[i], shader);
    }

}
