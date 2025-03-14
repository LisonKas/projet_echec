#pragma once

#include <vector>
#include <iostream>
#include "glad/glad.h"

class Skybox {
    private:
        GLuint m_VAO = 0, m_VBO = 0, m_Texture = 0;

    public:
        void InitializeSkybox();
        void DrawSkybox();
        GLuint loadCubemap();
};