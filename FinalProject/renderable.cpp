#pragma once

#include "renderable.h"

// ��ֵ��һ����model��Χ�еĶ����ȡ��

Renderable::Renderable(Model* model) : _model(model) {}

Renderable::Renderable(Model* model, glm::vec3 position) : _model(model), _position(position) {}

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
    // Set model matrix
    shader.setUniform("model", modelMatrix());
    // Render
    _model->render(shader);
}

// check here to get global boundary
void Renderable::check_boundary() {
    std::vector<glm::vec3> temp = { _lower_bound,_upper_bound };
    _lower_bound = glm::vec3(3e36, 3e36, 3e36);
    _upper_bound = -_lower_bound;
    auto model = this->modelMatrix();// �任����
    // λ���� ��000��111��ÿһ�����
    for (int i = 0; i < 7; i++) {
        //����ÿһ������
        glm::vec4 vex = glm::vec4(temp[i & 4][0], temp[i & 2][1], temp[i & 1][2],1.0f);
        vex = model * vex; // �����仯֮��ĵ������
        for (int j = 0; j < 3; j++) {
            _lower_bound[j] = _lower_bound[j] < vex[j] ? _lower_bound[j] : vex[j];
            _upper_bound[j] = _upper_bound[j] > vex[j] ? _upper_bound[j] : vex[j];
        }
    }
}