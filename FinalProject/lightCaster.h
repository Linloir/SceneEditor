#pragma once
#include"shader.h"
#include<GLM/glm.hpp>
#include<string>
#include"logger.h"
#include<queue>

using std::queue;
using std::string;
// �����ﲻ��Ҫ��������˵��Ҫ�����shader
// ��sceneviewer�б�����
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
    // �޸Ĵ˴�����ͬ���޸�fragment shader�����鶨����䣬Ŀǰ��ֵ���Ϊ4
    const static int MAXPOINTLIGHT = 4;

    Illuminant();
    // white color by default��
    Illuminant(glm::vec3 position, LightType lightType);
    // set up with one color��������Դʹ�����
    Illuminant(glm::vec3 position,glm::vec3 color, LightType lightType);

    // set up with one color������ƽ�й�Դʹ�����
    Illuminant(LightType lightType,glm::vec3 direction, glm::vec3 color);

    // set up with one color������۹�ƹ�Դʹ�����
    Illuminant(glm::vec3 position, glm::vec3 direction,glm::vec3 color, LightType lightType);

    //set up by assign all colors
    Illuminant(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, LightType lightType);

    // ���������У�����ǵ��Դ��Ӧ�����ӿ��õ��Դ��Ŀ��������ʱû��ʵ�֣����ϲ���п���
    // ������Σ����˺�����Ϊ��ɫ
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
    // ƽ�й����ֵ�Ͳ�������ô���
    void setDirection(glm::vec3 direction);

    void setColor(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    // ʹ��Ĭ�ϵķ���ϵ��
    void setColor(glm::vec3 color);
    // ������Դ����Ϣ���ݸ���ɫ��
    void updateLight(ShaderProgram& shader);



private:
    // ֻ��������趨��Դ���ͣ�û�б�Ҫ���ڼ��޸�
    void setType(LightType type);

    // ����ɾ��
    void switchOff();

};

// ��ʼ�������б���
void setAllLigntUniform(ShaderProgram& lightingShader);

void init_queue();