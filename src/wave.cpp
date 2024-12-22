#include "Wave.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Wave::Wave(unsigned int width, unsigned int height)
    : width(width), height(height), time(0.0f) {
    initMesh();
}

void Wave::initMesh() {
    // Initialize positions, normals, and indices
    for (unsigned int y = 0; y <= height; ++y) {
        for (unsigned int x = 0; x <= width; ++x) {
            positions.push_back(glm::vec3((float)x, 0.0f, (float)y));
            normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        }
    }

    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            indices.push_back(y * (width + 1) + x);
            indices.push_back((y + 1) * (width + 1) + x);
            indices.push_back(y * (width + 1) + x + 1);

            indices.push_back(y * (width + 1) + x + 1);
            indices.push_back((y + 1) * (width + 1) + x);
            indices.push_back((y + 1) * (width + 1) + x + 1);
        }
    }

    // Create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3) + normals.size() * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(glm::vec3), &positions[0]);
    glBufferSubData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), normals.size() * sizeof(glm::vec3), &normals[0]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(positions.size() * sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Wave::update(float deltaTime) {
    time += deltaTime;
    updateMesh();
}

void Wave::updateMesh() {
    // Update positions and normals based on time
    for (unsigned int y = 0; y <= height; ++y) {
        for (unsigned int x = 0; x <= width; ++x) {
            float waveHeight = sinf((float)x * 0.1f + time) * cosf((float)y * 0.1f + time);
            positions[y * (width + 1) + x].y = waveHeight;
            normals[y * (width + 1) + x] = glm::normalize(glm::vec3(-0.1f * cosf((float)x * 0.1f + time), 1.0f, -0.1f * sinf((float)y * 0.1f + time)));
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(glm::vec3), &positions[0]);
    glBufferSubData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), normals.size() * sizeof(glm::vec3), &normals[0]);
}

void Wave::render() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}