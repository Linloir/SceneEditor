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

    void setBool(const std::string& name, bool value) const
    {
        OPENGL_EXTRA_FUNCTIONS->glUniform1i(OPENGL_EXTRA_FUNCTIONS->glGetUniformLocation(_shaderId, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const
    {
        OPENGL_EXTRA_FUNCTIONS->glUniform1i(OPENGL_EXTRA_FUNCTIONS->glGetUniformLocation(_shaderId, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const
    {
        OPENGL_EXTRA_FUNCTIONS->glUniform1f(OPENGL_EXTRA_FUNCTIONS->glGetUniformLocation(_shaderId, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        OPENGL_EXTRA_FUNCTIONS->glUniform2fv(OPENGL_EXTRA_FUNCTIONS->glGetUniformLocation(_shaderId, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const
    {
        OPENGL_EXTRA_FUNCTIONS->glUniform2f(OPENGL_EXTRA_FUNCTIONS->glGetUniformLocation(_shaderId, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        OPENGL_EXTRA_FUNCTIONS->glUniform3fv(OPENGL_EXTRA_FUNCTIONS->glGetUniformLocation(_shaderId, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        OPENGL_EXTRA_FUNCTIONS->glUniform3f(OPENGL_EXTRA_FUNCTIONS->glGetUniformLocation(_shaderId, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        OPENGL_EXTRA_FUNCTIONS->glUniform4fv(OPENGL_EXTRA_FUNCTIONS->glGetUniformLocation(_shaderId, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w)
    {
        OPENGL_EXTRA_FUNCTIONS->glUniform4f(OPENGL_EXTRA_FUNCTIONS->glGetUniformLocation(_shaderId, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        OPENGL_EXTRA_FUNCTIONS->glUniformMatrix2fv(OPENGL_EXTRA_FUNCTIONS->glGetUniformLocation(_shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        OPENGL_EXTRA_FUNCTIONS->glUniformMatrix3fv(OPENGL_EXTRA_FUNCTIONS->glGetUniformLocation(_shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        OPENGL_EXTRA_FUNCTIONS->glUniformMatrix4fv(OPENGL_EXTRA_FUNCTIONS->glGetUniformLocation(_shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

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
