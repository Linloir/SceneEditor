#include "terrain.h"
#include "utils.h"

#include <STBImage/stb_image.h>
#include <vector>

Terrain::Terrain(std::string path){
    Vertex.clear();
    Indicess.clear(); 
    int imgW, imgH, imgChannel;

    unsigned char* data = stbi_load((path + "/heightmap.jpg").c_str(), &imgW, &imgH, &imgChannel, 1);


    int index1 = 0, index = 0;
    float xrate = imgW / 2.0f;
    float rrate = imgH / 2.0f;

    if (data) {
        for (int r = 0; r < imgH; r++)
        {
            for (int c = 0; c < imgW; c++)
            {
                //生成顶点数组, 坐标按照三角网格处理 GL_TRIGANLES
                index1 = r * imgW + c;
                Vertex.push_back((c - xrate) / xrate);
                Vertex.push_back(data[index1] / 255.0f);
                Vertex.push_back((r - rrate) / rrate);
                //顶点颜色
                Vertex.push_back(1.0f);
                Vertex.push_back(0.0f);
                Vertex.push_back(0.0f);
                //纹理坐标
                Vertex.push_back((c - xrate) / imgW);
                Vertex.push_back((r - rrate) / imgH);
            }
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
    OPENGL_EXTRA_FUNCTIONS->glBufferData(GL_ARRAY_BUFFER, Vertex.size() * sizeof(float), &Vertex[0], GL_STATIC_DRAW);

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