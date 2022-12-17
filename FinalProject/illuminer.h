#pragma once

#include <GLM/glm.hpp>
#include <string>
#include <queue>

#include "shader.h"

class Illuminer {
protected:
    glm::vec3 _lightColor;

public:
    Illuminer(glm::vec3 color);
    ~Illuminer();
    
protected:
    virtual glm::vec3 ambientLightColor() const = 0;
    virtual glm::vec3 diffuseLightColor() const = 0;
    virtual glm::vec3 specularLightColor() const = 0;

    virtual void updateShader(ShaderProgram shader) const = 0;
    
public:
    glm::vec3 lightColor() const { return _lightColor; }
    void setLightColor(glm::vec3 lightColor) { _lightColor = lightColor; }
};

// Direction light
class DirLight : public Illuminer{
protected:
    glm::vec3 _direction;   // The outgoing direction of the light source
    
public:
    DirLight(glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3 color = glm::vec3(1.0f));
    ~DirLight();
    
protected:
    virtual glm::vec3 ambientLightColor() const override { return _lightColor * 0.2f; }
    virtual glm::vec3 diffuseLightColor() const override { return _lightColor * 0.5f; }
    virtual glm::vec3 specularLightColor() const override { return _lightColor * 0.9f; }
    
public:
    glm::vec3 lightDirection() const { return _direction; }  // The same direction as the outgoing direction
    void setLightDirection(glm::vec3 direction) { _direction = direction; }
    
    // Render util function
    virtual void updateShader(ShaderProgram shader) const override;
};

// Scoped Light is a combination of point light and spot light
// Shader setting is automatically configured based on the phi value
// If phi < 162.5, then the light is considered as spot light
// Otherwise it's considered as point light
class ScopedLight : public Illuminer{
protected:
    // Light source status
    glm::vec3 _position;
    glm::vec3 _direction;
    float _cutOffAngle = 180.0f;
    
    // Light property
    int _idealDistance = 32;     // ideally calculated distance 
    const float _attConstant = 1.0f;   // attenuation constant
    float _attLinear = 0.14f;   // attenuation linear term
    float _attQuadratic = 0.07f;    // attenuation quad term
    
public:
    ScopedLight(glm::vec3 position, glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3 color = glm::vec3(1.0f));
    ScopedLight(int distance, glm::vec3 position, glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3 color = glm::vec3(1.0f));
    ~ScopedLight();

private:
    // Util function
    inline void updateLinear();
    inline void updateQuadratic();
    inline float innerCutOffAngle() const;

protected:
    virtual glm::vec3 ambientLightColor() const override { return _lightColor * 0.2f; }
    virtual glm::vec3 diffuseLightColor() const override { return _lightColor * 0.5f; }
    virtual glm::vec3 specularLightColor() const override { return _lightColor * 0.9f; }

public:
    // Property setters and getters
    int idealDistance() const { return _idealDistance; }
    void setIdealDistance(int distance);
    glm::vec3 lightDirection() const { return _direction; }
    void setLightDirection(glm::vec3 direction) { _direction = direction; }
    float cutOffAngle() const { return _cutOffAngle; }
    void setCutOffAngle(float angle);

    // Render util function
    virtual void updateShader(ShaderProgram shader) const override;
};