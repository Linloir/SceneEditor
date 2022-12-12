#pragma once

#include <vector>

#include "vertex.h"

class VertexBufferObject {
private:
    unsigned int _id;

public:
    VertexBufferObject(std::vector<Vertex> vertices);
    ~VertexBufferObject();

    inline unsigned int id() const { return _id; }

    inline void bind() const;
    inline void unbind() const;
};