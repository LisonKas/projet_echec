#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Material {
    public:
        std::string name;
        glm::vec3 Ka {glm::vec3(1.0f)}; 
        glm::vec3 Kd {glm::vec3(1.0f)}; 
        glm::vec3 Ks {glm::vec3(1.0f)}; 
        float Ns {250.0f};     
        GLuint textureID {0};

        void loadTexture(const std::string& path);
};