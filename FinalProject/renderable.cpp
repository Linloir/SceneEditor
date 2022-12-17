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
