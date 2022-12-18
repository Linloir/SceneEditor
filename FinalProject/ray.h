#pragma once

#include <GLM/glm.hpp>

class Ray {
private:
    glm::vec3 _origin;
    glm::vec3 _direction;
    
public:
    Ray() {}
    Ray(glm::vec3 origin, glm::vec3 direction) : _origin(origin), _direction(direction) {}
    
public:
    inline glm::vec3 origin() const { return _origin; }
    inline glm::vec3 direction() const { return _direction; }

    inline void setOrigin(glm::vec3 origin) { _origin = origin; }
    inline void setDirection(glm::vec3 direction) { _direction = direction; }

public:
    Ray toLocalSpace(glm::mat4 modelMatrix);
};