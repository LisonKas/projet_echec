#include "Camera.hpp"
#include <iostream>

std::unordered_map<int, bool> Camera::keyStates;

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float speed)
    : position(position), front(front), up(up), speed(speed) {}

// Gère les entrées clavier et met à jour keyStates
void Camera::key_callback(int key, int scancode, int action, int mods) {
    if (action == 1) { // Touche pressée
        keyStates[scancode] = true;
    } else if (action == 0) { // Touche relâchée
        keyStates[scancode] = false;
    }
    std::cout << "Key: " << key << " Scancode: " << scancode << " Action: " << action << " Mods: " << mods << '\n';
}

// Traite les entrées utilisateur pour déplacer la caméra
void Camera::processInput() {
    if (keyStates[328]) { // Flèche HAUT
        position += speed * front;
    }
    if (keyStates[336]) { // Flèche BAS
        position -= speed * front;
    }
    if (keyStates[331]) { // Flèche GAUCHE
        position -= glm::normalize(glm::cross(front, up)) * speed;
    }
    if (keyStates[333]) { // Flèche DROITE
        position += glm::normalize(glm::cross(front, up)) * speed;
    }
}

// Retourne la matrice de vue
glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::setSpeed(float newSpeed) {
    speed = newSpeed;
}

float Camera::getSpeed() const {
    return speed;
}

glm::vec3 Camera::getPosition() const {
    return position;
}

glm::vec3 Camera::getFront() const {
    return front;
}