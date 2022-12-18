#pragma once

#include <GLM/glm.hpp>

class HitRecord {
private:
    bool _hitted;
    glm::vec3 _position = glm::vec3(0.0f);
    glm::vec3 _normal = glm::vec3(0.0f);

public:
    HitRecord() : _hitted(false) {}
    HitRecord(glm::vec3 position, glm::vec3 normal) : _hitted(true), _position(position), _normal(normal) {}
    
public:
    inline bool hitted() const { return _hitted; }
    inline glm::vec3 position() const { return _position; }
    inline glm::vec3 normal() const { return _normal; }
};
