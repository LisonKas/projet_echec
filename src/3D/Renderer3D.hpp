#pragma once
#include "Camera.hpp"
#include "Shader.hpp"
#include "Skybox.hpp"
#include "ObjModel.hpp"


class Renderer3D {
    private:
        Skybox m_skybox;

        ObjModel* m_chessboard = nullptr;
        Shader* m_chessboardShader = nullptr;
        Camera m_camera = Camera(glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.05f);

    public:
        void initialize();
        void render();
        void close();
};