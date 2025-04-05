#include "Renderer3D.hpp"
#include <iostream>
#include <unordered_map>
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "quick_imgui/quick_imgui.hpp"

void Renderer3D::initialize()
{
    m_skybox.InitializeSkybox();
    m_chessboardShader = new Shader("shaders/chessboard.vs.glsl", "shaders/chessboard.fs.glsl");
    m_chessboard = new ObjModel("../../models/chessboard.obj");
}

void Renderer3D::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_LESS);

    m_camera.processInput();

    glm::mat4 view = m_camera.getViewMatrix();
    float aspectRatio = 16.0f / 9.0f;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);

    m_skybox.DrawSkybox(glm::value_ptr(view), glm::value_ptr(projection));

    m_chessboardShader->use();
    m_chessboardShader->setMat4("uView", glm::value_ptr(view));
    m_chessboardShader->setMat4("uProjection", glm::value_ptr(projection));
    m_chessboard->draw(*m_chessboardShader, model);
}

void Renderer3D::close()
{
    m_skybox.Destroy();

    delete m_chessboard;
    delete m_chessboardShader;
    m_chessboard = nullptr;
    m_chessboardShader = nullptr;
}
