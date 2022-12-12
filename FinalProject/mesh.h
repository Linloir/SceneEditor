#pragma once

#include <vector>

#include "vertex.h"
#include "texture.h"
#include "vao.h"

class Mesh {
private:
    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;
    std::vector<Texture> _textures;

    VertexArrayObject _vao;

public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures);

public:
    inline std::vector<Vertex> vertices() const { return _vertices; }
    inline std::vector<unsigned int> indices() const { return _indices; }
    inline std::vector<Texture> textures() const { return _textures; }

    inline VertexArrayObject vao() const { return _vao; }

public:
    void render(ShaderProgram shader);
};