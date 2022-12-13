#pragma once

#include <GLM/glm.hpp>

struct Vertex {
    glm::vec3 _position = glm::vec3(0.0f);
    glm::vec3 _normal = glm::vec3(0.0f);
    glm::vec2 _texCoords = glm::vec2(0.0f);
    //glm::vec3 _tangent = glm::vec3(0.0f);
    //glm::vec3 _bitangent = glm::vec3(0.0f);
    
    Vertex();
    Vertex(glm::vec3 position);
    Vertex(glm::vec3 position, glm::vec3 normal);
    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords);
    //Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords, glm::vec3 tangent, glm::vec3 bitangent);
};
