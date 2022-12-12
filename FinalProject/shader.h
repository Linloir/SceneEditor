#pragma once

#include <string>
#include <vector>
#include <GLM/glm.hpp>

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
