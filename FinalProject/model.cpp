#pragma once

#include "logger.h"
#include "model.h"

Model::Model(std::string path) {
    // Load model meta info from path
    // TODO: Implement as async task
    loadModel(path);
}

Model::~Model() {
    // TODO: Maybe delete all meshes?
}

Model::Model(std::vector<Mesh>&& meshes, std::vector<Texture>&& textures, std::string directory) {
    _meshes = std::move(meshes);
    _texturesLoaded = std::move(textures);
    _directory = directory;
    _status = LOADED;
}

void Model::loadModel(std::string path) {
    Logger::info("Loading model from path: " + path);
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        Logger::error("Failed to load model: " + std::string(importer.GetErrorString()));
        _status = ERR;
        return;
    }

    // Convert all '\' to '/'
    std::replace(path.begin(), path.end(), '\\', '/');
    _directory = path.substr(0, path.find_last_of('/'));
    
    Logger::info("Model read successfully");
    Logger::info("Processing model nodes");
    processNode(scene->mRootNode, scene);
    _status = LOADED;
    Logger::info("Model loaded");
    // 仅检查一次即可
    //checkBoundary();
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    // Process all meshes in node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(processMesh(mesh, scene));
    }
    
    // Recursively process child nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Logger::debug("Processing mesh with " + std::to_string(mesh->mNumVertices) + " vertices");
    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // Create placeholder vectors
        glm::vec3 vertexPosition = glm::vec3(0.0f);
        glm::vec3 vertexNormal = glm::vec3(0.0f);
        glm::vec2 vertexTextureCoordinate = glm::vec2(0.0f);
        glm::vec3 vertexTangent = glm::vec3(0.0f);
        glm::vec3 vertexBitangent = glm::vec3(0.0f);
        
        // Process vertex positions
        //使用循环避免代码重复，如果可行的话，可以在此循环中确定法向量等信息
        for (int j = 0; j < 3; j++) {
            vertexPosition[j] = mesh->mVertices[i][j];
            _leftBackBottomVex[j] = _leftBackBottomVex[j] < vertexPosition[j] ? _leftBackBottomVex[j] : vertexPosition[j];
            _rightFrontTopVex[j] = _rightFrontTopVex[j] > vertexPosition[j] ? _rightFrontTopVex[j] : vertexPosition[j];
        }

        //vertexPosition.x = mesh->mVertices[i].x;
        //vertexPosition.y = mesh->mVertices[i].y;
        //vertexPosition.z = mesh->mVertices[i].z;

        // Process vertex normals
        if (mesh->mNormals) {
            vertexNormal.x = mesh->mNormals[i].x;
            vertexNormal.y = mesh->mNormals[i].y;
            vertexNormal.z = mesh->mNormals[i].z;
        }

        // Process vertex texture coordinates
        if (mesh->mTextureCoords[0]) {
            // Process texture coordinates
            vertexTextureCoordinate.x = mesh->mTextureCoords[0][i].x;
            vertexTextureCoordinate.y = mesh->mTextureCoords[0][i].y;
            
            //// Process vertex tangents
            //if (mesh->mTangents) {
            //    vertexTangent.x = mesh->mTangents[i].x;
            //    vertexTangent.y = mesh->mTangents[i].y;
            //    vertexTangent.z = mesh->mTangents[i].z;
            //}
            //
            //// Process vertex bitangents
            //if (mesh->mBitangents) {
            //    vertexBitangent.x = mesh->mBitangents[i].x;
            //    vertexBitangent.y = mesh->mBitangents[i].y;
            //    vertexBitangent.z = mesh->mBitangents[i].z;
            //}
        }
        else {
            vertexTextureCoordinate = glm::vec2(0.0f, 0.0f);
        }

        // Create new vertex
        Vertex newVertex = {
            vertexPosition,
            vertexNormal,
            vertexTextureCoordinate,
            //vertexTangent,
            //vertexBitangent
        };

        // Add vertex to vertices
        vertices.push_back(newVertex);
    }
    Logger::debug("Vertices vector memory usage: " + std::to_string(vertices.size() * sizeof(Vertex) / 1024) + " KB");

    Logger::debug("Processing mesh with " + std::to_string(mesh->mNumFaces) + " faces");
    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    Logger::debug("Indices vector memory usage: " + std::to_string(indices.size() * sizeof(unsigned int) / 1024) + " KB");

    Logger::debug("Processing mesh materials");
    // Process material
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // Diffuse maps
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // Specular maps
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    Logger::debug("Textures vector memory usage: " + std::to_string(textures.size() * sizeof(Texture) / 1024) + " KB");

    Logger::debug("Mesh processed");
    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < _texturesLoaded.size(); j++) {
            if (std::strcmp(_texturesLoaded[j].path().data(), str.C_Str()) == 0) {
                textures.push_back(_texturesLoaded[j]);
                skip = true;
                Logger::debug("Texture already loaded, skipped");
                break;
            }
        }
        if (!skip) {
            Texture newTexture(textureType, _directory + '/' + str.C_Str());
            textures.push_back(newTexture);
            _texturesLoaded.push_back(newTexture);
        }
    }
    return textures;
}

void Model::render(const ShaderProgram& shader) const {
    // Test for model status
    if (_status != LOADED) {
        Logger::error("Trying to render unloaded model");
        return;
    }
    // Render the model
    for (unsigned int i = 0; i < _meshes.size(); i++) {
        _meshes[i].render(shader);
    }
}

void Model::checkBoundary() {
    for (int i = 0; i < _meshes.size(); i++) {
        for (int j = 0; j < _meshes[i].vertices().size();j++) {
            // 0,1,2 for x,y,z
            for (int k = 0; k < 3; k++) {
                _leftBackBottomVex[k] = _leftBackBottomVex[k] < _meshes[i].vertices()[j]._position[k] ?
                    _leftBackBottomVex[k] : _meshes[i].vertices()[j]._position[k];

                _rightFrontTopVex[k] = _rightFrontTopVex[k] > _meshes[i].vertices()[j]._position[k] ?
                    _rightFrontTopVex[k] : _meshes[i].vertices()[j]._position[k];
            }
        }
    }
}

Model* Model::copyToCurrentContext() const {
    // Reload all textures
    std::vector<Texture> newTextures;
    for (unsigned int i = 0; i < _texturesLoaded.size(); i++) {
        // Load texture
        Texture newTexture = Texture(_texturesLoaded[i].type(), _directory + '/' + _texturesLoaded[i].path());
        newTextures.push_back(newTexture);
    }
    
    // Copy all meshes
    std::vector<Mesh> newMeshes;
    for (unsigned int i = 0; i < _meshes.size(); i++) {
        // Copy mesh
        Mesh newMesh = Mesh(_meshes[i].vertices(), _meshes[i].indices(), newTextures);
        newMeshes.push_back(newMesh);
    }
    
    // Create new model
    Model* newModel = new Model(std::move(newMeshes), std::move(newTextures), _directory);
    return newModel;
}
