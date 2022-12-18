#include "ray.h"

Ray Ray::toLocalSpace(glm::mat4 modelMatrix) {
    // The model matrix is the matrix that transforms local space to world space
    // Therefore the inverse of the model matrix transforms world space to local space
    glm::mat4 inversedModelMatrix = glm::inverse(modelMatrix);
    
    // Transform the origin and direction of the ray to local space
    glm::vec4 localOrigin = inversedModelMatrix * glm::vec4(_origin, 1.0f);
    glm::vec4 localDirection = inversedModelMatrix * glm::vec4(_direction, 0.0f);
    
    // Return the new ray
    return Ray(glm::vec3(localOrigin), glm::vec3(localDirection));
}