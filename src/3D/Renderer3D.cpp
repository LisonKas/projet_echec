#include "Renderer3D.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "quick_imgui/quick_imgui.hpp"
#include <unordered_map>
#include <iostream>

void Renderer3D::initialize() {
    m_skybox.InitializeSkybox();
    m_chessboard.InitializeChessboard();
}

void Renderer3D::render() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_LESS);

    m_camera.processInput();

    glm::mat4 view = m_camera.getViewMatrix();
    float aspectRatio = 16.0f / 9.0f;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    m_skybox.DrawSkybox(glm::value_ptr(view), glm::value_ptr(projection));
    m_chessboard.DrawChessboard(view, projection);
}

void Renderer3D::close() {
    m_skybox.Destroy();
    m_chessboard.Destroy();
}
