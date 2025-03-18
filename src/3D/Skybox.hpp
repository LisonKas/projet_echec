#pragma once

#include <vector>
#include <iostream>
#include "glad/glad.h"
#include "Shader.hpp"

class Skybox {
    private:
        GLuint m_VAO = 0, m_VBO = 0, m_Texture = 0;
        Shader* m_Shader;

    public:
        void InitializeSkybox();
        void DrawSkybox(const float* view, const float* projection);
        GLuint loadCubemap();
        void Destroy();
};