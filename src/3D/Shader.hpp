#pragma once

#include "glad/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    GLuint ID;

    Shader(const char* vertexPath, const char* fragmentPath);
    void use();
    void setMat4(const std::string& name, const float* value);
    ~Shader();
};
