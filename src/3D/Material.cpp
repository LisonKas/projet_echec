#include "Material.hpp"
#include <fstream>
#include <vector>
#include <iostream>
#include <filesystem>

void Material::loadTexture(const std::string& texturePath) {
    std::filesystem::path path(texturePath);
    
    std::cout << "Loading texture: " << path.string() << std::endl;
    
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open texture file: " << path.string() << std::endl;
        return;
    }

    char header[54];
    file.read(header, 54);
    
    if (header[0] != 'B' || header[1] != 'M') {
        std::cerr << "Not a valid BMP file: " << path.string() << std::endl;
        return;
    }
    
    int dataPos     = *(int*)&(header[0x0A]);
    int imageSize   = *(int*)&(header[0x22]);
    int width       = *(int*)&(header[0x12]);
    int height      = *(int*)&(header[0x16]);
    
    if (imageSize == 0)    
        imageSize = width * height * 3; 
    if (dataPos == 0)      
        dataPos = 54; 
    
    std::vector<unsigned char> data(imageSize);
    
    file.seekg(dataPos, std::ios::beg);
    
    file.read(reinterpret_cast<char*>(data.data()), imageSize);
    
    file.close();
    
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    
    std::cout << "Texture loaded successfully: " << width << "x" << height << " (ID: " << textureID << ")" << std::endl;
}