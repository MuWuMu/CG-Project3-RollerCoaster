#include "Light.h"

Light::Light(LightType type, glm::vec3 position, glm::vec3 direction, glm::vec3 color)
    : type(type), position(position), direction(direction), color(color) {}

void Light::apply(Shader& shader) const {
    shader.use();
    switch (type) {
    case DIRECTIONAL:
        shader.setVec3("dirLight.direction", direction);
        shader.setVec3("dirLight.ambient", color * 0.1f);
        shader.setVec3("dirLight.diffuse", color * 0.8f);
        shader.setVec3("dirLight.specular", color);
        break;
    case POINT:
        shader.setVec3("pointLight.position", position);
        shader.setVec3("pointLight.ambient", color * 0.1f);
        shader.setVec3("pointLight.diffuse", color * 0.8f);
        shader.setVec3("pointLight.specular", color);
        shader.setFloat("pointLight.constant", 1.0f);
        shader.setFloat("pointLight.linear", 0.09f);
        shader.setFloat("pointLight.quadratic", 0.032f);
        break;
    case SPOT:
        shader.setVec3("spotLight.position", position);
        shader.setVec3("spotLight.direction", direction);
        shader.setVec3("spotLight.ambient", color * 0.1f);
        shader.setVec3("spotLight.diffuse", color * 0.8f);
        shader.setVec3("spotLight.specular", color);
        shader.setFloat("spotLight.constant", 1.0f);
        shader.setFloat("spotLight.linear", 0.09f);
        shader.setFloat("spotLight.quadratic", 0.032f);
        shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
        break;
    }
}