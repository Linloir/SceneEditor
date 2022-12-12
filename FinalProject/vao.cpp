#pragma once

#include <qopengl.h>

#include "vao.h"
#include "utils.h"
#include "logger.h"

VertexArrayObject::VertexArrayObject() {
    OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &_id);
}

VertexArrayObject::VertexArrayObject(const VertexBufferObject& vbo) :
    _vbo(vbo) {
    OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &_id);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(_id);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, _vbo.id());
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
}

VertexArrayObject::VertexArrayObject(VertexBufferObject&& vbo) :
    _vbo(std::move(vbo)) {
    OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &_id);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(_id);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, _vbo.id());
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
}

VertexArrayObject::VertexArrayObject(const VertexBufferObject& vbo, const ElementBufferObject& ebo) :
    _vbo(vbo), _ebo(ebo) {
    OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &_id);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(_id);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, _vbo.id());
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo.id());
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
}

VertexArrayObject::VertexArrayObject(VertexBufferObject&& vbo, ElementBufferObject&& ebo) :
    _vbo(std::move(vbo)), _ebo(std::move(ebo)) {
    OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &_id);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(_id);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, _vbo.id());
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo.id());
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
}

inline void VertexArrayObject::setActive() const {
    // Check is id is valid
    if (_id == -1) {
        Logger::error("Binding an invalid VertexArrayObject");
        return;
    }
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(_id);
}

inline void VertexArrayObject::setInactive() const {
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
}

inline void VertexArrayObject::bindVertexBufferObject(const VertexBufferObject& vbo) {
    _vbo = vbo;
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(_id);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, _vbo.id());
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
}

inline void VertexArrayObject::bindVertexBufferObject(VertexBufferObject&& vbo) {
    _vbo = std::move(vbo);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(_id);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, _vbo.id());
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
}

inline void VertexArrayObject::bindElementBufferObject(const ElementBufferObject& ebo) {
    _ebo = ebo;
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(_id);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo.id());
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
}

inline void VertexArrayObject::bindElementBufferObject(ElementBufferObject&& ebo) {
    _ebo = std::move(ebo);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(_id);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo.id());
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