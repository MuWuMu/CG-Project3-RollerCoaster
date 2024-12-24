#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>
#include <iostream>

class Texture {
public:
    unsigned int ID;
    std::string type;
    std::string path;

    Texture(const char* texturePath, const std::string& textureType);

    void bind() const;
};

#endif