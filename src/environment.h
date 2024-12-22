#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Environment {
public:
    Environment();
    void render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos, unsigned int skyboxTexture, float time);

private:
    void initSphere();
    unsigned int VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

#endif