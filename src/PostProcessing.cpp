#include "PostProcessing.h"
#include <iostream>

PostProcessing::PostProcessing(unsigned int width, unsigned int height)
    : width(width), height(height), pixelSize(1.0f),
      postProcessingShader("shaders/post_processings/postProcessing.vs", "shaders/post_processings/postProcessing.fs"),
      pixelizationShader("shaders/post_processings/postProcessing.vs", "shaders/post_processings/pixelization.fs"),
      inversionShader("shaders/post_processings/postProcessing.vs", "shaders/post_processings/inversion.fs"),
      grayscaleShader("shaders/post_processings/postProcessing.vs", "shaders/post_processings/grayscale.fs"),
      sharpenShader("shaders/post_processings/postProcessing.vs", "shaders/post_processings/sharpen.fs"),
      blurShader("shaders/post_processings/postProcessing.vs", "shaders/post_processings/blur.fs"),
      edgeDetectionShader("shaders/post_processings/postProcessing.vs", "shaders/post_processings/edgeDetection.fs") {
    // Initialize FBO
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Create texture to render to
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // Create renderbuffer object for depth and stencil attachment
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Initialize render data
    initRenderData();
}

void PostProcessing::beginRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PostProcessing::endRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessing::render(PostProcessingEffect effect) {
    Shader* shader;
    switch (effect) {
        case PIXELIZATION:
            shader = &pixelizationShader;
            shader->use();
            shader->setFloat("pixelSize", pixelSize);
            shader->setVec2("screenSize", glm::vec2(width, height));
            break;
        case INVERSION:
            shader = &inversionShader;
            shader->use();
            break;
        case GRAYSCALE:
            shader = &grayscaleShader;
            shader->use();
            break;
        case SHARPEN:
            shader = &sharpenShader;
            shader->use();
            break;
        case BLUR:
            shader = &blurShader;
            shader->use();
            break;
        case EDGE_DETECTION:
            shader = &edgeDetectionShader;
            shader->use();
            break;
        case NONE:
        default:
            shader = &postProcessingShader;
            shader->use();
            break;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void PostProcessing::initRenderData() {
    // Configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}