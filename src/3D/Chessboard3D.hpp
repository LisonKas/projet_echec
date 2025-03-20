#pragma once

#include <vector>
#include <iostream>
#include "glad/glad.h"
#include "Shader.hpp"

class Chessboard3D {
    private:
        GLuint m_VAO = 0, m_VBO = 0, m_EBO = 0, m_TextureWhite = 0, m_TextureBlack = 0, m_BorderTexture = 0;
        Shader* m_Shader;
        GLuint m_BorderVAO = 0, m_BorderVBO = 0;

    public:
        void InitializeChessboard();
        void DrawChessboard(const float* view, const float* projection);
        GLuint loadTexture(const std::string& path);
        void Destroy();
};