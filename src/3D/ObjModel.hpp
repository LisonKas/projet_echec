#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "Shader.hpp"

class ObjModel {
public:
    ObjModel(const std::string& path);
    ~ObjModel();

    void draw(Shader& shader, const glm::mat4& modelMatrix);

private:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    std::vector<Vertex> vertices;
    GLuint VAO = 0, VBO = 0;

    void loadObj(const std::string& path);
    void setupMesh();
};