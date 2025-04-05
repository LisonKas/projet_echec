#include "ObjModel.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

ObjModel::ObjModel(const std::string& path, const std::string& mtlPath) {
    loadObj(path);
    loadMtl(mtlPath);
    setupMesh();
}

ObjModel::~ObjModel() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void ObjModel::draw(Shader& shader, const glm::mat4& modelMatrix) {
    shader.use();
    shader.setMat4("model", &modelMatrix[0][0]);

    glBindVertexArray(VAO);
    for (const auto& materialPair : materials) {
        const Material& material = materialPair.second;
        shader.setVec3("Ka", material.Ka);
        shader.setVec3("Kd", material.Kd);
        shader.setVec3("Ks", material.Ks);
        shader.setFloat("Ns", material.Ns);

        if (material.textureID) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, material.textureID);
            shader.setInt("texture1", 0);
        }

        // Draw the part of the mesh with this material
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}

void ObjModel::loadObj(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Erreur ouverture fichier: " << path << std::endl;
        return;
    }

    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec3> temp_normals;
    std::vector<glm::vec2> temp_texcoords;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            glm::vec3 pos;
            iss >> pos.x >> pos.y >> pos.z;
            temp_positions.push_back(pos);
        } else if (prefix == "vt") {
            glm::vec2 tex;
            iss >> tex.x >> tex.y;
            temp_texcoords.push_back(tex);
        } else if (prefix == "vn") {
            glm::vec3 norm;
            iss >> norm.x >> norm.y >> norm.z;
            temp_normals.push_back(norm);
        } else if (prefix == "f") {
            std::string vertex;
            for (int i = 0; i < 3; ++i) {
                iss >> vertex;
                std::replace(vertex.begin(), vertex.end(), '/', ' ');
                std::istringstream vss(vertex);

                unsigned int posIndex, texIndex, normIndex;
                vss >> posIndex >> texIndex >> normIndex;

                Vertex vert;
                vert.position = temp_positions[posIndex - 1];
                vert.texCoord = temp_texcoords[texIndex - 1];
                vert.normal   = temp_normals[normIndex - 1];
                vertices.push_back(vert);
                indices.push_back(indices.size());
            }
        }
    }
}

void ObjModel::loadMtl(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Erreur ouverture fichier MTL: " << path << std::endl;
        return;
    }

    std::string line, currentMaterialName;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "newmtl") {
            iss >> currentMaterialName;
            materials[currentMaterialName] = Material();
        } else if (prefix == "Ka") {
            iss >> materials[currentMaterialName].Ka.r >> materials[currentMaterialName].Ka.g >> materials[currentMaterialName].Ka.b;
        } else if (prefix == "Kd") {
            iss >> materials[currentMaterialName].Kd.r >> materials[currentMaterialName].Kd.g >> materials[currentMaterialName].Kd.b;
        } else if (prefix == "Ks") {
            iss >> materials[currentMaterialName].Ks.r >> materials[currentMaterialName].Ks.g >> materials[currentMaterialName].Ks.b;
        } else if (prefix == "Ns") {
            iss >> materials[currentMaterialName].Ns;
        } else if (prefix == "map_Kd") {
            std::string texturePath;
            iss >> texturePath;
            materials[currentMaterialName].loadTexture(texturePath);
        }
    }
}

void ObjModel::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);



    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}