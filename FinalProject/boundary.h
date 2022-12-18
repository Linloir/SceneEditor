#pragma once

#include <GLM/glm.hpp>

#include "ray.h"
#include "utils.h"

class Boundary {
private:
    glm::vec3 _bottomControlPoint;  // left back bottom
    glm::vec3 _topControlPoint;     // right front top

public:
    Boundary() : _bottomControlPoint(glm::vec3(std::numeric_limits<float>::max())), _topControlPoint(glm::vec3(std::numeric_limits<float>::lowest())) {}
    Boundary(glm::vec3 bottomControlPoint, glm::vec3 topControlPoint) : _bottomControlPoint(bottomControlPoint), _topControlPoint(topControlPoint) {}

public:
    void updateControlPoints(const glm::vec3 point);    // update the control points of the boundary box
    
    inline glm::vec3 bottomControlPoint() const { return _bottomControlPoint; }
    inline glm::vec3 topControlPoint() const { return _topControlPoint; }
    inline glm::vec3 bottomCenterPoint() const;

    bool hit(const Ray& ray) const;

    void render() const;
};

inline glm::vec3 Boundary::bottomCenterPoint() const {
    return glm::vec3((_bottomControlPoint.x + _topControlPoint.x) / 2.0f, _bottomControlPoint.y, (_bottomControlPoint.z + _topControlPoint.z) / 2.0f);
}
