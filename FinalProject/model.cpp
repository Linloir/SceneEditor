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

void Model::loadModel(std::string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        Logger::error("Failed to load model: " + std::string(importer.GetErrorString()));
        _status = ERROR;
        return;
    }
    _directory = path.substr(0, path.find_last_of('/'));
    
    processNode(scene->mRootNode, scene);
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

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // Create placeholder vectors
        glm::vec3 vertexPosition = glm::vec3(0.0f);
        glm::vec3 vertexNormal = glm::vec3(0.0f);
        glm::vec2 vertexTextureCoordinate = glm::vec2(0.0f);
        
        // Process vertex positions
        vertexPosition.x = mesh->mVertices[i].x;
        vertexPosition.y = mesh->mVertices[i].y;
        vertexPosition.z = mesh->mVertices[i].z;

        // Process vertex normals
        if (mesh->mNormals) {
            vertexNormal.x = mesh->mNormals[i].x;
            vertexNormal.y = mesh->mNormals[i].y;
            vertexNormal.z = mesh->mNormals[i].z;
        }

        // Process vertex texture coordinates
        if (mesh->mTextureCoords[0]) {
            vertexTextureCoordinate.x = mesh->mTextureCoords[0][i].x;
            vertexTextureCoordinate.y = mesh->mTextureCoords[0][i].y;
        }
        else {
            vertexTextureCoordinate = glm::vec2(0.0f, 0.0f);
        }

        // Create new vertex
        Vertex newVertex = {
            vertexPosition,
            vertexNormal,
            vertexTextureCoordinate
        };

        // Add vertex to vertices
        vertices.push_back(newVertex);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

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
