#include "Shader.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {  // Charger les vertex et fragment shaders
    std::ifstream vShaderFile(vertexPath);
    std::ifstream fShaderFile(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;

    if (!vShaderFile.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier de vertex shader : " << vertexPath << std::endl;
    }

    if (!fShaderFile.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier de fragment shader : " << fragmentPath << std::endl;
    }

    if (!vShaderFile || !fShaderFile) {
        std::cerr << "Erreur : Impossible de lire les fichiers shader !" << std::endl;
        return;
    }

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    vShaderFile.close();
    fShaderFile.close();

    std::string vertexCode = vShaderStream.str();
    std::string fragmentCode = fShaderStream.str();

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "Erreur de compilation du vertex shader : " << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "Erreur de compilation du fragment shader : " << infoLog << std::endl;
    }

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertex);
    glAttachShader(m_ID, fragment);
    glLinkProgram(m_ID);
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
        std::cerr << "Erreur de linkage du shader program : " << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(m_ID);
}

void Shader::use() {
    glUseProgram(m_ID);
}

// SETTERS
void Shader::setMat4(const std::string& name, const float* value) {
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::setBool(const std::string& name, bool value) const {
    GLint location = glGetUniformLocation(m_ID, name.c_str());
    glUniform1i(location, (int)value);
}

void Shader::checkCompileErrors(GLuint shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}