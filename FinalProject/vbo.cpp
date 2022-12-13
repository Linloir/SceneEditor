#pragma once

#include "vbo.h"
#include "logger.h"

VertexBufferObject::VertexBufferObject() {
    _id = 0;
}

VertexBufferObject::VertexBufferObject(const std::vector<Vertex>& vertices) {
    OPENGL_EXTRA_FUNCTIONS->glGenBuffers(1, &_id);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, _id);
    OPENGL_EXTRA_FUNCTIONS->glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_STATIC_DRAW
    );
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferObject::setBuffer(const std::vector<Vertex>& vertices) {
    if (_id == 0) {
        Logger::error("Try to bind buffer to an uninitialized vertex buffer object");
        return;
    }
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, _id);
    OPENGL_EXTRA_FUNCTIONS->glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_STATIC_DRAW
    );
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, 0);
}