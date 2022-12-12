#pragma once

#include <GLM/glm.hpp>

class Camera {
public:
    inline glm::vec3 defaultOrigin() { return glm::vec3(1.0f, 1.0f, 1.0f); }
    inline glm::vec3 defaultTarget() { return glm::vec3(0.0f, 0.0f, 0.0f); }
    
private:
    // Camera state
    glm::vec3 _position;
    glm::vec3 _right;
    glm::vec3 _front;
    glm::vec3 _up;
    float _yaw = 0.0f;
    float _pitch = 0.0f;
    float _roll = 0.0f;
    float _zoom = 1.0f;

private:
    // Camera settings
    float _moveStep = 1.0f;
    
private:
    // World settings
    glm::vec3 _worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    
public:
    Camera();
    Camera(glm::vec3 position, glm::vec3 target);
    Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 right, glm::vec3 up);

public:
    inline glm::mat4 viewMatrix();
    
public:
    inline void move(glm::vec3 direction, float step);
    inline void pitch(float deltaAngle);
    inline void setPitch(float angle);
    inline void yaw(float deltaAngle);
    inline void setYaw(float angle);
    inline void roll(float deltaAngle);
    inline void setRoll(float angle);

private:
    void updateCameraState();
};

inline glm::mat4 Camera::viewMatrix() {
    return glm::lookAt(_position, _position + _front, _up);
}

inline void Camera::move(glm::vec3 direction, float step) {
    _position += direction * step;
    updateCameraState();
}

inline void Camera::pitch(float deltaAngle) {
    _pitch += deltaAngle;
    updateCameraState();
}

inline void Camera::setPitch(float angle) {
    _pitch = angle;
    updateCameraState();
}

inline void Camera::yaw(float deltaAngle) {
    _yaw += deltaAngle;
    updateCameraState();
}

inline void Camera::setYaw(float angle) {
    _yaw = angle;
    updateCameraState();
}

inline void Camera::roll(float deltaAngle) {
    _roll += deltaAngle;
    updateCameraState();
}

inline void Camera::setRoll(float angle) {
    _roll = angle;
    updateCameraState();
}
