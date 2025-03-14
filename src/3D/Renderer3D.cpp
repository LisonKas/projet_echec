#include "Renderer3D.hpp"
#include <iostream>

void Renderer3D::initialize() {
    m_skybox.InitializeSkybox();
}

void Renderer3D::render() {
    // m_skybox.DrawSkybox();
}

void Renderer3D::close() {
    // glDeleteVertexArrays(1, &m_skybox.getVAO());
    // glDeleteBuffers(1, &m_skybox.getVBO());
    // glDeleteTextures(1, &m_skybox.getTexture());
}