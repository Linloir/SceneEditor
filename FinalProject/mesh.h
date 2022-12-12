#pragma once

#include <vector>

#include "vertex.h"
#include "texture.h"
#include "vao.h"
#include "shader.h"

class Mesh {
private:
    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;
    std::vector<Texture> _textures;

    VertexArrayObject _vao;

public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);
    Mesh(const std::vector<Vertex>& vertices, const std::vector<Texture>& textures);

public:
    inline std::vector<Vertex> vertices() const { return _vertices; }
    inline std::vector<unsigned int> indices() const { return _indices; }
    inline std::vector<Texture> textures() const { return _textures; }

    inline VertexArrayObject vao() const { return _vao; }

public:
    void render(const ShaderProgram& shader) const ;

private:
    void setupMesh();
};