#ifndef WAVE_H
#define WAVE_H

#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Light.h"

class Wave {
public:

    Wave(unsigned int width, unsigned int height);
    void update(float deltaTime);
    void render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos,
                const Light& directionalLight, const Light& pointLight, const Light& spotLight,
                unsigned int skyboxTexture);

    unsigned int width, height;
    
    float speed;
    float amplitude;
    float frequency;
    glm::vec3 color;
    glm::vec2 direction;

private:
    void initMesh();
    void updateMesh();

    float time;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;
    Shader waveShader;
};

#endif