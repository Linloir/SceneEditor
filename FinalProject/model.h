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

    // smallest point
    glm::vec3 _left_down_back = glm::vec3(3e36f, 3e36f, 3e36f);
    // largest point
    glm::vec3 _right_up_front = -_left_down_back;

public:
    Model(std::string path);
    ~Model();

public:
    inline MODELSTATUS status() const { return _status; }


    // maybe we can check if boundary has not been set yet
    inline glm::vec3 get_upper_bound() {
        return _right_up_front;
    }
    inline glm::vec3 get_lower_bound() {
        return _left_down_back;
    }

private:
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType);

public:
    void render(const ShaderProgram& shader) const;
    void check_boundary();
};