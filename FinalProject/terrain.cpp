#include "terrain.h"
#include "utils.h"
#include "vertex.h"
#include <STBImage/stb_image.h>
#include <vector>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

Terrain::Terrain(std::string path){
    
    stbi_set_flip_vertically_on_load(false);

    unsigned char* data = stbi_load((path + "/heightmap.png").c_str(), &width, &height, &nrChannels, 0);


    float yScale = 64.0f / 256.0f, yShift = 16.0f;
    int rez = 1;
    unsigned bytePerPixel = nrChannels;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            unsigned char* pixelOffset = data + (j + width * i) * bytePerPixel;
            unsigned char y = pixelOffset[0];

            // vertex
            vertices.push_back(-height / 2.0f + height * i / (float)height);   // vx
            vertices.push_back((int)y * yScale - yShift);   // vy
            vertices.push_back(-width / 2.0f + width * j / (float)width);   // vz
        }
    }
    stbi_image_free(data);

    for (unsigned i = 0; i < height - 1; i += rez)
    {
        for (unsigned j = 0; j < width; j += rez)
        {
            for (unsigned k = 0; k < 2; k++)
            {
                indices.push_back(j + width * (i + k * rez));
            }
        }
    }
    
    numStrips = (height - 1) / rez;
    numTrisPerStrip = (width / rez) * 2 - 2;

    OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &terrainVAO);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(terrainVAO);

    OPENGL_EXTRA_FUNCTIONS->glGenBuffers(1, &terrainVBO);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    OPENGL_EXTRA_FUNCTIONS->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // position attribute
    OPENGL_EXTRA_FUNCTIONS->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    OPENGL_EXTRA_FUNCTIONS->glEnableVertexAttribArray(0);

    OPENGL_EXTRA_FUNCTIONS->glGenBuffers(1, &terrainIBO);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
    OPENGL_EXTRA_FUNCTIONS->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);


    //textureID = loadTexture2(texName, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    tex = loadTexture(path + "/white.jpg");

}

unsigned int Terrain::loadTexture(std::string path) {
    unsigned int textureID;
    OPENGL_FUNCTIONS->glGenTextures(1, &textureID);
    OPENGL_FUNCTIONS->glBindTexture(GL_TEXTURE_2D, textureID);

    

    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;

    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1) {
            format = GL_RED;
        }
        else if (nrChannels == 3) {
            format = GL_RGB;
        }
        else if (nrChannels == 4) {
            format = GL_RGBA;
        }
        else {
            return 0;
        }

        OPENGL_FUNCTIONS->glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        OPENGL_FUNCTIONS->glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);

    return textureID;
}

void Terrain::render() {
    OPENGL_EXTRA_FUNCTIONS->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    OPENGL_EXTRA_FUNCTIONS->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the texture wrapping parameters
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    OPENGL_EXTRA_FUNCTIONS->glActiveTexture(GL_TEXTURE2);
    OPENGL_EXTRA_FUNCTIONS->glBindTexture(GL_TEXTURE_2D, tex);

    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(terrainVAO);
    for (unsigned strip = 0; strip < numStrips; strip++)
    {
        OPENGL_EXTRA_FUNCTIONS->glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
            numTrisPerStrip + 2,   // number of indices to render
            GL_UNSIGNED_INT,     // index data type
            (void*)(sizeof(unsigned) * (numTrisPerStrip + 2) * strip)); // offset to starting index
    }

}

int Terrain::if_under_terrain(glm::vec3 point) {

    int greater_x = ceil(point.x);
    int greater_y = ceil(point.y);
    int less_x = floor(point.x);
    int less_y = floor(point.y);

    float great_z = Point[greater_x][greater_y];
    float less_z = Point[less_x][less_y];

    float z = (point.x - less_x) * (great_z - less_z) + less_z;

    if (fabs(z - point.z) < 1e-2) {
        return 2;
    }
    if (z > point.z) {
        return 1;
    }
    else
    {
        return -1;
    }

}

Vertex Terrain::hitPoint(glm::vec3 orig, glm::vec3 dir) {

    glm::vec3 step = glm::normalize(dir);

    int flag = if_under_terrain(orig);

    glm::vec3 right = orig;
    while (true) {
        right += step;
        int temp = if_under_terrain(right);
        if (temp == 0) {
            return Vertex(glm::vec3(0.0f));
        }
        if (flag * temp == -1 || temp == 2) {
            glm::vec4 ans = Model * glm::vec4(right,1.0f);
            return Vertex(glm::vec3(ans));
        }
    }

}