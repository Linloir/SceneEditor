#pragma once

#include <vector>

#include "vertex.h"
#include "utils.h"

class VertexBufferObject {
private:
    unsigned int _id = 0;
    std::vector<Vertex> _vertices;

public:
    VertexBufferObject();
    VertexBufferObject(const std::vector<Vertex>& vertices);
    VertexBufferObject(std::vector<Vertex>&& vertices);

    inline unsigned int id() const { return _id; }
    inline std::vector<Vertex> vertices() const { return _vertices; }
    
    inline void dispose() const;
};

inline void VertexBufferObject::dispose() const {
    OPENGL_EXTRA_FUNCTIONS->glDeleteBuffers(1, &_id);
}