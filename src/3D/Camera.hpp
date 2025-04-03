#ifndef CAMERA_HPP
#define CAMERA_HPP

#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <unordered_map>

class Camera {
    public:
        Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float speed);

        static void key_callback(int key, int scancode, int action, int mods);
        void processInput();
        
        glm::mat4 getViewMatrix() const;

        void setSpeed(float speed);
        float getSpeed() const;

        glm::vec3 getPosition() const;
        glm::vec3 getFront() const;

    private:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        float speed;

        static std::unordered_map<int, bool> keyStates;
};

#endif // CAMERA_HPP