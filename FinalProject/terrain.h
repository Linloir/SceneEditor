#pragma once

#include "utils.h"

#include <vector>
#include <string>

class Terrain {
private:
    std::vector<float> Vertex;
    std::vector<unsigned int> Indicess;
    unsigned int TerrainVAO, TerrainVBO;
    int row_num, col_num;
public:
    unsigned int dep, tex1, tex2;
    Terrain(int rows = 200, int cols = 200);
    void render();
    unsigned int loadTexture(std::string path);
};