#pragma once

#include <vector>
#include <string>
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#include "mesh.h"
#include "shader.h"

class Model {
public:
    enum MODELSTATUS { LOADING, LOADED, ERR };

private:
    std::vector<Mesh> _meshes;
    std::vector<Texture> _texturesLoaded;
    std::string _directory;
    MODELSTATUS _status = LOADING;



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