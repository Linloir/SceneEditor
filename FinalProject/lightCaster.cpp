#include"lightCaster.h"

Illuminant::Illuminant() {
    _position = glm::vec3(0.0f);
    _color = glm::vec3(1.0f);
}
Illuminant::Illuminant(glm::vec3 position, glm::vec3 color) {
    _position = position;
    _color = color;
}



void Illuminant::setPosition(glm::vec3 position) {
    _position = position;
}
void Illuminant::setColor(glm::vec3 color) {
    _color = color;
}