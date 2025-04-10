#include "Skybox.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include "../laws/maths.hpp"
#include "glm/glm.hpp"

GLfloat skyboxVertices[] = {
    -10.0f, 10.0f, -10.0f,
    -10.0f, -10.0f, -10.0f,
    10.0f, -10.0f, -10.0f,
    10.0f, -10.0f, -10.0f,
    10.0f, 10.0f, -10.0f,
    -10.0f, 10.0f, -10.0f,

    -10.0f, -10.0f, 10.0f,
    -10.0f, -10.0f, -10.0f,
    -10.0f, 10.0f, -10.0f,
    -10.0f, 10.0f, -10.0f,
    -10.0f, 10.0f, 10.0f,
    -10.0f, -10.0f, 10.0f,

    10.0f, -10.0f, -10.0f,
    10.0f, -10.0f, 10.0f,
    10.0f, 10.0f, 10.0f,
    10.0f, 10.0f, 10.0f,
    10.0f, 10.0f, -10.0f,
    10.0f, -10.0f, -10.0f,

    -10.0f, -10.0f, 10.0f,
    -10.0f, 10.0f, 10.0f,
    10.0f, 10.0f, 10.0f,
    10.0f, 10.0f, 10.0f,
    10.0f, -10.0f, 10.0f,
    -10.0f, -10.0f, 10.0f,

    -10.0f, 10.0f, -10.0f,
    10.0f, 10.0f, -10.0f,
    10.0f, 10.0f, 10.0f,
    10.0f, 10.0f, 10.0f,
    -10.0f, 10.0f, 10.0f,
    -10.0f, 10.0f, -10.0f,

    -10.0f, -10.0f, -10.0f,
    -10.0f, -10.0f, 10.0f,
    10.0f, -10.0f, -10.0f,
    10.0f, -10.0f, -10.0f,
    -10.0f, -10.0f, 10.0f,
    10.0f, -10.0f, 10.0f
};

// Les différents dossiers de skybox
std::vector<std::string> skybox_paths = {
    "../../images/3D/Skybox/skybox_stormy_weather/",
    "../../images/3D/Skybox/skybox_galaxy/",
    "../../images/3D/Skybox/skybox_dawn/",
    "../../images/3D/Skybox/skybox_aurora/"
};

// Sélection de la skybox au lancement
int         selected_index = select_index_uniform(skybox_paths.size());
std::string default_path   = skybox_paths[selected_index];

// Noms des images de Skybox
std::vector<std::string> faces = {
    "nz.bmp",
    "pz.bmp",
    "py.bmp",
    "ny.bmp",
    "px.bmp",
    "nx.bmp"
};

unsigned char* loadTexture(const std::string& filename, int* width, int* height, int* nrChannels) {
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
        std::swap(data[i], data[i + 2]);
    }

    for (int i = 0; i < *height / 2; ++i)
    {
        unsigned char* topRow    = data + i * (*width) * (*nrChannels);
        unsigned char* bottomRow = data + (*height - 1 - i) * (*width) * (*nrChannels);

        std::vector<unsigned char> tempRow(*width * *nrChannels); 
        ////////////////// WINDOWS ////////////////// WINDOWS //////////////////////// WINDOWS /////////////////////////
        std::memcpy(tempRow.data(), topRow, *width * *nrChannels);
        std::memcpy(topRow, bottomRow, *width * *nrChannels);
        std::memcpy(bottomRow, tempRow.data(), *width * *nrChannels);
        /////////////////// LINUX ////////////////// LINUX //////////////////////// LINUX /////////////////////////
        /*
            memcpy(tempRow.data(), topRow, *width * *nrChannels);
            memcpy(topRow, bottomRow, *width * *nrChannels);
            memcpy(bottomRow, tempRow.data(), *width * *nrChannels);
        */
        ////////// LINUX ////////////////// LINUX //////////////////////// LINUX /////////////////////////
    }

    return data;
}

void Skybox::InitializeSkybox() {  // Initialisation des VAO, VBO et du shader de la skybox
    glGenVertexArrays(1, &this->m_VAO);
    glGenBuffers(1, &this->m_VBO);
    glBindVertexArray(this->m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->m_Texture = loadCubemap();

    this->m_Shader = new Shader("../../src/3D/shaders/shader.vs.glsl", "../../src/3D/shaders/shader.fs.glsl");
}

void Skybox::DrawSkybox(const float* view, const float* projection) { // Apparition de la skybox
    glDisable(GL_DEPTH_TEST);
    m_Shader->use();

    glUniformMatrix4fv(glGetUniformLocation(m_Shader->m_ID, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(m_Shader->m_ID, "projection"), 1, GL_FALSE, projection);

    glBindVertexArray(this->m_VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_Texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glEnable(GL_DEPTH_TEST);
}

GLuint Skybox::loadCubemap() { // Chargement des éléments de la skybox
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int            width, height, nrChannels;
    unsigned char* data;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        data = loadTexture((default_path + faces[i]).c_str(), &width, &height, &nrChannels);
        if (data)
        {
            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB; 
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

            delete[] data;
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void Skybox::Destroy() {
    delete m_Shader;
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteTextures(1, &m_Texture);
}