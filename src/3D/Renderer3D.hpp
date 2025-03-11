#pragma once
#include "Skybox.hpp"

class Renderer3D {
    private:
        Skybox m_skybox;

    public:
        void initialize();
        void render();
        void close();
};