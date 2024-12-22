#ifndef PIXELIZATION_H
#define PIXELIZATION_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"

class Pixelization {
public:
    Pixelization(unsigned int width, unsigned int height);
    void beginRender();
    void endRender();
    void render(float pixelSize);

private:
    void initRenderData();

    unsigned int width, height;
    unsigned int FBO, RBO;
    unsigned int texture;
    unsigned int VAO, VBO;
    Shader pixelizationShader;
};

#endif