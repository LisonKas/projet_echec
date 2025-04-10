#include "Camera.hpp"
#include <iostream>

std::unordered_map<int, bool> Camera::m_keyStates; // Pour enregistrer les actions des touches

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float speed)
    : m_position(position), m_front(front), m_up(up), m_speed(speed), m_target(glm::vec3(0.0f)), m_radius(10.0f), m_theta(0.0f), m_phi(0.0f) {}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_position, m_target, m_up);
}

void Camera::key_callback(int key, int scancode, int action, int mods)
{
    if (action == 1)
    {
        m_keyStates[scancode] = true;
    }
    else if (action == 0)
    {
        m_keyStates[scancode] = false;
    }
}

void Camera::processInput()  // Calculs et réalisation des mouvements de la Caméra
{
    ////////////////// WINDOWS ////////////////// WINDOWS //////////////////////// WINDOWS /////////////////////////
    if (m_keyStates[336])
    {
        m_phi += 0.01f;
        if (m_phi > glm::radians(89.0f))
            m_phi = glm::radians(89.0f);
        updatePosition();
    }
    if (m_keyStates[328])
    {
        m_phi -= 0.01f;
        if (m_phi < glm::radians(1.0f))
            m_phi = glm::radians(1.0f);
        updatePosition();
    }
    if (m_keyStates[331])
    {
        m_theta += 0.01f;
        updatePosition();
    }
    if (m_keyStates[333])
    {
        m_theta -= 0.01f;
        updatePosition();
    }
    ///////////////////   LINUX ////////////////// LINUX //////////////////////// LINUX ///////////////////////// 
    /*
    if (m_keyStates[116])
    {
        m_phi += 0.01f;
        if (m_phi > glm::radians(89.0f))
            m_phi = glm::radians(89.0f);
        updatePosition();
    }
    if (m_keyStates[111])
    {
        m_phi -= 0.01f;
        if (m_phi < glm::radians(1.0f))
            m_phi = glm::radians(1.0f);
        updatePosition();
    }
    if (m_keyStates[113])
    {
        m_theta += 0.01f;
        updatePosition();
    }
    if (m_keyStates[114])
    {
        m_theta -= 0.01f;
        updatePosition();
    }
        */
    ///////////////////   LINUX ////////////////// LINUX //////////////////////// LINUX /////////////////////////
}

void Camera::updatePosition() // Adaptation de la position en fonction des mouvements de la caméra
{
    m_position.x = m_target.x + m_radius * sin(m_phi) * cos(m_theta);
    m_position.y = m_target.y + m_radius * cos(m_phi);
    m_position.z = m_target.z + m_radius * sin(m_phi) * sin(m_theta);

    m_front = glm::normalize(m_target - m_position);
}

glm::vec3 Camera::getPosition() const {
    return m_position; 
}
