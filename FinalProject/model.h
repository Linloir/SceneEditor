#pragma once

#include <vector>
#include <string>
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#include "mesh.h"
#include "shader.h"
#include <limits>
class Model {
public:
    enum MODELSTATUS { LOADING, LOADED, ERR };

private:
    std::vector<Mesh> _meshes;
    std::vector<Texture> _texturesLoaded;
    std::string _directory;
    MODELSTATUS _status = LOADING;

    // largest point
    glm::vec3 right_up_front = glm::vec3(3e37f, 3e37f, 3e37f);
    // smallest point
    glm::vec3 left_down_back = -right_up_front;


public:
    Model(std::string path);
    ~Model();

public:
    inline MODELSTATUS status() const { return _status; }

private:
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType);

public:
    void render(const ShaderProgram& shader) const;
};