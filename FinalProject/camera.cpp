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

void Camera::updateCameraState() {
    // TODO: Implement vector update with roll pitch, and *roll*
}