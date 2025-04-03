#include "Chessboard3D.hpp"
#include <fstream>
#include <vector>
#include <iostream>
#include <cstring>
#include "Skybox.hpp"

std::vector<float> cubeVertices = {
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

std::vector<unsigned int> cubeIndices = {
    0, 1, 2,  2, 3, 0,  
    1, 5, 6,  6, 2, 1,  
    7, 6, 5,  5, 4, 7,  
    4, 0, 3,  3, 7, 4,  
    4, 5, 1,  1, 0, 4,  
    3, 2, 6,  6, 7, 3   
};


unsigned char* LoadTexture(const std::string& filename, int* width, int* height, int* nrChannels)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Erreur : Impossible d'ouvrir " << filename << std::endl;
        return nullptr;
    }

    unsigned char header[54];
    file.read(reinterpret_cast<char*>(header), 54);

    if (header[0] != 'B' || header[1] != 'M')
    {
        std::cerr << "Erreur : Le fichier n'est pas un BMP valide." << std::endl;
        return nullptr;
    }

    int dataOffset = *reinterpret_cast<int*>(&header[10]);
    *width         = *reinterpret_cast<int*>(&header[18]);
    *height        = *reinterpret_cast<int*>(&header[22]);
    int bitCount   = *reinterpret_cast<short*>(&header[28]);

    if (bitCount != 24 && bitCount != 32)
    {
        std::cerr << "Erreur : Seuls les fichiers BMP 24 ou 32 bits sont supportés." << std::endl;
        return nullptr;
    }

    *nrChannels   = bitCount / 8;
    int imageSize = (*width) * (*height) * (*nrChannels);

    unsigned char* data = new unsigned char[imageSize];

    file.seekg(dataOffset, std::ios::beg);
    file.read(reinterpret_cast<char*>(data), imageSize);
    file.close();

    for (int i = 0; i < imageSize; i += *nrChannels)
    {
        std::swap(data[i], data[i + 2]); //Inverse le rouge et le bleu
    }

    for (int i = 0; i < *height / 2; ++i)
    {
        unsigned char* topRow    = data + i * (*width) * (*nrChannels);
        unsigned char* bottomRow = data + (*height - 1 - i) * (*width) * (*nrChannels);

        std::vector<unsigned char> tempRow(*width * *nrChannels);
        std::memcpy(tempRow.data(), topRow, *width * *nrChannels);
        std::memcpy(topRow, bottomRow, *width * *nrChannels);
        std::memcpy(bottomRow, tempRow.data(), *width * *nrChannels);
    }

    return data;
}

GLuint Chessboard3D::loadTexture(const std::string& path) {
    int width, height, nrChannels;
    unsigned char* data = LoadTexture(path, &width, &height, &nrChannels);
    if (!data) {
        std::cerr << "Erreur lors du chargement de la texture : " << path << std::endl;
        return 0;
    }

    GLuint textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete[] data;
    return textureID;
}

void Chessboard3D::InitializeChessboard() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(float), cubeVertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(unsigned int), cubeIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    m_TextureWhite = loadTexture("../../images/3D/Chessboard/white_square.bmp");
    m_TextureBlack = loadTexture("../../images/3D/Chessboard/black_square.bmp");
    m_BorderTexture = loadTexture("../../images/3D/Chessboard/borders.bmp");

    m_Shader = new Shader("../../src/3D/shaders/chessboard.vs.glsl", "../../src/3D/shaders/chessboard.fs.glsl");
}

void Chessboard3D::DrawChessboard(const glm::mat4& view, const glm::mat4& projection) {
    if (!m_Shader) return;
    
    glEnable(GL_DEPTH_TEST);
    m_Shader->use();
    glUniformMatrix4fv(glGetUniformLocation(m_Shader->ID, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_Shader->ID, "projection"), 1, GL_FALSE, &projection[0][0]);

    glBindVertexArray(m_VAO);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            glm::vec3 position = glm::vec3((i - 3.5f) * 1.0f, -0.5f, (j - 3.5f) * 1.0f);
            GLuint texture = ((i + j) % 2 == 0) ? m_TextureWhite : m_TextureBlack;
            drawCube(position, 1.0f, texture);
        }
    }
    glBindVertexArray(0);
}

void Chessboard3D::drawCube(const glm::vec3& position, float size, GLuint texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(size));
    glUniformMatrix4fv(glGetUniformLocation(m_Shader->ID, "model"), 1, GL_FALSE, &model[0][0]);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Chessboard3D::Destroy() {
    delete m_Shader;
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteTextures(1, &m_TextureWhite);
    glDeleteTextures(1, &m_TextureBlack);
    glDeleteTextures(1, &m_BorderTexture);
}