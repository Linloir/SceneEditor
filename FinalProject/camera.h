#pragma once

#include <GLM/glm.hpp>
#include <GLM/ext/matrix_transform.hpp>

#define CAMERA_MAX_ZOOM 90.0f
#define CAMERA_MIN_ZOOM 1.0f

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
    float _zoom = 45.0f;
    
private:
    // World settings
    glm::vec3 _worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    
public:
    Camera();
    Camera(glm::vec3 position, glm::vec3 target);
    Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 right, glm::vec3 up);
    Camera(glm::vec3 position, float yaw, float pitch);

public:
    inline float zoomVal() const { return _zoom; }
    inline glm::mat4 viewMatrix();
    
public:
    inline void move(glm::vec2 offset);
    inline void setPosition(glm::vec3 position);
    inline void pitch(float deltaAngle);
    inline void setPitch(float angle);
    inline void yaw(float deltaAngle);
    inline void setYaw(float angle);
    inline void rotate(float deltaPitchAngle, float deltaYawAngle);
    inline void setRotation(float pitchAngle, float yawAngle);
    inline void zoom(float deltaZoom);
    inline void setZoom(float zoom);
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
    _yaw += deltaYawAngle;
    updateCameraState();
}

inline void Camera::setRotation(float pitchAngle, float yawAngle) {
    _pitch = pitchAngle;
    _yaw = yawAngle;
    updateCameraState();
}

inline void Camera::zoom(float deltaZoom) {
    _zoom += deltaZoom;
    if (_zoom > CAMERA_MAX_ZOOM) {
        _zoom = CAMERA_MAX_ZOOM;
    }
    else if (_zoom < CAMERA_MIN_ZOOM) {
        _zoom = CAMERA_MIN_ZOOM;
    }
    updateCameraState();
}

inline void Camera::setZoom(float zoom) {
    _zoom = zoom;
    if (_zoom > CAMERA_MAX_ZOOM) {
        _zoom = CAMERA_MAX_ZOOM;
    }
    else if (_zoom < CAMERA_MIN_ZOOM) {
        _zoom = CAMERA_MIN_ZOOM;
    }
    updateCameraState();
}

inline void Camera::push(float distance) {
    _position += distance * _front;
    updateCameraState();
}

inline glm::mat4 Camera::viewMatrix() {
    return glm::lookAt(_position, _position + _front, _up);
}
