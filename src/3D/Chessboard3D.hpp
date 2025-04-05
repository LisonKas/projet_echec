#pragma once

#include <iostream>
#include <vector>
#include "Shader.hpp"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class Chessboard3D {
private:
    GLuint  m_VAO = 0, m_VBO = 0, m_EBO = 0, m_TextureWhite = 0, m_TextureBlack = 0, m_BorderTexture = 0;
    Shader* m_Shader;
    GLuint  m_BorderVAO = 0, m_BorderVBO = 0;

public:
    void   InitializeChessboard();
    void   drawCube(const glm::vec3& position, float size, GLuint texture);
    void   DrawChessboard(const glm::mat4& view, const glm::mat4& projection); // const glm::mat4& view, const glm::mat4& projection ou //const float* view, const float* projection
    GLuint loadTexture(const std::string& path);
    void   Destroy();
};