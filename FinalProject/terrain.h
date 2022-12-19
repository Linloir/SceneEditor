#pragma once

#include <vector>
#include <string>

#include "utils.h"
#include "vertex.h"
#include "ray.h"
#include "hitrecord.h"

class Terrain {
private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int terrainVAO, terrainVBO, terrainIBO;
    std::vector<std::vector<float>> Point;
    float GetHeight(float px, float pz);
    glm::vec3 GetNormal(glm::vec3 pos);
    int width, height, nrChannels;
    int numStrips, numTrisPerStrip;
    int NUM_PATCH_PTS = 4;
    unsigned rez = 20;
    float yScale = 64.0f / 256.0f, yShift = 30.0f;
public:
    unsigned int tex;
    Terrain(std::string path);
    void render();
    unsigned int loadTexture(std::string path);
    HitRecord hit(const Ray& ray);
    glm::mat4 modelMatrix() const { return glm::mat4(1.0f); }
};