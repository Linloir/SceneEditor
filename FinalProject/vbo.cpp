#pragma once

#include "vbo.h"

VertexBufferObject::VertexBufferObject() {
    OPENGL_EXTRA_FUNCTIONS->glGenBuffers(1, &_id);
}

VertexBufferObject::VertexBufferObject(const std::vector<Vertex>& vertices) :
    _vertices(vertices) {
    OPENGL_EXTRA_FUNCTIONS->glGenBuffers(1, &_id);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, _id);
    OPENGL_EXTRA_FUNCTIONS->glBufferData(
        GL_ARRAY_BUFFER,
        _vertices.size() * sizeof(Vertex),
        _vertices.data(),
        GL_STATIC_DRAW
    );
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBufferObject::VertexBufferObject(std::vector<Vertex>&& vertices) :
    _vertices(std::move(vertices)) {
    OPENGL_EXTRA_FUNCTIONS->glGenBuffers(1, &_id);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, _id);
    OPENGL_EXTRA_FUNCTIONS->glBufferData(
        GL_ARRAY_BUFFER,
        _vertices.size() * sizeof(Vertex),
        _vertices.data(),
        GL_STATIC_DRAW
    );
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, 0);
}
