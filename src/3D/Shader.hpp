#pragma once

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
    public:
        GLuint m_ID;

        Shader(const char* vertexPath, const char* fragmentPath);
        ~Shader();

        void use();

        void setMat4(const std::string& name, const float* value);
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setFloat(const std::string& name, float value) const;
        void setInt(const std::string& name, int value) const;
        void setBool(const std::string& name, bool value) const; 

    private:
        void checkCompileErrors(GLuint shader, std::string type);
};
