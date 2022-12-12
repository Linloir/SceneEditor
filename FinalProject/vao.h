#pragma once

#include <vector>

#include "vbo.h"
#include "ebo.h"

class VertexArrayObject {
private:
    unsigned int _id = 0;
    VertexBufferObject _vbo;
    ElementBufferObject _ebo;

public:
    VertexArrayObject();
    VertexArrayObject(const VertexBufferObject& vbo);
    VertexArrayObject(VertexBufferObject&& vbo);
    VertexArrayObject(const VertexBufferObject& vbo, const ElementBufferObject& ebo);
    VertexArrayObject(VertexBufferObject&& vbo, ElementBufferObject&& ebo);

    inline unsigned int id() const { return _id; }

    inline void setActive() const;
    inline void setInactive() const;

    inline void bindVertexBufferObject(const VertexBufferObject& vbo);
    inline void bindVertexBufferObject(VertexBufferObject&& vbo);
    inline void bindElementBufferObject(const ElementBufferObject& ebo);
    inline void bindElementBufferObject(ElementBufferObject&& ebo);

    inline void setVertexAttributePointer(unsigned int index, int size, unsigned int type, bool normalized, unsigned int stride, const void* pointer) const;
    inline void enableVertexAttribute(unsigned int index) const;
    inline void disableVertexAttribute(unsigned int index) const;
};