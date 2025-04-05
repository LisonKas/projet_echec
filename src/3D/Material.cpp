#include "Material.hpp"
#include <fstream>
#include <vector>
#include <iostream>

void Material::loadTexture(const std::string& path) {
    // Open the image file
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open texture file: " << path << std::endl;
        return;
    }

    // Read the image data (assuming a simple BMP format for example purposes)
    // Note: This is a very basic loader and may not work for all image formats
    file.seekg(0, std::ios::end);
    std::vector<unsigned char> data(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(data.data()), data.size());

    // Generate and bind the texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load the texture data (this example assumes a BMP with 24-bit color)
    // You will need to adjust this for other formats
    int width = 512; // Example width
    int height = 512; // Example height
    if (data.size() >= 54) { // BMP header size
        width = data[18] + (data[19] << 8) + (data[20] << 16) + (data[21] << 24);
        height = data[22] + (data[23] << 8) + (data[24] << 16) + (data[25] << 24);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data.data() + 54);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Invalid texture format" << std::endl;
    }
}