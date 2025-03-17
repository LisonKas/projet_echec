#include "Skybox.hpp"
#include <iostream>
#include <fstream>
#include <vector>

GLfloat skyboxVertices[] = {
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

std::string default_path = "../../images/3D/Skybox/skybox_stormy_weather/";

std::vector<std::string> faces = {
    "pz.bmp",
    "nz.bmp",
    "py.bmp",
    "ny.bmp",
    "px.bmp",
    "nx.bmp"
};

unsigned char* loadTexture(const std::string& filename, int* width, int* height, int* nrChannels) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Erreur : Impossible d'ouvrir " << filename << std::endl;
        return nullptr;
    }

    unsigned char header[54];
    file.read(reinterpret_cast<char*>(header), 54);
    
    if (header[0] != 'B' || header[1] != 'M') {
        std::cerr << "Erreur : Le fichier n'est pas un BMP valide." << std::endl;
        return nullptr;
    }
    
    int dataOffset = *reinterpret_cast<int*>(&header[10]);
    *width = *reinterpret_cast<int*>(&header[18]);
    *height = *reinterpret_cast<int*>(&header[22]);
    int bitCount = *reinterpret_cast<short*>(&header[28]);
    
    if (bitCount != 24 && bitCount != 32) {
        std::cerr << "Erreur : Seuls les fichiers BMP 24 ou 32 bits sont supportés." << std::endl;
        return nullptr;
    }
    
    *nrChannels = bitCount / 8;
    int imageSize = (*width) * (*height) * (*nrChannels);
    
    unsigned char* data = new unsigned char[imageSize];
    
    file.seekg(dataOffset, std::ios::beg);
    file.read(reinterpret_cast<char*>(data), imageSize);
    file.close();

    for (int i = 0; i < *height / 2; ++i) {
        unsigned char* topRow = data + i * (*width) * (*nrChannels);
        unsigned char* bottomRow = data + (*height - 1 - i) * (*width) * (*nrChannels);
        
        std::vector<unsigned char> tempRow(*width * *nrChannels);
        std::memcpy(tempRow.data(), topRow, *width * *nrChannels);
        std::memcpy(topRow, bottomRow, *width * *nrChannels);
        std::memcpy(bottomRow, tempRow.data(), *width * *nrChannels);
    }
    
    return data;
}

void Skybox::InitializeSkybox() {
    glGenVertexArrays(1, &this->m_VAO);
    glGenBuffers(1, &this->m_VBO);
    glBindVertexArray(this->m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    this->m_Texture = loadCubemap();
}

void Skybox::DrawSkybox() {
    glDepthFunc(GL_LEQUAL); 
    glBindVertexArray(this->m_VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_Texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);  
}

GLuint Skybox::loadCubemap() {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    unsigned char* data;
    for (unsigned int i = 0; i < faces.size(); i++) {
        data = loadTexture(default_path+(faces[i].c_str()), &width, &height, &nrChannels);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            delete[] data;
            data = nullptr;
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            delete[] data;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
