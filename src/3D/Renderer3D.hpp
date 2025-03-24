#pragma once
#include "Skybox.hpp"
#include "Chessboard3D.hpp"

class Renderer3D {
    private:
        Skybox m_skybox;
        Chessboard3D m_chessboard;

    public:
        void initialize();
        void render();
        void close();
};