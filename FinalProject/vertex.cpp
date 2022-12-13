#pragma once

#include "vertex.h"

Vertex::Vertex() {}

Vertex::Vertex(glm::vec3 position) : 
    _position(position) {}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal) : 
    _position(position), _normal(normal) {}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords) :
    _position(position), _normal(normal), _texCoords(texCoords) {}

//Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords, glm::vec3 tangent, glm::vec3 bitangent) :
//    _position(position), _normal(normal), _texCoords(texCoords), _tangent(tangent), _bitangent(bitangent) {}
