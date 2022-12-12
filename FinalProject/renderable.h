#pragma once

#include <GLM/glm.hpp>

#include "model.h"
#include "shader.h"

class Renderable {
private:
    Model* _model;
    glm::vec3 _position;
    glm::mat4 _rotation;
    glm::vec3 _scale;

public:
    Renderable(Model* model);
    Renderable(Model* model, glm::vec3 position);
    ~Renderable();

public:
    void move(glm::vec3 deltaVec);
    void setPosition(glm::vec3 deltaVec);
    void rotate(glm::vec3 axis, float angleRad);
    void setRotation(glm::vec3 axis, float angleRad);
    void scale(float deltaScale);
    void setScale(float scale);

public:
    void render(ShaderProgram shader);
};
