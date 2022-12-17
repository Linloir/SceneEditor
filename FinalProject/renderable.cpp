#pragma once

#include "renderable.h"

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
void Renderable::checkBoundary() {
    std::vector<glm::vec3> temp = {_model->upperBoundVex(),_model->lowerBoundVex()};
    
    _lowerBoundVex = glm::vec3(3e36, 3e36, 3e36);
    _upperBoundVex = -_lowerBoundVex;
    
    auto model = this->modelMatrix();   // transform matrix

    // Bit calculation
    for (int i = 0; i < 8; i++) {
        glm::vec4 vx = glm::vec4(temp[(i & 4) >> 2][0], temp[(i & 2)>>1][1], temp[i & 1][2], 1.0f);
        auto vex = model * vx; // Transformed vertex position
        for (int j = 0; j < 3; j++) {
            _lowerBoundVex[j] = _lowerBoundVex[j] < vex[j] ? _lowerBoundVex[j] : vex[j];
            _upperBoundVex[j] = _upperBoundVex[j] > vex[j] ? _upperBoundVex[j] : vex[j];
        }
    }
}