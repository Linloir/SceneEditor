#pragma once

#include <GLM/glm.hpp>

class Camera {
private:
    glm::vec3 _position;
    glm::vec3 _direction;
    glm::vec3 _right;
    glm::vec3 _up;
    float _zoom;
    float _yawRad;
    float _pitchRad;
    float _rollRad;

private:
    glm::vec3 frontVec();
    
public:
    Camera();
    Camera(glm::vec3 position, glm::vec3 lookat);
    Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 right, glm::vec3 up);
    ~Camera();

public:
    glm::mat4 viewMatrix();
    
public:
    void move(glm::vec3 deltaVec);
    void setPosition(glm::vec3 position);
    void pitch(float deltaPitch);
    void setPitch(float pitch);
    void yaw(float deltaRad);
    void setYaw(float yaw);
    void roll(float deltaRad);
    void setRoll(float roll);
};
