#pragma once

#include <GLM/glm.hpp>

class Camera {
private:
    // Camera state
    glm::vec3 _position;
    glm::vec3 _right;
    glm::vec3 _front;
    glm::vec3 _up;
    float _yawRad;
    float _pitchRad;
    float _rollRad;
    float _zoom;

//private:
//    // Camera settings
//    float _moveStep;
//    float _yawStep;
//    float _pitchStep;
//    float _rollStep;
    
private:
    // World settings
    glm::vec3 _worldUp;
    
public:
    Camera();
    Camera(glm::vec3 position, glm::vec3 lookat);
    Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 right, glm::vec3 up);
    ~Camera();

public:
    inline glm::mat4 viewMatrix();
    
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
