#pragma once

enum TextureType { DIFFUSE, SPECULAR };

class Texture {
private:
    unsigned int _id;
    TextureType _type;

public:
    unsigned int id() const;
    TextureType type() const;

    inline void bind() const;
    inline void unbind() const;
};
