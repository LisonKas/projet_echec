#ifndef CAMERA_HPP
#define CAMERA_HPP

#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <unordered_map>

class Camera {
    public:
        // Ancien constructeur : pour les caméras basées sur position/front/up
        Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float speed);

        // Nouveau constructeur pour le mode Trackball
        Camera(glm::vec3 target, float radius, float speed);

        static void key_callback(int key, int scancode, int action, int mods);
        void processInput();
        
        glm::mat4 getViewMatrix() const;

        void setSpeed(float speed);
        float getSpeed() const;

        glm::vec3 getPosition() const;
        glm::vec3 getFront() const; // Calculé à la volée dans le mode Trackball

    private:
        glm::vec3 position;  // Position de la caméra
        glm::vec3 front;     // Vecteur direction de la caméra (calculé dans le mode Trackball)
        glm::vec3 up;        // Direction "up" de la caméra
        glm::vec3 target;    // Centre de l'échiquier (utilisé pour Trackball)
        float radius;        // Distance à la cible (utilisé pour Trackball)
        float theta;         // Angle horizontal (azimut) - utilisé pour Trackball
        float phi;           // Angle vertical (élévation) - utilisé pour Trackball
        float speed;         // Vitesse de déplacement

        static std::unordered_map<int, bool> keyStates;

        void updatePosition();  // Met à jour `position` en fonction de `theta` et `phi` pour Trackball
};

#endif // CAMERA_HPP
