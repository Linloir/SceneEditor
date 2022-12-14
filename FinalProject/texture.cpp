#pragma once
#define STB_IMAGE_IMPLEMENTATION

#include <STBImage/stb_image.h>
#include <qopengl.h>
#include <qopenglcontext.h>

#include "texture.h"
#include "logger.h"

QOpenGLContext* sharedContext;

// Must set context before calling this function
Texture::Texture(TextureType type, std::string path) {
    _type = type;
    _path = path.substr(path.find_last_of('/') + 1);

    OPENGL_FUNCTIONS->glGenTextures(1, &_id);
    OPENGL_FUNCTIONS->glBindTexture(GL_TEXTURE_2D, _id);
    
    // Set the texture wrapping parameters
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set stbi to reverse the y-axis
    stbi_set_flip_vertically_on_load(true);

    // Load image, create texture and generate mipmaps
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
            Logger::error("Unexpected channel count");
            return;
        }

        OPENGL_FUNCTIONS->glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        OPENGL_FUNCTIONS->glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        Logger::error("Failed to load texture: " + path);
    }
    stbi_image_free(data);
}