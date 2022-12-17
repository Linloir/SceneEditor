#include "illuminer.h"
#include "logger.h"

Illuminer::Illuminer(glm::vec3 color) :
    _lightColor(color)
{}

Illuminer::~Illuminer() {}

DirLight::DirLight(glm::vec3 direction, glm::vec3 color) :
    Illuminer(color), _direction(direction)
{}

DirLight::~DirLight() {}

void DirLight::updateShader(ShaderProgram shader) const {
    // Recall DirLight structure in fragment shader
    // -------------
    // struct DirLight {
    //     vec3 direction;
    //     vec3 ambient;
    //     vec3 diffuse;
    //     vec3 specular;
    // };

    shader.setUniform("DirLight.direction", -_direction);
    shader.setUniform("DirLight.ambient", ambientLightColor());
    shader.setUniform("DirLight.diffuse", diffuseLightColor());
    shader.setUniform("DirLight.specular", specularLightColor());
}

ScopedLight::ScopedLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color) :
    Illuminer(color), _position(position), _direction(direction)
{
    updateLinear();
    updateQuadratic();
}

ScopedLight::ScopedLight(int distance, glm::vec3 position, glm::vec3 direction, glm::vec3 color) :
    Illuminer(color), _position(position), _direction(direction), _idealDistance(distance)
{
    updateLinear();
    updateQuadratic();
}

ScopedLight::~ScopedLight() {}

inline void ScopedLight::updateLinear() {
    // Double is used here to prevent precision loss
    double linear = -0.0001 + 4.7688 / (double)_idealDistance;
    _attLinear = (float)linear;
}

inline void ScopedLight::updateQuadratic() {
    // Double is used here to prevent precision loss
    double quadratic = 82.4448 * glm::pow((double)_idealDistance, -2.0192);
    _attQuadratic = (float)quadratic;
}

void ScopedLight::setIdealDistance(int distance) {
    if (distance < 10) {
        distance = 10;
    }
    if (distance > 3500) {
        distance = 3500;
    }
    _idealDistance = distance;
    updateLinear();
    updateQuadratic();
}

void ScopedLight::setCutOffAngle(float angle) {
    if (angle < 0.0f) {
        angle = 0.0f;
    }
    if (angle > 180.0f) {
        angle = 180.0f;
    }
    _cutOffAngle = angle;
}

inline float ScopedLight::innerCutOffAngle() const {
    return 0.0011 * glm::pow(_cutOffAngle, 2) + 0.6440 * _cutOffAngle;
}

void ScopedLight::updateShader(ShaderProgram shader) const {
    // Recall PointLight and SpotLight structure in fragment shader
    // -------------
    // struct PointLight {
    //     vec3 position;
    //     vec3 ambient;
    //     vec3 diffuse;
    //     vec3 specular;
    //     float constant;
    //     float linear;
    //     float quadratic;
    // };
    // ------------
    // struct SpotLight {
    //     vec3 position;
    //     vec3 direction;
    //     vec3 ambient;
    //     vec3 diffuse;
    //     vec3 specular;
    //     float constant;
    //     float linear;
    //     float quadratic;
    //     float cutOff;
    //     float outerCutOff;
    // };
    
    // Check the cutoff angle to determine the type of light
    if (abs(_cutOffAngle - 180.0f) < 1e-6) {
        // Point light
        shader.setUniform("PointLight.position", _position);
        shader.setUniform("PointLight.ambient", ambientLightColor());
        shader.setUniform("PointLight.diffuse", diffuseLightColor());
        shader.setUniform("PointLight.specular", specularLightColor());
        shader.setUniform("PointLight.constant", _attConstant);
        shader.setUniform("PointLight.linear", _attLinear);
        shader.setUniform("PointLight.quadratic", _attQuadratic);
    }
    else {
        // Spot light
        shader.setUniform("SpotLight.position", _position);
        shader.setUniform("SpotLight.direction", -_direction);
        shader.setUniform("SpotLight.ambient", ambientLightColor());
        shader.setUniform("SpotLight.diffuse", diffuseLightColor());
        shader.setUniform("SpotLight.specular", specularLightColor());
        shader.setUniform("SpotLight.constant", _attConstant);
        shader.setUniform("SpotLight.linear", _attLinear);
        shader.setUniform("SpotLight.quadratic", _attQuadratic);
        shader.setUniform("SpotLight.innercutoff", glm::cos(glm::radians(innerCutOffAngle())));
        shader.setUniform("SpotLight.outercutoff", glm::cos(glm::radians(_cutOffAngle)));
    }
}