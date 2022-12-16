#include"lightCaster.h"

ShaderProgram* lastShader = NULL;

queue<int> available = queue<int>();

Illuminant::Illuminant(){
    Logger::debug("We do not recommend to construct illuminant without parameter");
}
// white color by default
Illuminant::Illuminant(glm::vec3 position, LightType lightType){
    _position = position;
    setType(lightType);
    Logger::info("55555555555");
}
// set up with one color
Illuminant::Illuminant(glm::vec3 position, glm::vec3 color, LightType lightType){
    _position = position;
    _ambient = color * 0.05f;
    _diffuse = color * 0.8f;
    _specular = color;
    setType(lightType);
    Logger::info("4444444444444444444");
}
//set up by assign all colors
Illuminant::Illuminant(glm::vec3 position, glm::vec3 ambient, 
                        glm::vec3 diffuse, glm::vec3 specular, LightType lightType){
    _position = position;
    _ambient = ambient;
    _diffuse = diffuse;
    _specular = specular;  
    setType(lightType);
    Logger::info("333333333333333");
}

// set up with one color，建议平行光源使用这个
Illuminant::Illuminant(LightType lightType, glm::vec3 direction, glm::vec3 color) {
    _direction = direction;
    _ambient = color * 0.05f;
    _diffuse = color * 0.8f;
    _specular = color;
    setType(lightType);
    Logger::info("22222222222222222");
}

// set up with one color，建议聚光灯光源使用这个
Illuminant::Illuminant(glm::vec3 position, glm::vec3 direction, glm::vec3 color, LightType lightType) {
    _direction = direction;
    _position = position;
    _ambient = color * 0.05f;
    _diffuse = color * 0.8f;
    _specular = color;
    setType(lightType);
    Logger::info("1111111111111111111");
}

Illuminant::~Illuminant() {
    switchOff();
    if (_lightType == point) {
        available.push(this->pointID);
    }

}

void Illuminant::setAttenuation(float constant, float linear, float quadratic){
    _constant = constant;
    _linear = linear;
    _quadratic = quadratic;
}

glm::vec3 Illuminant::direction(){
    return _direction;
}
glm::vec3 Illuminant::position(){
    return _position;
}
glm::vec3 Illuminant::ambient(){
    return _ambient;
}
glm::vec3 Illuminant::diffuse(){
    return _diffuse;
}
glm::vec3 Illuminant::specular(){
    return _specular;
}

Illuminant::LightType Illuminant::type(){
    return _lightType;
}

void Illuminant::move(glm::vec3 deltaPos){
    _position += deltaPos;
}
void Illuminant::setPosition(glm::vec3 Pos){
    _position = Pos;
}

void Illuminant::setColor(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular){
    _ambient = ambient;
    _diffuse = diffuse;
    _specular = specular;
}

// 使用默认的分配系数
void Illuminant::setColor(glm::vec3 color){
    _ambient = color * 0.05f;
    _diffuse = color * 0.8f;
    _specular = color;
}
void Illuminant::setDirection(glm::vec3 direction) {
    _direction = direction;
}

void Illuminant::setType(LightType type){

    if (available.size() == 0 and type == point) {
        Logger::error("Point light number exceed!\n");
        return;
    }
    _lightType = type;
    if (type == point) {
        pointID = available.front();
        available.pop();
    }
}

void Illuminant::updateLight(ShaderProgram& shader) {
    lastShader = &shader;
    if (_lightType == dir) {
        // directional light
        shader.setUniform("dirLight.direction", _direction);
        shader.setUniform("dirLight.ambient", _ambient);
        shader.setUniform("dirLight.diffuse", _diffuse);
        shader.setUniform("dirLight.specular",_specular);
    }
    else if (_lightType == point)
    {    // point light 1
        string prefix = "pointLights[" + std::to_string(pointID) + "].";

        shader.setUniform(prefix+"position", _position);
        shader.setUniform(prefix + "ambient", _ambient);
        shader.setUniform(prefix + "diffuse", _diffuse);
        shader.setUniform(prefix + "specular", _specular);
        shader.setUniform(prefix + "constant",_constant);
        shader.setUniform(prefix + "linear", _linear);
        shader.setUniform(prefix + "quadratic", _quadratic);
         
    }
    else {
        // spotLight
        shader.setUniform("spotLight.position", _position);
        shader.setUniform("spotLight.direction", _direction);
        shader.setUniform("spotLight.ambient", _ambient);
        shader.setUniform("spotLight.diffuse", _diffuse);
        shader.setUniform("spotLight.specular", _specular);
        shader.setUniform("spotLight.constant", _constant);
        shader.setUniform("spotLight.linear", _linear);
        shader.setUniform("spotLight.quadratic", _quadratic);
        shader.setUniform("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        shader.setUniform("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
    }


}

void Illuminant::switchOff() {
    // 只要没有颜色就行了
    setColor(glm::vec3(0.0f, 0.0f, 0.0f));
    if (lastShader != NULL) {
        updateLight(*lastShader);
    }
}

void init_queue() {
    for (int i = 0; i < Illuminant::MAXPOINTLIGHT; i++) {
        available.push(i);
        Logger::info("23533423q543");
    }
}

// 如果uniform变量没有被设置，可能会出现不可预知的后果！
void setAllLigntUniform(ShaderProgram& shader) {

    shader.setUniform("dirLight.direction",0,0,-1);
    shader.setUniform("dirLight.ambient", 0.1f, 0.1f, 0.1f);
    shader.setUniform("dirLight.diffuse", 0.0f, 0.0f, 0.0f);
    shader.setUniform("dirLight.specular", 0.0f, 0.0f, 0.0f);
    // point light 1
    
    shader.setUniform("pointLights[0].position", 0, 0, 0);
    shader.setUniform("pointLights[0].ambient", 0.0f, 0.0f, 0.0f);
    shader.setUniform("pointLights[0].diffuse", 0.0f, 0.0f, 0.0f);
    shader.setUniform("pointLights[0].specular", 0.0f, 0.0f, 0.0f);
    shader.setUniform("pointLights[0].constant", 1.0f);
    shader.setUniform("pointLights[0].linear", 0.09f);
    shader.setUniform("pointLights[0].quadratic", 0.032f);
    // point light 2
    shader.setUniform("pointLights[1].position", 0.0f, 0.0f, 0.0f);
    shader.setUniform("pointLights[1].ambient", 0.0f, 0.0f, 0.0f);
    shader.setUniform("pointLights[1].diffuse", 0.0f, 0.0f, 0.0f);
    shader.setUniform("pointLights[1].specular", 0.0f, 0.0f, 0.0f);
    shader.setUniform("pointLights[1].constant", 1.0f);
    shader.setUniform("pointLights[1].linear", 0.09f);
    shader.setUniform("pointLights[1].quadratic", 0.032f);
    // point light 3
    shader.setUniform("pointLights[2].position", 0.0f, 0.0f, 0.0f);
    shader.setUniform("pointLights[2].ambient", 0.0f, 0.0f, 0.0f);
    shader.setUniform("pointLights[2].diffuse", 0.0f, 0.0f, 0.0f);
    shader.setUniform("pointLights[2].specular", 0.0f, 0.0f, 0.0f);
    shader.setUniform("pointLights[2].constant", 1.0f);
    shader.setUniform("pointLights[2].linear", 0.09f);
    shader.setUniform("pointLights[2].quadratic", 0.032f);
    // point light 4
    shader.setUniform("pointLights[3].position", 0.0f, 0.0f, 0.0f);
    shader.setUniform("pointLights[3].ambient", 0.0f, 0.0f, 0.0f);
    shader.setUniform("pointLights[3].diffuse", 0.0f, 0.0f, 0.0f);
    shader.setUniform("pointLights[3].specular", 0.0f, 0.0f, 0.0f);
    shader.setUniform("pointLights[3].constant", 1.0f);
    shader.setUniform("pointLights[3].linear", 0.09f);
    shader.setUniform("pointLights[3].quadratic", 0.032f);
    // spotLight
    shader.setUniform("spotLight.position", 0.0f, 0.0f, 3.0f);
    shader.setUniform("spotLight.direction", -0.0f,-0.0f, -1.0f);
    shader.setUniform("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    shader.setUniform("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
    shader.setUniform("spotLight.specular", 0.0f, 0.0f, 0.0f);
    shader.setUniform("spotLight.constant", 1.0f);
    shader.setUniform("spotLight.linear", 0.09f);
    shader.setUniform("spotLight.quadratic", 0.032f);
    shader.setUniform("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    shader.setUniform("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
}