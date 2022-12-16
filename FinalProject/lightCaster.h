#pragma once
#include"shader.h"
#include<GLM/glm.hpp>
#include<string>
#include"logger.h"
#include<queue>

using std::queue;
using std::string;
// 发光物不需要纹理，所以说需要另外的shader
// 在sceneviewer中被包含
class Illuminant {
public:
    enum LightType { dir, spot, point };
    int pointID = -1;
private:
    LightType _lightType = dir;

    glm::vec3 _direction = glm::vec3(0,2,-1);
    glm::vec3 _position = glm::vec3(0.0f);
    glm::vec3 _ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    glm::vec3 _diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    glm::vec3 _specular = glm::vec3(0.8f, 0.8f, 0.8f);
    
    float _constant = 1.0f;
    float _linear = 0.09f;
    float _quadratic = 0.032f;


public:
    // 修改此处必须同步修改fragment shader的数组定义语句，目前此值最大为4
    const static int MAXPOINTLIGHT = 4;

    Illuminant();
    // white color by default，
    Illuminant(glm::vec3 position, LightType lightType);
    // set up with one color，建议点光源使用这个
    Illuminant(glm::vec3 position,glm::vec3 color, LightType lightType);

    // set up with one color，建议平行光源使用这个
    Illuminant(LightType lightType,glm::vec3 direction, glm::vec3 color);

    // set up with one color，建议聚光灯光源使用这个
    Illuminant(glm::vec3 position, glm::vec3 direction,glm::vec3 color, LightType lightType);

    //set up by assign all colors
    Illuminant(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, LightType lightType);

    // 析构函数中，如果是点光源，应该增加可用点光源数目。不过暂时没有实现，在上层进行控制
    // 无论如何，将此函数置为黑色
    ~Illuminant();
    void setAttenuation(float constant, float linear, float quadratic);

    glm::vec3 direction();
    glm::vec3 position();
    glm::vec3 ambient();
    glm::vec3 diffuse();
    glm::vec3 specular();
    LightType type();

    void move(glm::vec3 deltaPos);
    void setPosition(glm::vec3 Pos);
    // 平行光与手电筒必须设置此项
    void setDirection(glm::vec3 direction);

    void setColor(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    // 使用默认的分配系数
    void setColor(glm::vec3 color);
    // 将本光源的信息传递给着色器
    void updateLight(ShaderProgram& shader);



private:
    // 只能在最初设定光源类型，没有必要在期间修改
    void setType(LightType type);

    // 懒惰删除
    void switchOff();

};

// 初始设置所有变量
void setAllLigntUniform(ShaderProgram& lightingShader);

void init_queue();