#include "Chessboard3D.hpp"
#include <fstream>
#include <vector>
#include <cstring>

GLfloat chessboardVertices[] = {
    // Chaque case est un cube avec des sommets définis ici (simplifié)
    // Format : x, y, z
    // Exemple pour une seule case en 3D (cube) - à répéter pour 64 cases
    -0.5f, 0.0f, -0.5f,  // Bas gauche arrière
    0.5f, 0.0f, -0.5f,   // Bas droite arrière
    0.5f, 0.0f, 0.5f,    // Bas droite avant
    0.5f, 0.0f, 0.5f,    // Bas droite avant
    -0.5f, 0.0f, 0.5f,   // Bas gauche avant
    -0.5f, 0.0f, -0.5f,  // Bas gauche arrière

    -0.5f, 0.1f, -0.5f,  // Haut gauche arrière
    0.5f, 0.1f, -0.5f,   // Haut droite arrière
    0.5f, 0.1f, 0.5f,    // Haut droite avant
    0.5f, 0.1f, 0.5f,    // Haut droite avant
    -0.5f, 0.1f, 0.5f,   // Haut gauche avant
    -0.5f, 0.1f, -0.5f,  // Haut gauche arrière
};

std::string whiteTexturePath = "../../images/3D/Chessboard/white_square.bmp";
std::string blackTexturePath = "../../images/3D/Chessboard/black_square.bmp";
std::string borderTexturePath = "../../images/3D/Chessboard/borders.bmp";

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
        std::swap(data[i], data[i + 2]); // Inverse le rouge et le bleu
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

    GLuint textureID;
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
    glGenVertexArrays(1, &this->m_VAO);
    glGenBuffers(1, &this->m_VBO);
    glBindVertexArray(this->m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(chessboardVertices), &chessboardVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    this->m_TextureWhite = loadTexture(whiteTexturePath);
    this->m_TextureBlack = loadTexture(blackTexturePath);
    this->m_BorderTexture = loadTexture(borderTexturePath);

    this->m_Shader = new Shader("../../src/3D/shaders/chessboard.vs.glsl", "../../src/3D/shaders/chessboard.fs.glsl");
}

void Chessboard3D::DrawChessboard(const float* view, const float* projection) {
    glEnable(GL_DEPTH_TEST);
    m_Shader->use();

    glUniformMatrix4fv(glGetUniformLocation(m_Shader->ID, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(m_Shader->ID, "projection"), 1, GL_FALSE, projection);

    glBindVertexArray(this->m_VAO);
    
    for (int i = 0; i < 64; i++) {
        glActiveTexture(GL_TEXTURE0);
        if ((i / 8 + i) % 2 == 0) {
            glBindTexture(GL_TEXTURE_2D, this->m_TextureWhite);
        } else {
            glBindTexture(GL_TEXTURE_2D, this->m_TextureBlack);
        }
        glDrawArrays(GL_TRIANGLES, i * 36, 36);
    }

    glBindVertexArray(this->m_BorderVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->m_BorderTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Chessboard3D::Destroy() {
    delete m_Shader;
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteTextures(1, &m_TextureWhite);
    glDeleteTextures(1, &m_TextureBlack);
    glDeleteTextures(1, &m_BorderTexture);
}