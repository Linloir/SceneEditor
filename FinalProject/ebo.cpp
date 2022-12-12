#pragma once

#include "ebo.h"

ElementBufferObject::ElementBufferObject() {
    OPENGL_EXTRA_FUNCTIONS->glGenBuffers(1, &_id);
}

ElementBufferObject::ElementBufferObject(const std::vector<unsigned int>& indices) :
    _indices(indices) {
    OPENGL_EXTRA_FUNCTIONS->glGenBuffers(1, &_id);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    OPENGL_EXTRA_FUNCTIONS->glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        _indices.size() * sizeof(unsigned int),
        _indices.data(),
        GL_STATIC_DRAW
    );
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

ElementBufferObject::ElementBufferObject(std::vector<unsigned int>&& indices) :
    _indices(std::move(indices)) {
    OPENGL_EXTRA_FUNCTIONS->glGenBuffers(1, &_id);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    OPENGL_EXTRA_FUNCTIONS->glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        _indices.size() * sizeof(unsigned int),
        _indices.data(),
        GL_STATIC_DRAW
    );
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
