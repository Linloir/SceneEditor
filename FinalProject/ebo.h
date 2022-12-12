#pragma once

#include <vector>

#include "utils.h"

class ElementBufferObject
{
private:
    unsigned int _id = 0;
    std::vector<unsigned int> _indices;
    
public:
    ElementBufferObject();
    ElementBufferObject(const std::vector<unsigned int>& indices);
    ElementBufferObject(std::vector<unsigned int>&& indices);

    inline unsigned int id() const { return _id; }
    inline std::vector<unsigned int> indices() const { return _indices; }
    
    inline void dispose() const;
};

inline void ElementBufferObject::dispose() const {
    OPENGL_EXTRA_FUNCTIONS->glDeleteBuffers(1, &_id);
}