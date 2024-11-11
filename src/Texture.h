#ifndef TEXTURE_H
#define TEXUTRE_H

#include <glad/glad.h>;

#include <iostream>

#include "stb_image.h"

class Texture
{
public:
    unsigned int ID;

    Texture(const char* texturePath);
    
    void use();
    void del();
};

#endif