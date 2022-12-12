#pragma once

#include <GLM/glm.hpp>

class Vertex {
private:
    glm::vec3 _position;
    glm::vec3 _normal;
    glm::vec2 _texCoords;

public:
    Vertex();
    Vertex(glm::vec3 position);
    Vertex(glm::vec3 position, glm::vec3 normal);
    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords);
    ~Vertex();
    
public:
    inline glm::vec3 position() const { return _position; }
    inline glm::vec3 normal() const { return _normal; }
    inline glm::vec2 texCoords() const { return _texCoords; }
};
