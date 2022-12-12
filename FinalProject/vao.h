#pragma once

#include <vector>

#include "vbo.h"
#include "ebo.h"

class VertexArrayObject {
private:
    unsigned int _id;
    VertexBufferObject _vbo;
    ElementBufferObject _ebo;

public:
    VertexArrayObject(VertexBufferObject vbo);
    VertexArrayObject(std::vector<Vertex> vertices);
    VertexArrayObject(VertexBufferObject vbo, ElementBufferObject ebo);
    VertexArrayObject(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

    inline unsigned int id() const { return _id; }

    inline void setActive() const;
    inline void setInactive() const;

    void setVertexAttributePointer(unsigned int index, int size, unsigned int type, bool normalized, unsigned int stride, const void* pointer) const;
};