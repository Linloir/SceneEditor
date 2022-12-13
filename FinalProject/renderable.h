#pragma once

#include <GLM/glm.hpp>
#include <GLM/ext/matrix_transform.hpp>

#include "model.h"
#include "shader.h"

class Renderable {
private:
    Model* _model;
    glm::vec3 _position = glm::vec3(0.0f);
    glm::mat4 _rotation = glm::mat4(1.0f);
    glm::vec3 _scale = glm::vec3(1.0f);
    // 下面是与坐标轴平行的顶点
    glm::vec3 _lower_bound;
    glm::vec3 _upper_bound;

public:
    Renderable(Model* model);
    Renderable(Model* model, glm::vec3 position);

public:
    void move(glm::vec3 deltaVec);
    void setPosition(glm::vec3 position);
    void rotate(glm::vec3 axis, float deltaAngle);
    void setRotation(glm::vec3 axis, float angle);
    void scale(float deltaScale);
    void setScale(float scale);

    inline glm::mat4 modelMatrix() const;

public:
    void render(ShaderProgram shader);
    // check here to get global boundary
    void check_boundary();
};

inline glm::mat4 Renderable::modelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, _position);
    model = model * _rotation;
    model = glm::scale(model, _scale);
    return model;
}
