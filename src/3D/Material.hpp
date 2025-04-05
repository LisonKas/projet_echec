#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Material {
public:
    std::string name;
    glm::vec3 Ka; // Ambient color
    glm::vec3 Kd; // Diffuse color
    glm::vec3 Ks; // Specular color
    float Ns;     // Specular exponent
    GLuint textureID;

    Material() : textureID(0) {}

    void loadTexture(const std::string& path);
};