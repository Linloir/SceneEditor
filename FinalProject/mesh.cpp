#pragma once

#include "mesh.h"
#include "utils.h"
#include "logger.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures) {
    _vertices = vertices;
    _indices = indices;
    _textures = textures;

    setupMesh();
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<Texture>& textures) {
    _vertices = vertices;
    _textures = textures;

    setupMesh();
}

void Mesh::render(const ShaderProgram& shader) const {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    shader.setUniform("material.shininess",_shininess);
    for (int i = 0; i < _textures.size(); i++) {
        OPENGL_EXTRA_FUNCTIONS->glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = _textures[i].type() == TextureType::DIFFUSE ? "texture_diffuse" : "texture_specular";
        name = "material." + name;
        
        if (_textures[i].type() == TextureType::DIFFUSE)
            number = std::to_string(diffuseNr++);
        else if (_textures[i].type() == TextureType::SPECULAR)
            number = std::to_string(specularNr++);

        shader.setUniform(name + number, i);
        _textures[i].bind();
    }
    OPENGL_EXTRA_FUNCTIONS->glActiveTexture(GL_TEXTURE0);

    _vao.bind();
    OPENGL_EXTRA_FUNCTIONS->glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
    _vao.unbind();
}

void Mesh::setupMesh() {
    _vao = VertexArrayObject(_vertices, _indices);
    _vao.setVertexAttributePointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    _vao.setVertexAttributePointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)OFFSETOF(Vertex, _normal));
    _vao.setVertexAttributePointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)OFFSETOF(Vertex, _texCoords));
    //_vao.setVertexAttributePointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)OFFSETOF(Vertex, _tangent));
    //_vao.setVertexAttributePointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)OFFSETOF(Vertex, _bitangent));
}
