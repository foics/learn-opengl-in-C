#pragma once

#include <stdbool.h>

#include "mesh.h"

typedef struct model {
    Mesh* meshes;
    unsigned int numMeshes;
    char* directory;
} Model;

typedef struct aiScene aiScene;
typedef struct aiNode aiNode;
typedef struct aiMaterial aiMaterial;
typedef struct aiMesh aiMesh;
typedef struct aiFace aiFace;

Model loadModel(char* path);
void processNode(Model* model, const aiNode* node, const aiScene* scene, unsigned int* meshIndex);
Mesh processMesh(Model* model, const aiMesh* mesh, const aiScene* scene);
unsigned int countMeshes(const aiNode* node);
Texture* loadMaterialTextures(Model* model, const aiMaterial* mat, unsigned int type, char* typeName, unsigned int* numTextures);
void drawModel(Model* model, unsigned int shader);
