#pragma once
#include"shader.h"
#include<GLM/glm.hpp>
// 发光物不需要纹理，所以说需要另外的shader
// 在sceneviewer中被包含
class Illuminant {
private:
    glm::vec3 _position;
    glm::vec3 _color;
public:
    Illuminant();
    Illuminant(glm::vec3 position, glm::vec3 color);

    inline glm::vec3 position();
    glm::vec3 color();

    void setPosition(glm::vec3 position);
    void setColor(glm::vec3 color);

};

inline glm::vec3 Illuminant::position() {
    return _position;
}
inline glm::vec3 Illuminant::color() {
    return _color;
}