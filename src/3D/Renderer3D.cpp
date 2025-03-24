#include "Renderer3D.hpp"
#include <iostream>

void Renderer3D::initialize() {
    m_skybox.InitializeSkybox();
    m_chessboard.InitializeChessboard();
}

void Renderer3D::render() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_LESS);
    float view[16] = {
        1, 0, 0, 0,  
        0, 1, 0, 0,  
        0, 0, 1, 0,  
        0, 0, 0, 1  
    };

    float projection[16] = {
        1, 0, 0, 0,  
        0, 1, 0, 0,  
        0, 0, -1, 0,  
        0, 0, -0.1, 1  
    };
    m_skybox.DrawSkybox(view, projection);
    m_chessboard.DrawChessboard(view, projection);
}

void Renderer3D::close() {
    m_skybox.Destroy();
}