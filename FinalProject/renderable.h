#pragma once

#include <GLM/glm.hpp>
#include <GLM/ext/matrix_transform.hpp>
#include <GLM/gtx/quaternion.hpp>

#include "model.h"
#include "illuminer.h"
#include "shader.h"
#include "boundary.h"
#include "hitrecord.h"

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
    Boundary _boundary; // the renderable's boudary box, should be updated after a transformation is done

public:
    Renderable(Model* model);
    Renderable(Model* model, glm::vec3 position);
    ~Renderable();

public:
    glm::vec3 position() const { return _position; }
    glm::mat4 rotation() const { return _rotation; }
    glm::vec3 scaleVal() const { return _scale; }
    Model* model() const { return _model; }
    
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
    void disposeLight();    // remove the light source in the object

    const Boundary& boundary() const { return _boundary; }
    
    inline glm::mat4 modelMatrix() const;

public:
    void updateBoundary();  // update the boundary of the renderable after a transformation is done
    HitRecord hit(const Ray& ray) const;    // test the hit record of an input array
    void render(ShaderProgram shader);
};

inline glm::mat4 Renderable::modelMatrix() const {
    // Calculate rotate direction and angle from rotation matrix _rotation without glm
    glm::vec3 axis;
    float angle;
    glm::quat quat(_rotation);
    glm::vec3 axisAngle = glm::axis(quat);
    axis = glm::normalize(axisAngle);
    angle = glm::angle(quat);
    //float rotateX = glm::degrees(glm::asin(-_rotation[1][2]));
    //float rotateY = glm::degrees(glm::atan(_rotation[0][2] / _rotation[2][2]));
    //float rotateZ = glm::degrees(glm::atan(_rotation[1][0] / _rotation[1][1]));
    // Calculate model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, _position);
    model = glm::rotate(model, angle, axis);
    //model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1.0f, 0.0f, 0.0f));
    //model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0.0f, 1.0f, 0.0f));
    //model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, _scale);
    return model;
}