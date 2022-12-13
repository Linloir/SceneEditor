#pragma once

#include <GLM/ext/matrix_transform.hpp>

#include "camera.h"

// If no default input, set camera to (0, 0, 1) looking at (0, 0, 0)
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

void Camera::rotate(glm::vec3 center, float deltaPitchAngle, float deltaYawAngle) {
    if (deltaPitchAngle + _pitch > 89.0f) {
        deltaPitchAngle = 89.0f - _pitch;
    }
    if (deltaPitchAngle + _pitch < -89.0f) {
        deltaPitchAngle = -89.0f - _pitch;
    }
    // Get the normalized direction vector
    glm::vec3 direction = glm::normalize(_position - center);
    // Get the original distance from the center
    float distance = glm::length(_position - center);
    // Rotate the direction vector
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(deltaPitchAngle), _right);
    rotation = glm::rotate(rotation, glm::radians(deltaYawAngle), _worldUp);
    direction = glm::normalize(glm::vec3(rotation * glm::vec4(direction, 1.0f)));
    // Get new position
    _position = center + direction * distance;
    // Update the position
    _pitch += deltaPitchAngle;
    _yaw -= deltaYawAngle;

    updateCameraState();
}

void Camera::setRotation(glm::vec3 center, float pitchAngle, float yawAngle) {
    if (pitchAngle > 89.0f) {
        pitchAngle = 89.0f;
    }
    if (pitchAngle < -89.0f) {
        pitchAngle = -89.0f;
    }
    // Get the direction vector
    glm::vec3 direction = _position - center;
    // Rotate the direction vector
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(pitchAngle), _right);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(yawAngle), _worldUp);
    direction = glm::vec3(rotationMatrix * glm::vec4(direction, 1.0f));
    // Update the position
    _position = center + direction;
    _pitch = pitchAngle;
    _yaw = yawAngle;
    
    updateCameraState();
}
