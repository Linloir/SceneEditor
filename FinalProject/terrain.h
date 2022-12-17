#pragma once

#include "utils.h"

#include <vector>
#include <string>

class Terrain {
private:
    std::vector<float> Vertex;
    std::vector<unsigned int> Indicess;
    unsigned int VAO, VBO, EBO;
public:
    unsigned int tex;
    Terrain(std::string path);
    void render();
    unsigned int loadTexture(std::string path);
};