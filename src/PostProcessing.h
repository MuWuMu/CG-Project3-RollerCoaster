#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"

enum PostProcessingEffect {
    NONE,
    PIXELIZATION,
    INVERSION,
    GRAYSCALE,
    SHARPEN,
    BLUR,
    EDGE_DETECTION
};

class PostProcessing {
public:
    PostProcessing(unsigned int width, unsigned int height);
    void beginRender();
    void endRender();
    void render(PostProcessingEffect effect);

    float pixelSize;

private:
    void initRenderData();

    unsigned int width, height;
    unsigned int FBO, RBO;
    unsigned int texture;
    unsigned int VAO, VBO;
    Shader defaultShader;
    Shader pixelizationShader;
    Shader inversionShader;
    Shader grayscaleShader;
    Shader sharpenShader;
    Shader blurShader;
    Shader edgeDetectionShader;
};

#endif