#include "Renderer3D.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "quick_imgui/quick_imgui.hpp"
#include <unordered_map>
#include <iostream>

// Stockage des touches pressées
std::unordered_map<int, bool> keyStates;

// Position et orientation de la caméra
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

// Sensibilité du mouvement
float cameraSpeed = 0.05f;

void Renderer3D::key_callback(int key, int scancode, int action, int mods) {
    if (action == 1) { // Touche pressée
        keyStates[scancode] = true;
    } else if (action == 0) { // Touche relâchée
        keyStates[scancode] = false;
    }
    std::cout << "Key: " << key << " Scancode: " << scancode << " Action: " << action << " Mods: " << mods << '\n';
}

void Renderer3D::initialize() {
    m_skybox.InitializeSkybox();
    m_chessboard.InitializeChessboard();
}

void processInput() {
    // Vérifie les scancodes pour les touches fléchées
    if (keyStates[328]) { // Flèche HAUT
        cameraPos += cameraSpeed * cameraFront;
    }
    if (keyStates[336]) { // Flèche BAS
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (keyStates[331]) { // Flèche GAUCHE
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (keyStates[333]) { // Flèche DROITE
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
}

void Renderer3D::render() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_LESS);

    processInput();

    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    float aspectRatio = 16.0f / 9.0f;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    m_skybox.DrawSkybox(glm::value_ptr(view), glm::value_ptr(projection));
    m_chessboard.DrawChessboard(view, projection);
}

void Renderer3D::close() {
    m_skybox.Destroy();
    m_chessboard.Destroy();
}
