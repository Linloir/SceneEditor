#pragma once

#include <vector>

#include "utils.h"
#include "logger.h"

class ElementBufferObject
{
public:
    static ElementBufferObject empty() {
        return ElementBufferObject();
    }
    
private:
    unsigned int _id = 0;
    
private:
    ElementBufferObject();
    
public:
    ElementBufferObject(const std::vector<unsigned int>& indices);

    inline unsigned int id() const { return _id; }
    
    inline void bind() const;
    inline void unbind() const;
    inline void dispose();
    inline void ensureInitialized();

    void setBuffer(const std::vector<unsigned int>& indices);
};

inline void ElementBufferObject::bind() const {
    if (_id == 0) {
        Logger::error("Binding an invalid ElementBufferObject");
        return;
    }
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

inline void ElementBufferObject::unbind() const {
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

inline void ElementBufferObject::dispose() {
    if (_id == 0) return;
    OPENGL_EXTRA_FUNCTIONS->glDeleteBuffers(1, &_id);
    _id = 0;
}

inline void ElementBufferObject::ensureInitialized() {
    if (_id == 0) {
        OPENGL_EXTRA_FUNCTIONS->glGenBuffers(1, &_id);
        if (_id == 0) {
            Logger::error("Failed to create ElementBufferObject");
        }
    }
}