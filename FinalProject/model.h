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

private:
    Model(std::vector<Mesh>&& meshes, std::vector<Texture>&& textures, std::string directory);

public:
    inline MODELSTATUS status() const { return _status; }
    inline int meshCount() const { return _meshes.size(); }
    inline int textureCount() const { return _texturesLoaded.size(); }
    inline std::string directory() const { return _directory; }

    Model* copyToCurrentContext() const;


    // maybe we can check if boundary has not been set yet
    // Do remember to ensure you have called checkBoundary
    inline glm::vec3 get_upper_bound() {
        return _right_up_front;
    }

    // Do remember to ensure you have called checkBoundary
    inline glm::vec3 get_lower_bound() {
        return _left_down_back;
    }

private:
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType);
    // Since these are loacl position, this function should be called just once.
    void checkBoundary();

public:
    void render(const ShaderProgram& shader) const;

    

    // maybe we can encapsure a function to get both upper and lower bound?

};