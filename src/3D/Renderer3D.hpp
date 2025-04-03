#pragma once
#include "Skybox.hpp"
#include "Chessboard3D.hpp"
#include "Camera.hpp"

class Renderer3D {
    private:
        Skybox m_skybox;
        Chessboard3D m_chessboard;
        Camera m_camera = Camera(glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.05f);

    public:
        void initialize();
        void render();
        void close();
};