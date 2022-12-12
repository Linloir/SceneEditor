#pragma once

#include "vertex.h"

Vertex::Vertex() {}

Vertex::Vertex(glm::vec3 position) : 
    _position(position) {}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal) : 
    _position(position), _normal(normal) {}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords) :
    _position(position), _normal(normal), _texCoords(texCoords) {}
