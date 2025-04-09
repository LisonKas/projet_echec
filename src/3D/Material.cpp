#include "Material.hpp"
#include <fstream>
#include <vector>
#include <iostream>

void Material::loadTexture(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open texture file: " << path << std::endl;
        return;
    }

    file.seekg(0, std::ios::end);
    std::vector<unsigned char> data(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(data.data()), data.size());

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width = 512; 
    int height = 512; 
    if (data.size() >= 54) { 
        width = data[18] + (data[19] << 8) + (data[20] << 16) + (data[21] << 24);
        height = data[22] + (data[23] << 8) + (data[24] << 16) + (data[25] << 24);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data.data() + 54);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Invalid texture format" << std::endl;
    }
}