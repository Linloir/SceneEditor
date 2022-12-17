#pragma once

#include "renderable.h"

// ��ֵ��һ����model��Χ�еĶ��㴦ȡ��

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
    auto model = this->modelMatrix();// �任����
    // λ���� ��000��111��ÿһ�����
    for (int i = 0; i < 8; i++) {
        //����ÿһ������
        glm::vec4 vx = glm::vec4(temp[(i & 4) >> 2][0], temp[(i & 2)>>1][1], temp[i & 1][2], 1.0f);
        auto vex = model * vx; // �����仯֮��ĵ������
        for (int j = 0; j < 3; j++) {
            _lower_bound[j] = _lower_bound[j] < vex[j] ? _lower_bound[j] : vex[j];
            _upper_bound[j] = _upper_bound[j] > vex[j] ? _upper_bound[j] : vex[j];
        }
    }
}