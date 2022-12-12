#pragma once

#include <GLM/gtc/type_ptr.hpp>

#include "shader.h"
#include "logger.h"

VertexShader::VertexShader(const std::string& source){
    _shaderId = OPENGL_EXTRA_FUNCTIONS->glCreateShader(GL_VERTEX_SHADER);
    compile(source);
}

void VertexShader::compile(const std::string& source) {
    const char* sourcePtr = source.c_str();
    OPENGL_EXTRA_FUNCTIONS->glShaderSource(_shaderId, 1, &sourcePtr, nullptr);
    OPENGL_EXTRA_FUNCTIONS->glCompileShader(_shaderId);
}

FragmentShader::FragmentShader(const std::string& source){
    _shaderId = OPENGL_EXTRA_FUNCTIONS->glCreateShader(GL_FRAGMENT_SHADER);
    compile(source);
}

void FragmentShader::compile(const std::string& source) {
    const char* sourcePtr = source.c_str();
    OPENGL_EXTRA_FUNCTIONS->glShaderSource(_shaderId, 1, &sourcePtr, nullptr);
    OPENGL_EXTRA_FUNCTIONS->glCompileShader(_shaderId);
}

GeometryShader::GeometryShader(const std::string& source) {
    _shaderId = OPENGL_EXTRA_FUNCTIONS->glCreateShader(GL_GEOMETRY_SHADER);
    compile(source);
}

void GeometryShader::compile(const std::string& source) {
    const char* sourcePtr = source.c_str();
    OPENGL_EXTRA_FUNCTIONS->glShaderSource(_shaderId, 1, &sourcePtr, nullptr);
    OPENGL_EXTRA_FUNCTIONS->glCompileShader(_shaderId);
}

ShaderProgram::ShaderProgram() {
    _programId = OPENGL_EXTRA_FUNCTIONS->glCreateProgram();
}

ShaderProgram::ShaderProgram(VertexShader vertexShader) {
    _programId = OPENGL_EXTRA_FUNCTIONS->glCreateProgram();
    OPENGL_EXTRA_FUNCTIONS->glAttachShader(_programId, vertexShader.shaderId());
    OPENGL_EXTRA_FUNCTIONS->glLinkProgram(_programId);
    vertexShader.dispose();
}

ShaderProgram::ShaderProgram(FragmentShader fragmentShader) {
    _programId = OPENGL_EXTRA_FUNCTIONS->glCreateProgram();
    OPENGL_EXTRA_FUNCTIONS->glAttachShader(_programId, fragmentShader.shaderId());
    OPENGL_EXTRA_FUNCTIONS->glLinkProgram(_programId);
    fragmentShader.dispose();
}

ShaderProgram::ShaderProgram(GeometryShader geometryShader) {
    _programId = OPENGL_EXTRA_FUNCTIONS->glCreateProgram();
    OPENGL_EXTRA_FUNCTIONS->glAttachShader(_programId, geometryShader.shaderId());
    OPENGL_EXTRA_FUNCTIONS->glLinkProgram(_programId);
    geometryShader.dispose();
}

ShaderProgram::ShaderProgram(VertexShader vertexShader, FragmentShader fragmentShader) {
    _programId = OPENGL_EXTRA_FUNCTIONS->glCreateProgram();
    OPENGL_EXTRA_FUNCTIONS->glAttachShader(_programId, vertexShader.shaderId());
    OPENGL_EXTRA_FUNCTIONS->glAttachShader(_programId, fragmentShader.shaderId());
    OPENGL_EXTRA_FUNCTIONS->glLinkProgram(_programId);
    vertexShader.dispose();
    fragmentShader.dispose();
}

ShaderProgram::ShaderProgram(VertexShader vertexShader, GeometryShader geometryShader) {
    _programId = OPENGL_EXTRA_FUNCTIONS->glCreateProgram();
    OPENGL_EXTRA_FUNCTIONS->glAttachShader(_programId, vertexShader.shaderId());
    OPENGL_EXTRA_FUNCTIONS->glAttachShader(_programId, geometryShader.shaderId());
    OPENGL_EXTRA_FUNCTIONS->glLinkProgram(_programId);
    vertexShader.dispose();
    geometryShader.dispose();
}

ShaderProgram::ShaderProgram(FragmentShader fragmentShader, GeometryShader geometryShader) {
    _programId = OPENGL_EXTRA_FUNCTIONS->glCreateProgram();
    OPENGL_EXTRA_FUNCTIONS->glAttachShader(_programId, fragmentShader.shaderId());
    OPENGL_EXTRA_FUNCTIONS->glAttachShader(_programId, geometryShader.shaderId());
    OPENGL_EXTRA_FUNCTIONS->glLinkProgram(_programId);
    fragmentShader.dispose();
    geometryShader.dispose();
}

ShaderProgram::ShaderProgram(VertexShader vertexShader, FragmentShader fragmentShader, GeometryShader geometryShader) {
    _programId = OPENGL_EXTRA_FUNCTIONS->glCreateProgram();
    OPENGL_EXTRA_FUNCTIONS->glAttachShader(_programId, vertexShader.shaderId());
    OPENGL_EXTRA_FUNCTIONS->glAttachShader(_programId, fragmentShader.shaderId());
    OPENGL_EXTRA_FUNCTIONS->glAttachShader(_programId, geometryShader.shaderId());
    OPENGL_EXTRA_FUNCTIONS->glLinkProgram(_programId);
    vertexShader.dispose();
    fragmentShader.dispose();
    geometryShader.dispose();
}