#ifndef WAVE_H
#define WAVE_H

#include <vector>
#include <glm/glm.hpp>

class Wave {
public:

    Wave(unsigned int width, unsigned int height);
    void update(float deltaTime);
    void render();

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
};

#endif