#include <algorithm>

#include "boundary.h"

void Boundary::updateControlPoints(const glm::vec3 point) {
    _bottomControlPoint.x = std::min(_bottomControlPoint.x, point.x);
    _bottomControlPoint.y = std::min(_bottomControlPoint.y, point.y);
    _bottomControlPoint.z = std::min(_bottomControlPoint.z, point.z);
    _topControlPoint.x = std::max(_topControlPoint.x, point.x);
    _topControlPoint.y = std::max(_topControlPoint.y, point.y);
    _topControlPoint.z = std::max(_topControlPoint.z, point.z);
}

bool Boundary::hit(const Ray& ray) const {
    // Test whether the ray intersects with the boundary box
    // The boundary box is defined by the bottom back left control point and the top front right control point
    // The ray is already transformed
    
    // Test x-axis
    float tmin = (_bottomControlPoint.x - ray.origin().x) / ray.direction().x;
    float tmax = (_topControlPoint.x - ray.origin().x) / ray.direction().x;
    if (tmin > tmax) std::swap(tmin, tmax);
    
    // Test y-axis
    float tymin = (_bottomControlPoint.y - ray.origin().y) / ray.direction().y;
    float tymax = (_topControlPoint.y - ray.origin().y) / ray.direction().y;
    if (tymin > tymax) std::swap(tymin, tymax);
    
    // Test z-axis
    float tzmin = (_bottomControlPoint.z - ray.origin().z) / ray.direction().z;
    float tzmax = (_topControlPoint.z - ray.origin().z) / ray.direction().z;
    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    // Test t range of x-y and get the intersection of x-y
    if ((tmin > tymax) || (tymin > tmax)) return false;
    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;
    
    // Test t range of x-y-z
    if ((tmin > tzmax) || (tzmin > tmax)) return false;
    
    return true;
}