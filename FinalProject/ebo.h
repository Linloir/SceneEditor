#pragma once

#include <vector>

class ElementBufferObject
{
private:
    unsigned int _id;
    
public:
    ElementBufferObject(std::vector<unsigned int> indices);

    inline unsigned int id() const { return _id; }

    inline void bind() const;
    inline void unbind() const;
};