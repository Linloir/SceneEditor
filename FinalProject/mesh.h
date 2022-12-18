#pragma once

#include <vector>

#include "vertex.h"
#include "texture.h"
#include "vao.h"
#include "shader.h"
#include "ray.h"
#include "hitrecord.h"

class Mesh {
private:
    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;
    std::vector<Texture> _textures;

    // we can control shininess in mesh
    float _shininess = 32.0f;

    VertexArrayObject _vao = VertexArrayObject::empty();

public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);
    Mesh(const std::vector<Vertex>& vertices, const std::vector<Texture>& textures);

public:
    inline std::vector<Vertex> vertices() const { return _vertices; }
    inline std::vector<unsigned int> indices() const { return _indices; }
    inline std::vector<Texture> textures() const { return _textures; }
    
    inline float shininess() const { return _shininess; }
    inline void setShininess(float shininess) { _shininess = shininess; }

    inline VertexArrayObject vao() const { return _vao; }

public:
    void render(const ShaderProgram& shader) const ;
    HitRecord hit(const Ray& ray, const glm::mat4& modelMatrix) const;

private:
    void setupMesh();
};
