#pragma once

#include <GLM/glm.hpp>

class Ray {
private:
    glm::vec3 _origin = glm::vec3(0.0f);
    glm::vec3 _direction = glm::vec3(0.0f, 0.0f, -1.0f);
    
public:
    Ray() {}
    Ray(glm::vec3 origin, glm::vec3 direction) : _origin(origin), _direction(direction) {}
    
public:
    inline glm::vec3 origin() const { return _origin; }
    inline glm::vec3 direction() const { return _direction; }

public:
    Ray toLocalSpace(glm::mat4 modelMatrix) const;
};