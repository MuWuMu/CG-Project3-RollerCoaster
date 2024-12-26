#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"

class Skybox {
public:
    Skybox(const std::vector<std::string>& faces);
    void render(const glm::mat4& view, const glm::mat4& projection);
    unsigned int getCubemapTexture() const { return cubemapTexture; }

private:
    void loadCubemap(const std::vector<std::string>& faces);
    void initRenderData();
    
    unsigned int VAO, VBO, cubemapTexture;
    Shader skyboxShader;
};

#endif