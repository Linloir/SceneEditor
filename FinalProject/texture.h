#pragma once

#include <string>
#include <qopenglcontext.h>

#include "utils.h"

enum TextureType { DIFFUSE, SPECULAR };

class Texture {
private:
    unsigned int _id;
    TextureType _type;
    std::string _path;

public:
    Texture(TextureType type, std::string path);

public:
    inline unsigned int id() const { return _id; }
    inline TextureType type() const { return _type; }
    inline std::string path() const { return _path; }

    void bind() const;
    void unbind() const;
};

inline void Texture::bind() const {
    OPENGL_FUNCTIONS->glBindTexture(GL_TEXTURE_2D, _id);
}

inline void Texture::unbind() const {
    OPENGL_FUNCTIONS->glBindTexture(GL_TEXTURE_2D, 0);
}
