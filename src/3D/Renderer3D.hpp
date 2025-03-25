#pragma once
#include "Skybox.hpp"
#include "Chessboard3D.hpp"

class Renderer3D {
    private:
        Skybox m_skybox;
        Chessboard3D m_chessboard;

    public:
        static void key_callback(int key, int scancode, int action, int mods);
        void initialize();
        void render();
        void close();
};