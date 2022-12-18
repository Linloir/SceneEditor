#pragma once

#include <GLM/glm.hpp>
#include <GLM/ext/matrix_transform.hpp>

#include "model.h"
#include "illuminer.h"
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
    ScopedLight* _light = nullptr;
    glm::vec3 _position = glm::vec3(0.0f);
    glm::mat4 _rotation = glm::mat4(1.0f);
    glm::vec3 _scale = glm::vec3(1.0f);
    glm::vec3 _lowerBoundVex;
    glm::vec3 _upperBoundVex;

public:
    Renderable(Model* model);
    Renderable(Model* model, glm::vec3 position);
    ~Renderable();

public:
    void setModel(Model* model);
    void move(glm::vec3 deltaVec);
    void setPosition(glm::vec3 position);
    void rotate(glm::vec3 axis, float deltaAngle);
    void setRotation(glm::vec3 axis, float angle);
    void scale(float deltaScale);
    void setScale(float scale);

    ScopedLight transformedLight() const; // pass out the light object to scene manager to gather all light sources
    ScopedLight* originalLight() const; // pass out the light object to scene manager to set light attributes
    bool hasLight() const { return _light != nullptr; }
    void makeLight();   // create a light source in the object
    
    inline glm::mat4 modelMatrix() const;

    inline glm::vec3 upperBoundVex()const;
    inline glm::vec3 lowerBoundVex()const;

public:
    void render(ShaderProgram shader);
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