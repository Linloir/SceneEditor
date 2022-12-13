#pragma once

#include <vector>

#include "vertex.h"
#include "utils.h"
#include "logger.h"

class VertexBufferObject {
public:
    static VertexBufferObject empty() {
        return VertexBufferObject();
    }
    
private:
    unsigned int _id = 0;

private:
    VertexBufferObject();
    
public:
    VertexBufferObject(const std::vector<Vertex>& vertices);

    inline unsigned int id() const { return _id; }
    
    inline void bind() const;
    inline void unbind() const;
    inline void dispose();
    inline void ensureInitialized();
    
    void setBuffer(const std::vector<Vertex>& vertices);
};

inline void VertexBufferObject::bind() const {
    if (_id == 0) {
        Logger::error("Binding an invalid VertexBufferObject");
        return;
    }
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, _id);
}

inline void VertexBufferObject::unbind() const {
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline void VertexBufferObject::dispose() {
    if (_id == 0) return;
    OPENGL_EXTRA_FUNCTIONS->glDeleteBuffers(1, &_id);
    _id = 0;
}

inline void VertexBufferObject::ensureInitialized() {
    if (_id == 0) {
        OPENGL_EXTRA_FUNCTIONS->glGenBuffers(1, &_id);
        if (_id == 0) {
            Logger::error("Failed to create VertexBufferObject");
        }
    }
}