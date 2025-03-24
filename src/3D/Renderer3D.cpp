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

    float chess_view[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, -5.0f, // Déplace la caméra de 5 unités sur l'axe Z
        0.0f, 0.0f, 0.0f, 1.0f
    };
    
    float chess_projection[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, -0.1f, 1.0f
    };
    m_chessboard.DrawChessboard(chess_view, chess_projection);
}

void Renderer3D::close() {
    m_skybox.Destroy();
    m_chessboard.Destroy();
}