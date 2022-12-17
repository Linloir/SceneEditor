#pragma once

#include "renderable.h"

// 极值点一定在model包围盒的顶点处取到

Renderable::Renderable(Model* model) : _model(model) {}

Renderable::Renderable(Model* model, glm::vec3 position) : _model(model), _position(position) {}

void Renderable::setModel(Model* model) {
    _model = model;
}

void Renderable::move(glm::vec3 deltaVec) {
    _position += deltaVec;
}

void Renderable::setPosition(glm::vec3 position) {
    _position = position;
}

void Renderable::rotate(glm::vec3 axis, float deltaAngle) {
    _rotation = glm::rotate(_rotation, deltaAngle, axis);
}

void Renderable::setRotation(glm::vec3 axis, float angle) {
    _rotation = glm::rotate(glm::mat4(1.0f), angle, axis);
}

void Renderable::scale(float deltaScale) {
    _scale += deltaScale;
}

void Renderable::setScale(float scale) {
    _scale = glm::vec3(scale);
}

void Renderable::render(ShaderProgram shader) {
    // Check if initialized
    if (_model == nullptr) {
        Logger::error("Tries to render an uninitialized renderable object");
        return;
    }
    // Set model matrix
    shader.setUniform("model", modelMatrix());
    // Render
    _model->render(shader);
}

// check here to get global boundary
// must check before get boundary
void Renderable::check_boundary() {
    std::vector<glm::vec3> temp = {_model->get_upper_bound(),_model->get_lower_bound()};
    _lower_bound = glm::vec3(3e36, 3e36, 3e36);
    _upper_bound = -_lower_bound;
    auto model = this->modelMatrix();// 变换矩阵
    // 位运算 从000到111的每一种组合
    for (int i = 0; i < 8; i++) {
        //遍历每一个顶点
        glm::vec4 vx = glm::vec4(temp[(i & 4) >> 2][0], temp[(i & 2)>>1][1], temp[i & 1][2], 1.0f);
        auto vex = model * vx; // 经过变化之后的点的坐标
        for (int j = 0; j < 3; j++) {
            _lower_bound[j] = _lower_bound[j] < vex[j] ? _lower_bound[j] : vex[j];
            _upper_bound[j] = _upper_bound[j] > vex[j] ? _upper_bound[j] : vex[j];
        }
    }
}