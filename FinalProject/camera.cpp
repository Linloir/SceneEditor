#pragma once

#include <GLM/ext/matrix_transform.hpp>

#include "camera.h"

// If no default input, set camera to (1, 1, 1) looking at (0, 0, 0)
Camera::Camera() : Camera(defaultOrigin(), defaultTarget()) {}

Camera::Camera(glm::vec3 position, glm::vec3 target) : _position(position) {
    updateCameraState();
}

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 right, glm::vec3 up) :
    _position(position), _front(direction), _right(right), _up(up)
{
    updateCameraState();
}

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

void Camera::updateCameraState() {
    // TODO: Implement vector update with roll pitch, and *roll*
}