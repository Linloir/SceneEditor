#pragma once

#include "utils.h"
#include "vertex.h"

#include <vector>
#include <string>

class Terrain {
private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int terrainVAO, terrainVBO, terrainIBO;
    std::vector<std::vector<float>> Point;
    int if_under_terrain(glm::vec3 point);
    int width, height, nrChannels;
    int numStrips, numTrisPerStrip;
    int NUM_PATCH_PTS = 4;
    unsigned rez = 20;
public:
    unsigned int tex;
    Terrain(std::string path);
    void render();
    unsigned int loadTexture(std::string path);
    Vertex hitPoint(glm::vec3 orig, glm::vec3 dir);
    glm::mat4 Model = glm::mat4(1.0f);
};