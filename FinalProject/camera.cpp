#pragma once

#include <GLM/ext/matrix_transform.hpp>

#include "camera.h"

// If no default input, set camera to (1, 1, 1) looking at (0, 0, 0)
Camera::Camera() : Camera(defaultOrigin(), defaultTarget()) {}

Camera::Camera(glm::vec3 position, glm::vec3 target) : _position(position) {
    glm::vec3 direction = glm::normalize(target - position);
    glm::vec3 right = glm::normalize(glm::cross(direction, _worldUp));
    glm::vec3 up = glm::normalize(glm::cross(right, direction));
    _right = right;
    _front = direction;
    _up = up;

    _yaw = glm::degrees(glm::atan(direction.z, direction.x));
    _pitch = glm::degrees(glm::asin(direction.y));
}

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 right, glm::vec3 up) :
    _position(position), _front(direction), _right(right), _up(up)
{
    _yaw = glm::degrees(glm::atan(direction.z, direction.x));
    _pitch = glm::degrees(glm::asin(direction.y));
}

Camera::Camera(glm::vec3 position, float yaw, float pitch) :
    _position(position), _yaw(yaw), _pitch(pitch)
{
    updateCameraState();
}

void Camera::updateCameraState() {
    
    // Update front vector
    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);
    
    // Update right vector
    _right = glm::normalize(glm::cross(_front, _worldUp));
    
    // Update up vector
    _up = glm::normalize(glm::cross(_right, _front));
}