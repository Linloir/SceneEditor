#pragma once

#include <string>
#include <vector>
#include <GLM/glm.hpp>

#include "utils.h"

class Shader {
protected:
    unsigned int _shaderId = -1;
    
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
    VertexShader() {}
    VertexShader(const std::string& sourceFilePath);
    
protected:
    virtual void compile(const std::string& sourceFilePath) override;
};

class FragmentShader : public Shader {
public:
    FragmentShader() {}
    FragmentShader(const std::string& sourceFilePath);

protected:
    virtual void compile(const std::string& sourceFilePath) override;
};

class GeometryShader : public Shader {
public:
    GeometryShader() {}
    GeometryShader(const std::string& sourceFilePath);

protected:
    virtual void compile(const std::string& sourceFilePath) override;
};

class ShaderProgram {
private:
    unsigned int _programId = 0;

public:
    ShaderProgram();
    ShaderProgram(VertexShader vertexShader);
    ShaderProgram(FragmentShader fragmentShader);
    ShaderProgram(GeometryShader geometryShader);
    ShaderProgram(VertexShader vertexShader, FragmentShader fragmentShader);
    ShaderProgram(VertexShader vertexShader, GeometryShader geometryShader);
    ShaderProgram(FragmentShader fragmentShader, GeometryShader geometryShader);
    ShaderProgram(VertexShader vertexShader, FragmentShader fragmentShader, GeometryShader geometryShader);
    
public:
    inline void setActive();
    inline void setInactive();

    inline unsigned int programId() const { return _programId; }

    inline void dispose();
};

inline void ShaderProgram::setActive() {
    OPENGL_EXTRA_FUNCTIONS->glUseProgram(_programId);
}

inline void ShaderProgram::setInactive() {
    OPENGL_EXTRA_FUNCTIONS->glUseProgram(0);
}

inline void ShaderProgram::dispose() {
    OPENGL_EXTRA_FUNCTIONS->glDeleteProgram(_programId);
    _programId = 0;
}
