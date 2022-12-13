#pragma once

#include "ebo.h"
#include "logger.h"

ElementBufferObject::ElementBufferObject() {
    _id = 0;
}

ElementBufferObject::ElementBufferObject(const std::vector<unsigned int>& indices) {
    OPENGL_EXTRA_FUNCTIONS->glGenBuffers(1, &_id);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    OPENGL_EXTRA_FUNCTIONS->glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(),
        GL_STATIC_DRAW
    );
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ElementBufferObject::setBuffer(const std::vector<unsigned int>& indices) {
    if (_id == 0) {
        Logger::error("Try to bind buffer to an uninitialized element buffer object");
        return;
    }
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    OPENGL_EXTRA_FUNCTIONS->glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(),
        GL_STATIC_DRAW
    );
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}