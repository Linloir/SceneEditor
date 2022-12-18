#pragma once

#include "renderable.h"

Renderable::Renderable(Model* model) : _model(model) {
    _boundary = model->boundBox();
}

Renderable::Renderable(Model* model, glm::vec3 position) : _model(model), _position(position) {
    _boundary = model->boundBox();
}

Renderable::~Renderable() {
    if (_light != nullptr) {
        delete _light;
        _light = nullptr;
    }
}

void Renderable::setModel(Model* model) {
    _model = model;
    _boundary = model->boundBox();
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

ScopedLight Renderable::transformedLight() const {
    // Transform the light position to the world space
    return _light->toWorldSpace(modelMatrix());
}

ScopedLight* Renderable::originalLight() const {
    return _light;
}

void Renderable::makeLight() {
    _light = new ScopedLight(glm::vec3(0.0f));
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

void Renderable::updateBoundary() {
    // Traverse every vertex in the transferred model and update the boundary
    Boundary newBoundary;
    glm::mat4 model = modelMatrix();
    for (auto& mesh : _model->meshes()) {
        for (auto& vertex : mesh.vertices()) {
            glm::vec4 transformedVertex = model * glm::vec4(vertex._position, 1.0f);
            newBoundary.updateControlPoints(glm::vec3(transformedVertex));
        }
    }
    _boundary = newBoundary;
    Logger::debug("Boundary updated");
    Logger::debug("[+] Bottom control point: " + std::to_string(newBoundary.bottomControlPoint().x) + ", " + std::to_string(newBoundary.bottomControlPoint().y) + ", " + std::to_string(newBoundary.bottomControlPoint().z));
    Logger::debug("[+] Top control point: " + std::to_string(newBoundary.topControlPoint().x) + ", " + std::to_string(newBoundary.topControlPoint().y) + ", " + std::to_string(newBoundary.topControlPoint().z));
}

HitRecord Renderable::hit(const Ray& ray) const {
    // First test if the ray hits the boundary box
    if (!_boundary.hit(ray)) {
        return HitRecord();
    }
    else {
        // If the ray hits the boundary box
        return _model->hit(ray, modelMatrix());
    }
}
