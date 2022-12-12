#pragma once

#include <string>
#include <vector>
#include <unordered_set>

class Shader {
private:
    int _shaderId = -1;
    
public:
    Shader();

protected:
    virtual void compile(std::string sourceFilePath) = 0;
    void dispose();

public:
    bool operator == (const Shader& other) const;
};

class VertexShader : public Shader {
public:
    VertexShader();
    VertexShader(std::string sourceFilePath);
    
protected:
    virtual void compile(std::string sourceFilePath) override;
};

class FragmentShader : public Shader {
public:
    FragmentShader();
    FragmentShader(std::string sourceFilePath);

protected:
    virtual void compile(std::string sourceFilePath) override;
};

class GeometryShader : public Shader {
public:
    GeometryShader();
    GeometryShader(std::string sourceFilePath);

protected:
    virtual void compile(std::string sourceFilePath) override;
};

class ShaderProgram {
private:
    VertexShader _vertexShader;
    FragmentShader _fragmentShader;
    GeometryShader _geometryShader;
    unsigned int _programId = -1;

public:
    ShaderProgram();
    ShaderProgram(VertexShader vertexShader);
    ShaderProgram(FragmentShader fragmentShader);
    ShaderProgram(GeometryShader geometryShader);
    ShaderProgram(VertexShader vertexShader, FragmentShader fragmentShader);
    ShaderProgram(VertexShader vertexShader, GeometryShader geometryShader);
    ShaderProgram(FragmentShader fragmentShader, GeometryShader geometryShader);
    ShaderProgram(VertexShader vertexShader, FragmentShader fragmentShader, GeometryShader geometryShader);
    ~ShaderProgram();
    
public:
    bool operator == (const Shader& other);
    
    template<typename UniformType>
    void setUniform(std::string name, const std::vector<UniformType>& values);

    inline void setActive();
};
