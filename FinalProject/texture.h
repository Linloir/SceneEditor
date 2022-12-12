#pragma once

#include <string>
#include <qopenglcontext.h>

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

    inline void bind() const;
    inline void unbind() const;
};
