#pragma once

#include "vao.h"

VertexArrayObject::VertexArrayObject() {
    _id = 0;
}

VertexArrayObject::VertexArrayObject(const VertexBufferObject& vbo) {
    OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &_id);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(_id);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, vbo.id());
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
}

VertexArrayObject::VertexArrayObject(const VertexBufferObject& vbo, const ElementBufferObject& ebo) {
    OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &_id);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(_id);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, vbo.id());
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.id());
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
}