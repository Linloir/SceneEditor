#pragma once

#include <GLM/glm.hpp>
#include <GLM/ext/matrix_transform.hpp>

#include "model.h"
#include "shader.h"

class Renderable {
public:
    static Renderable empty() {
        return Renderable();
    }

private:
    Renderable() {}

private:
    Model* _model = nullptr;
    glm::vec3 _position = glm::vec3(0.0f);
    glm::mat4 _rotation = glm::mat4(1.0f);
    glm::vec3 _scale = glm::vec3(1.0f);
    glm::vec3 _lowerBoundVex;
    glm::vec3 _upperBoundVex;

public:
    Renderable(Model* model);
    Renderable(Model* model, glm::vec3 position);

public:
    void setModel(Model* model);
    void move(glm::vec3 deltaVec);
    void setPosition(glm::vec3 position);
    void rotate(glm::vec3 axis, float deltaAngle);
    void setRotation(glm::vec3 axis, float angle);
    void scale(float deltaScale);
    void setScale(float scale);

    inline glm::mat4 modelMatrix() const;

    inline glm::vec3 upperBoundVex()const;
    inline glm::vec3 lowerBoundVex()const;

public:
    void render(ShaderProgram shader);
    // check here to get global boundary
    void checkBoundary();
};

inline glm::mat4 Renderable::modelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, _position);
    model = _rotation * model;
    model = glm::scale(model, _scale);
    return model;
}

inline glm::vec3 Renderable::lowerBoundVex() const {
    return _lowerBoundVex;
}
inline glm::vec3 Renderable::upperBoundVex() const {
    return _upperBoundVex;
}