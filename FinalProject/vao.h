#pragma once

#include <vector>

#include "vbo.h"
#include "ebo.h"
#include "utils.h"
#include "logger.h"

class VertexArrayObject {
public:
    static VertexArrayObject empty() {
        return VertexArrayObject();
    }

private:
    unsigned int _id = 0;

private:
    VertexArrayObject();

public:
    VertexArrayObject(const VertexBufferObject& vbo);
    VertexArrayObject(const VertexBufferObject& vbo, const ElementBufferObject& ebo);

    inline unsigned int id() const { return _id; }
    
    inline void bind() const;
    inline void unbind() const;
    inline void dispose();
    inline void ensureInitialized();

    inline void bindVertexBufferObject(const VertexBufferObject& vbo);
    inline void bindElementBufferObject(const ElementBufferObject& ebo);

    inline void setVertexAttributePointer(unsigned int index, int size, unsigned int type, bool normalized, unsigned int stride, const void* pointer) const;
    inline void enableVertexAttribute(unsigned int index) const;
    inline void disableVertexAttribute(unsigned int index) const;
};

inline void VertexArrayObject::bind() const {
    // Check is id is valid
    if (_id == 0) {
        Logger::error("Binding an invalid VertexArrayObject");
        return;
    }
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(_id);
}

inline void VertexArrayObject::unbind() const {
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
}

inline void VertexArrayObject::dispose() {
    if (_id == 0) return;
    OPENGL_EXTRA_FUNCTIONS->glDeleteVertexArrays(1, &_id);
    _id = 0;
}

inline void VertexArrayObject::ensureInitialized() {
    if (_id == 0) {
        OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &_id);
        if (_id == 0) {
            Logger::error("Failed to create VertexArrayObject");
        }
    }
}

inline void VertexArrayObject::bindVertexBufferObject(const VertexBufferObject& vbo) {
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(_id);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, vbo.id());
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
}

inline void VertexArrayObject::bindElementBufferObject(const ElementBufferObject& ebo) {
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(_id);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.id());
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
}

inline void VertexArrayObject::setVertexAttributePointer(
    unsigned int index,
    int size,
    unsigned int type,
    bool normalized,
    unsigned int stride,
    const void* pointer
) const {
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(_id);
    OPENGL_EXTRA_FUNCTIONS->glVertexAttribPointer(
        index,
        size,
        type,
        normalized,
        stride,
        pointer
    );
    OPENGL_EXTRA_FUNCTIONS->glEnableVertexAttribArray(index);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
}

inline void VertexArrayObject::enableVertexAttribute(unsigned int index) const {
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(_id);
    OPENGL_EXTRA_FUNCTIONS->glEnableVertexAttribArray(index);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
}

inline void VertexArrayObject::disableVertexAttribute(unsigned int index) const {
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(_id);
    OPENGL_EXTRA_FUNCTIONS->glDisableVertexAttribArray(index);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
}