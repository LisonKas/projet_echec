#include "Camera.hpp"
#include <iostream>

std::unordered_map<int, bool> Camera::keyStates;

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float speed)
    : position(position), front(front), up(up), speed(speed), target(glm::vec3(0.0f)), radius(10.0f), theta(0.0f), phi(0.0f) {}

Camera::Camera(glm::vec3 target, float radius, float speed)
    : target(target), radius(radius), theta(0.0f), phi(glm::radians(30.0f)), speed(speed)
{
    updatePosition();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position, target, up);
}

void Camera::key_callback(int key, int scancode, int action, int mods)
{
    if (action == 1)
    {
        keyStates[scancode] = true;
    }
    else if (action == 0)
    {
        keyStates[scancode] = false;
    }
    std::cout << "scancode " << scancode << std::endl;
}

void Camera::processInput()
{
    if (keyStates[336])
    {
        phi += 0.01f;
        if (phi > glm::radians(89.0f))
            phi = glm::radians(89.0f);
        updatePosition();
    }
    if (keyStates[328])
    {
        phi -= 0.01f;
        if (phi < glm::radians(1.0f))
            phi = glm::radians(1.0f);
        updatePosition();
    }
    if (keyStates[331])
    {
        theta += 0.01f;
        updatePosition();
    }
    if (keyStates[333])
    {
        theta -= 0.01f;
        updatePosition();
    }
}

void Camera::updatePosition()
{
    position.x = target.x + radius * sin(phi) * cos(theta);
    position.y = target.y + radius * cos(phi);
    position.z = target.z + radius * sin(phi) * sin(theta);

    front = glm::normalize(target - position);
}
