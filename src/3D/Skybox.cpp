#include "Skybox.hpp"
#include <iostream>
#include <vector>

GLfloat skyboxVertices[] = {
//     // Positions
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

std::vector<std::string> faces = {
    "right.jpg",
    "left.jpg",
    "top.jpg",
    "bottom.jpg",
    "front.jpg",
    "back.jpg"
};

void Skybox::InitializeSkybox() {
    //Generate and bind the VAO
    glGenVertexArrays(1, &this->m_VAO);
    glGenBuffers(1, &this->m_VBO);
    glBindVertexArray(this->m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //Load the cubemap texture
    // this->m_Texture = loadCubemap();
}

// void Skybox::DrawSkybox() {
//     glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
//     glBindVertexArray(this->m_VAO);
//     glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_Texture);
//     glDrawArrays(GL_TRIANGLES, 0, 36);
//     glDepthFunc(GL_LESS);  // Set depth function back to default
// }

GLuint Skybox::loadCubemap() {
    GLuint textureID;
    // glGenTextures(1, &textureID);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    // int width, height, nrChannels;
    // unsigned char* data;
    // for (unsigned int i = 0; i < faces.size(); i++) {
    //     //Load the texture data (using stb_image.h or any other image loading library)
    //     stbi_set_flip_vertically_on_load(false);
    //     data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
    //     if (data) {
    //         glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //         stbi_image_free(data);
    //     } else {
    //         std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
    //         stbi_image_free(data);
    //     }
    // }
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
