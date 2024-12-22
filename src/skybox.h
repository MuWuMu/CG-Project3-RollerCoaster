#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Skybox {
public:
    Skybox(const std::vector<std::string>& faces);
    void render(const glm::mat4& view, const glm::mat4& projection);

private:
    unsigned int loadCubemap(const std::vector<std::string>& faces);
    unsigned int VAO, VBO, cubemapTexture;
};

#endif