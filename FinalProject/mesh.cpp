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
    for (unsigned int i = 0; i < _textures.size(); i++) {
        OPENGL_EXTRA_FUNCTIONS->glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = _textures[i].type() == TextureType::DIFFUSE ? "texture_diffuse" : "texture_specular";
        if (_textures[i].type() == TextureType::DIFFUSE)
            number = std::to_string(diffuseNr++);
        else if (_textures[i].type() == TextureType::SPECULAR)
            number = std::to_string(specularNr++);

        OPENGL_EXTRA_FUNCTIONS->glUniform1i(OPENGL_EXTRA_FUNCTIONS->glGetUniformLocation(shader.programId(), (name + number).c_str()), i);
        _textures[i].bind();
    }
    OPENGL_EXTRA_FUNCTIONS->glActiveTexture(GL_TEXTURE0);

    _vao.bind();
    OPENGL_EXTRA_FUNCTIONS->glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
    _vao.unbind();
}

void Mesh::setupMesh() {
    _vao = VertexArrayObject(_vertices, _indices);
}
