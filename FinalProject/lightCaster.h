#pragma once
#include"shader.h"
#include<GLM/glm.hpp>
// �����ﲻ��Ҫ��������˵��Ҫ�����shader
// ��sceneviewer�б�����
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