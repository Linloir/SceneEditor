#include "terrain.h"
#include "utils.h"
#include "vertex.h"
#include <STBImage/stb_image.h>
#include <vector>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

Terrain::Terrain(std::string path){
    _Vertex.clear();
    Indicess.clear(); 
    Point.clear();
    

    Model = glm::translate(Model, glm::vec3(0.0f, -10.0f, 0.0f));
    Model = glm::scale(Model, glm::vec3(20.0f));

    unsigned char* data = stbi_load((path + "/heightmap.jpg").c_str(), &imgW, &imgH, &imgChannel, 1);


    int index1 = 0, index = 0;
    float xrate = imgW / 2.0f;
    float rrate = imgH / 2.0f;

    if (data) {
        for (int r = 0; r < imgH; r++)
        {
            std::vector<float> temp;
            for (int c = 0; c < imgW; c++)
            {
                //生成顶点数组, 坐标按照三角网格处理 GL_TRIGANLES
                index1 = r * imgW + c;
                _Vertex.push_back((c - xrate) / xrate);
                _Vertex.push_back(data[index1] / 255.0f);
                _Vertex.push_back((r - rrate) / rrate);

                temp.push_back(data[index1] / 255.0f);

                //顶点颜色
                _Vertex.push_back(1.0f);
                _Vertex.push_back(0.0f);
                _Vertex.push_back(0.0f);
                //纹理坐标
                _Vertex.push_back((c - xrate) / imgW);
                _Vertex.push_back((r - rrate) / imgH);
            }
            Point.push_back(temp);
        }
        imgH -= 1;
        int iW = imgW - 1;
        for (int r = 0; r < imgH; r++)
        {
            for (int c = 0; c < iW; c++)
            {

                index1 = r * imgW + c;
                index = (r + 1) * imgW + c;
                Indicess.push_back(index1);
                Indicess.push_back(index1 + 1);
                Indicess.push_back(index + 1);

                Indicess.push_back(index1);
                Indicess.push_back(index);
                Indicess.push_back(index + 1);

            }
        }
    }
    stbi_image_free(data);
    

    OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &VAO);
    OPENGL_EXTRA_FUNCTIONS->glGenBuffers(1, &VBO);
    OPENGL_EXTRA_FUNCTIONS->glGenBuffers(1, &EBO);

    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(VAO);

    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    OPENGL_EXTRA_FUNCTIONS->glBufferData(GL_ARRAY_BUFFER, _Vertex.size() * sizeof(float), &_Vertex[0], GL_STATIC_DRAW);

    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    OPENGL_EXTRA_FUNCTIONS->glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indicess.size() * sizeof(unsigned int), &Indicess[0], GL_STATIC_DRAW);

    OPENGL_EXTRA_FUNCTIONS->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    OPENGL_EXTRA_FUNCTIONS->glEnableVertexAttribArray(0);
    // color attribute
    OPENGL_EXTRA_FUNCTIONS->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    OPENGL_EXTRA_FUNCTIONS->glEnableVertexAttribArray(1);
    // texture coord attribute
    OPENGL_EXTRA_FUNCTIONS->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    OPENGL_EXTRA_FUNCTIONS->glEnableVertexAttribArray(2);

    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, 0);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);

    //textureID = loadTexture2(texName, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    tex = loadTexture(path + "/texture.jpg");

}

unsigned int Terrain::loadTexture(std::string path) {
    unsigned int textureID;
    OPENGL_FUNCTIONS->glGenTextures(1, &textureID);
    OPENGL_FUNCTIONS->glBindTexture(GL_TEXTURE_2D, textureID);

    // Set the texture wrapping parameters
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
    OPENGL_EXTRA_FUNCTIONS->glActiveTexture(GL_TEXTURE2);
    OPENGL_EXTRA_FUNCTIONS->glBindTexture(GL_TEXTURE_2D, tex);

    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(VAO);
    OPENGL_EXTRA_FUNCTIONS->glDrawElements(GL_TRIANGLES, Indicess.size(), GL_UNSIGNED_INT, 0);

}

int Terrain::if_under_terrain(glm::vec3 point) {
    if (point.x >= imgH-1 || point.y >= imgW-1 || point.x < 0 || point.y < 0) {
        return 0;
    }
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
    if (orig.x >= imgH - 1 || orig.y >= imgW - 1 || orig.x <0 || orig.y < 0) {
        return Vertex(glm::vec3(0.0f));
    }

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
            return Vertex(right);
        }
    }

}