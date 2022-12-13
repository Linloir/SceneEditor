#pragma once

#include <string>
#include <vector>
#include <GLM/glm.hpp>

#include "utils.h"
#include "logger.h"

class Shader {
protected:
    unsigned int _shaderId = 0;
    
public:
    Shader() {}

    inline unsigned int shaderId() const { return _shaderId; }
    inline void dispose();

protected:
    virtual void compile(const std::string& sourceFilePath) = 0;
};

inline void Shader::dispose() {
    OPENGL_EXTRA_FUNCTIONS->glDeleteShader(_shaderId);
    _shaderId = 0;
}

class VertexShader : public Shader {
public:
    VertexShader(const std::string& sourceFilePath);
    
protected:
    virtual void compile(const std::string& sourceFilePath) override;
};

class FragmentShader : public Shader {
public:
    FragmentShader(const std::string& sourceFilePath);

protected:
    virtual void compile(const std::string& sourceFilePath) override;
};

class GeometryShader : public Shader {
public:
    GeometryShader(const std::string& sourceFilePath);

protected:
    virtual void compile(const std::string& sourceFilePath) override;
};

class ShaderProgram {
public:
    static ShaderProgram empty() {
        return ShaderProgram();
    }
    
private:
    unsigned int _programId = 0;

private:
    ShaderProgram();

public:
    ShaderProgram(VertexShader vertexShader);
    ShaderProgram(FragmentShader fragmentShader);
    ShaderProgram(GeometryShader geometryShader);
    ShaderProgram(VertexShader vertexShader, FragmentShader fragmentShader);
    ShaderProgram(VertexShader vertexShader, GeometryShader geometryShader);
    ShaderProgram(FragmentShader fragmentShader, GeometryShader geometryShader);
    ShaderProgram(VertexShader vertexShader, FragmentShader fragmentShader, GeometryShader geometryShader);
    
public:
    inline unsigned int programId() const { return _programId; }

    inline void attachShader(const Shader& shader) const;
    
    inline void bind() const;
    inline void unbind() const;
    inline void dispose();
    inline void ensureInitialized();
};

inline void ShaderProgram::attachShader(const Shader& shader) const {
    if (_programId == 0) {
        Logger::error("Attaching a shader to an invalid ShaderProgram");
        return;
    }
    OPENGL_EXTRA_FUNCTIONS->glAttachShader(_programId, shader.shaderId());
    OPENGL_EXTRA_FUNCTIONS->glLinkProgram(_programId);
}

inline void ShaderProgram::bind() const {
    if (_programId == 0) {
        Logger::error("Binding an invalid ShaderProgram");
        return;
    }
    OPENGL_EXTRA_FUNCTIONS->glUseProgram(_programId);
}

inline void ShaderProgram::unbind() const {
    OPENGL_EXTRA_FUNCTIONS->glUseProgram(0);
}

inline void ShaderProgram::dispose() {
    if (_programId == 0) return;
    OPENGL_EXTRA_FUNCTIONS->glDeleteProgram(_programId);
    _programId = 0;
}

inline void ShaderProgram::ensureInitialized() {
    if (_programId == 0) {
        _programId = OPENGL_EXTRA_FUNCTIONS->glCreateProgram();
        if (_programId == 0) {
            Logger::error("Failed to create a ShaderProgram");
        }
    }
}
