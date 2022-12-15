#include "terrain.h"
#include "utils.h"

#include <STBImage/stb_image.h>
#include <vector>

Terrain::Terrain(int rows, int cols):row_num(rows),col_num(cols) {
    Vertex.clear();
    Indicess.clear();
    float x = -50, z = -50;

    for (int i = 0; i < rows; i++) {
        x = -5;
        for (int j = 0; j < cols; j++) {
            Vertex.push_back(x);
            Vertex.push_back(0);
            Vertex.push_back(z);
            Vertex.push_back(1.0f / cols * j);
            Vertex.push_back(1 - i * 1.0f / rows);
            x += 0.1;
        }
        z += 0.1;
    }

    for (int i = 1; i < rows; i++) {
        for (int j = 1; j < cols; j++) {
            Indicess.push_back((i - 1) * cols + j - 1);
            Indicess.push_back((i - 1) * cols + j);
            Indicess.push_back(i * cols + j - 1);

            Indicess.push_back(i * cols + j - 1);
            Indicess.push_back((i - 1) * cols + j);
            Indicess.push_back(i * cols + j);
        }
    }

    tex1 = loadTexture("D:/ProgrammingFile/SceneEditor/terrain/rock.jpg");
    tex2 = loadTexture("D:/ProgrammingFile/SceneEditor/terrain/water.jpg");
    dep = loadTexture("D:/ProgrammingFile/SceneEditor/terrain/heightmap.png");

    OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &TerrainVAO);
    OPENGL_EXTRA_FUNCTIONS->glGenBuffers(1, &TerrainVBO);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(TerrainVAO);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, TerrainVBO);
    OPENGL_EXTRA_FUNCTIONS->glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex), &Vertex, GL_STATIC_DRAW);
    OPENGL_EXTRA_FUNCTIONS->glEnableVertexAttribArray(0);
    OPENGL_EXTRA_FUNCTIONS->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
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

    OPENGL_EXTRA_FUNCTIONS->glActiveTexture(GL_TEXTURE0);
    OPENGL_EXTRA_FUNCTIONS->glBindTexture(GL_TEXTURE_2D, dep);
    OPENGL_EXTRA_FUNCTIONS->glActiveTexture(GL_TEXTURE1);
    OPENGL_EXTRA_FUNCTIONS->glBindTexture(GL_TEXTURE_2D, tex1);
    OPENGL_EXTRA_FUNCTIONS->glActiveTexture(GL_TEXTURE2);
    OPENGL_EXTRA_FUNCTIONS->glBindTexture(GL_TEXTURE_2D, tex2);

    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(TerrainVAO);
    OPENGL_EXTRA_FUNCTIONS->glDrawElements(GL_TRIANGLES, Indicess.size(), GL_UNSIGNED_SHORT, &Indicess.front());

}