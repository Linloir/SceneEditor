#pragma once

#include <vector>
#include <string>
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#include "mesh.h"
#include "shader.h"

class Model {
private:
    std::vector<Mesh*> _meshes;
    std::string _directory;
    bool _ready = false;

public:
    Model(std::string path);

public:
    inline bool isReady() const { return _ready; }

private:
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:
    void render(ShaderProgram shader);
};