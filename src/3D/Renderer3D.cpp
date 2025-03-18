#include "Renderer3D.hpp"
#include <iostream>

void Renderer3D::initialize() {
    m_skybox.InitializeSkybox();
}

void Renderer3D::render() {
    //glClearColor(1,0,0,1);
    //glClear(GL_COLOR_BUFFER_BIT);
    float view[16] = {
        1, 0, 0, 0,  
        0, 1, 0, 0,  
        0, 0, 1, 0,  
        0, 0, 0, 1  
    };

    float projection[16] = {
        1, 0, 0, 0,  
        0, 1, 0, 0,  
        0, 0, -1, -1,  
        0, 0, -0.1, 0  
    };
    m_skybox.DrawSkybox(view, projection);
}

void Renderer3D::close() {
    // glDeleteVertexArrays(1, &m_skybox.getVAO());
    // glDeleteBuffers(1, &m_skybox.getVBO());
    // glDeleteTextures(1, &m_skybox.getTexture());
}