#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include "Shader.h"

enum LightType {
    DIRECTIONAL,
    POINT,
    SPOT
};

class Light {
public:
    Light(LightType type, glm::vec3 position, glm::vec3 direction, glm::vec3 color);

    void apply(Shader& shader) const;

    LightType type;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
};

#endif