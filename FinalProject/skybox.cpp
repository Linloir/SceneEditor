#include <STBImage/stb_image.h>
#include <vector>
#include <string>

#include "skybox.h"
#include "shader.h"
#include "camera.h"

SkyBox::SkyBox(std::string path){
    faces.clear();
    faces.push_back(path + "/right.jpg");
    faces.push_back(path + "/left.jpg");
    faces.push_back(path + "/bottom.jpg");
    faces.push_back(path + "/top.jpg");
    faces.push_back(path + "/front.jpg");
    faces.push_back(path + "/back.jpg");

    cubemapTexture = loadCubemap(faces);
    
    OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &skyboxVAO);
    OPENGL_EXTRA_FUNCTIONS->glGenBuffers(1, &skyboxVBO);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(skyboxVAO);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    OPENGL_EXTRA_FUNCTIONS->glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    OPENGL_EXTRA_FUNCTIONS->glEnableVertexAttribArray(0);
    OPENGL_EXTRA_FUNCTIONS->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void SkyBox::render() {
    OPENGL_EXTRA_FUNCTIONS->glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    // skybox cube
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(skyboxVAO);
    OPENGL_EXTRA_FUNCTIONS->glActiveTexture(GL_TEXTURE0);
    OPENGL_EXTRA_FUNCTIONS->glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    OPENGL_EXTRA_FUNCTIONS->glDrawArrays(GL_TRIANGLES, 0, 36);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
    OPENGL_EXTRA_FUNCTIONS->glDepthFunc(GL_LESS); // set depth function back to default
}

unsigned int SkyBox::loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    OPENGL_FUNCTIONS->glGenTextures(1, &textureID);
    OPENGL_FUNCTIONS->glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            OPENGL_FUNCTIONS->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}