#ifndef CAMERA_HPP
#define CAMERA_HPP

#pragma once

#include <unordered_map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class Camera {
    private:
        glm::vec3 m_position; // Position de la caméra
        glm::vec3 m_front;    // Direction de la caméra 
        glm::vec3 m_up;       // Direction vers le haut de la caméra
        glm::vec3 m_target;   // Centre de l'échiquier 
        float     m_radius;   // Distance
        float     m_theta;    // Angle horizontal 
        float     m_phi;      // Angle vertical
        float     m_speed;    // Vitesse de déplacement

        static std::unordered_map<int, bool> m_keyStates;

        void updatePosition();

    public:
        Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float speed);

        static void key_callback(int key, int scancode, int action, int mods);
        void        processInput();
        
        void  setSpeed(float speed);

        // GETTERS
        glm::mat4 getViewMatrix() const;
        float getSpeed() const;
        glm::vec3 getPosition() const;
        glm::vec3 getFront() const;
};

#endif 
