#pragma once

#include "renderable.h"

// 极值点一定在model包围盒的顶点出取到

Renderable::Renderable(Model* model) : _model(model) {
    _upper_bound = model->get_upper_bound();
    _lower_bound = model->get_lower_bound();
}

Renderable::Renderable(Model* model, glm::vec3 position) : _model(model), _position(position) {
    _upper_bound = model->get_upper_bound();
    _lower_bound = model->get_lower_bound();
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
    // Set model matrix
    shader.setUniform("model", modelMatrix());
    // Render
    _model->render(shader);
}

// check here to get global boundary
void Renderable::check_boundary() {

}