#pragma once

#include "vao.h"

VertexArrayObject::VertexArrayObject() {}

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