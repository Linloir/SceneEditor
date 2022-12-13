#pragma once

#include <GLM/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

#include "shader.h"
#include "logger.h"

VertexShader::VertexShader(const std::string& source){
    _shaderId = OPENGL_EXTRA_FUNCTIONS->glCreateShader(GL_VERTEX_SHADER);
    compile(source);
}

void VertexShader::compile(const std::string& source) {
    std::string codeStr;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(source);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        codeStr = stream.str();
    }
    catch (std::ifstream::failure e) {
        Logger::error("Failed to read vertex shader file");
    }
    const char* code = codeStr.c_str();
    OPENGL_EXTRA_FUNCTIONS->glShaderSource(_shaderId, 1, &code, nullptr);
    OPENGL_EXTRA_FUNCTIONS->glCompileShader(_shaderId);
    int success;
    char infoLog[512];
    OPENGL_EXTRA_FUNCTIONS->glGetShaderiv(_shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        OPENGL_EXTRA_FUNCTIONS->glGetShaderInfoLog(_shaderId, 512, nullptr, infoLog);
        Logger::error("Vertex shader compilation failed: " + std::string(infoLog));
    }
}

FragmentShader::FragmentShader(const std::string& source){
    _shaderId = OPENGL_EXTRA_FUNCTIONS->glCreateShader(GL_FRAGMENT_SHADER);
    compile(source);
}

void FragmentShader::compile(const std::string& source) {
    std::string codeStr;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(source);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        codeStr = stream.str();
    }
    catch (std::ifstream::failure e) {
        Logger::error("Failed to read vertex shader file");
    }
    const char* code = codeStr.c_str();
    OPENGL_EXTRA_FUNCTIONS->glShaderSource(_shaderId, 1, &code, nullptr);
    OPENGL_EXTRA_FUNCTIONS->glCompileShader(_shaderId);
    int success;
    char infoLog[512];
    OPENGL_EXTRA_FUNCTIONS->glGetShaderiv(_shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        OPENGL_EXTRA_FUNCTIONS->glGetShaderInfoLog(_shaderId, 512, nullptr, infoLog);
        Logger::error("Vertex shader compilation failed: " + std::string(infoLog));
    }
}

GeometryShader::GeometryShader(const std::string& source) {
    _shaderId = OPENGL_EXTRA_FUNCTIONS->glCreateShader(GL_GEOMETRY_SHADER);
    compile(source);
}

void GeometryShader::compile(const std::string& source) {
    std::string codeStr;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(source);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        codeStr = stream.str();
    }
    catch (std::ifstream::failure e) {
        Logger::error("Failed to read vertex shader file");
    }
    const char* code = codeStr.c_str();
    OPENGL_EXTRA_FUNCTIONS->glShaderSource(_shaderId, 1, &code, nullptr);
    OPENGL_EXTRA_FUNCTIONS->glCompileShader(_shaderId);
    int success;
    char infoLog[512];
    OPENGL_EXTRA_FUNCTIONS->glGetShaderiv(_shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        OPENGL_EXTRA_FUNCTIONS->glGetShaderInfoLog(_shaderId, 512, nullptr, infoLog);
        Logger::error("Vertex shader compilation failed: " + std::string(infoLog));
    }
}

ShaderProgram::ShaderProgram() {
    _programId = 0;
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