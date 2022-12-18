#pragma once

#include "utils.h"
#include "vertex.h"

#include <vector>
#include <string>

class Terrain {
private:
    std::vector<float> _Vertex;
    std::vector<unsigned int> Indicess;
    unsigned int VAO, VBO, EBO;
    std::vector<std::vector<float>> Point;
    int if_under_terrain(glm::vec3 point);
    int imgW, imgH, imgChannel;
public:
    unsigned int tex;
    Terrain(std::string path);
    void render();
    unsigned int loadTexture(std::string path);
    Vertex hitPoint(glm::vec3 orig, glm::vec3 dir);
    glm::mat4 Model = glm::mat4(1.0f);
};