#pragma once

#include <GLM/glm.hpp>
#include <GLM/ext/matrix_transform.hpp>

#include "ray.h"

#define CAMERA_MAX_FOVY 90.0f
#define CAMERA_MIN_FOVY 30.0f

class Camera {
public:
    inline glm::vec3 defaultOrigin() { return glm::vec3(0.0f, 0.0f, 1.0f); }
    inline glm::vec3 defaultTarget() { return glm::vec3(0.0f, 0.0f, 0.0f); }
    
private:
    // Camera state
    glm::vec3 _position;
    glm::vec3 _right;
    glm::vec3 _front;
    glm::vec3 _up;
    float _yaw = 0.0f;
    float _pitch = 0.0f;
    float _fovy = 45.0f;
    float _nearPlane = 0.1f;
    float _farPlane = 100.0f;
    
private:
    // World settings
    glm::vec3 _worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    
public:
    Camera();
    Camera(glm::vec3 position, glm::vec3 target);
    Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 right, glm::vec3 up);
    Camera(glm::vec3 position, float yaw, float pitch);

public:
    inline glm::vec3 front() const { return _front; }
    
    inline float fovy() const { return _fovy; }
    inline float nearPlane() const { return _nearPlane; }
    inline float farPlane() const { return _farPlane; }
    
    inline glm::mat4 viewMatrix();
    inline glm::mat4 projectionMatrix(float aspectRatio);

    Ray generateRay(glm::vec2 mouseRelativePosition, float aspectRatio) const;

    inline glm::vec3 position() const { return _position; }
    
public:
    inline void move(glm::vec2 offset);
    inline void setPosition(glm::vec3 position);
    inline void pitch(float deltaAngle);
    inline void setPitch(float angle);
    inline void yaw(float deltaAngle);
    inline void setYaw(float angle);
    inline void rotate(float deltaPitchAngle, float deltaYawAngle);
    inline void setRotation(float pitchAngle, float yawAngle);
    void rotate(glm::vec3 center, float deltaPitchAngle, float deltaYawAngle);
    void setRotation(glm::vec3 center, float pitchAngle, float yawAngle);
    inline void setFovy(float fovy);
    inline void push(float distance);

private:
    void updateCameraState();
};

inline void Camera::move(glm::vec2 offset) {
    // Offset describe the movement on the xy plane in the camera's coordinate system
    // Should convert to the movement of position vector in world coordinate system
    glm::vec3 worldSpaceOffset = offset.x * _right + offset.y * _up;
    _position += worldSpaceOffset;
    updateCameraState();
}

inline void Camera::setPosition(glm::vec3 position) {
    _position = position;
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

inline void Camera::rotate(float deltaPitchAngle, float deltaYawAngle) {
    _pitch += deltaPitchAngle;
    if (_pitch > 89.0f) {
        _pitch = 89.0f;
    }
    if (_pitch < -89.0f) {
        _pitch = -89.0f;
    }
    _yaw += deltaYawAngle;
    if (_yaw > 360.0f) {
        _yaw -= 360.0f;
    }
    if (_yaw < 0.0f) {
        _yaw += 360.0f;
    }
    updateCameraState();
}

inline void Camera::setRotation(float pitchAngle, float yawAngle) {
    _pitch = pitchAngle;
    _yaw = yawAngle;
    updateCameraState();
}

inline void Camera::setFovy(float fovy) {
    _fovy = fovy;
    if (_fovy > CAMERA_MAX_FOVY) {
        _fovy = CAMERA_MAX_FOVY;
    }
    if (_fovy < CAMERA_MIN_FOVY) {
        _fovy = CAMERA_MIN_FOVY;
    }
}

inline void Camera::push(float distance) {
    _position += distance * _front;
    updateCameraState();
}

inline glm::mat4 Camera::viewMatrix() {
    return glm::lookAt(_position, _position + _front, _up);
}

inline glm::mat4 Camera::projectionMatrix(float aspectRatio) {
    return glm::perspective(glm::radians(_fovy), aspectRatio, _nearPlane, _farPlane);
}
