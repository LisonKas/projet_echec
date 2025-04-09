#include "ObjModel.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "glm/fwd.hpp"

ObjModel::ObjModel(const std::string& path, const std::string& mtlPath) {
    loadObj(path);
    loadMtl(mtlPath);
    setupMesh();
}

ObjModel::~ObjModel() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void ObjModel::draw(Shader& shader) { 
    glBindVertexArray(m_VAO);
    for (const auto& materialPair : m_materials) {
        const Material& material = materialPair.second;
        shader.setVec3("Ka", material.Ka);
        shader.setVec3("Kd", material.Kd);
        shader.setVec3("Ks", material.Ks);
        shader.setFloat("Ns", material.Ns);

        bool useTexture = (material.textureID != 0);
        shader.setBool("useTexture", useTexture);

        if (useTexture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, material.textureID);
            shader.setInt("texture1", 0);
        }

        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
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
                vert.m_position = temp_positions[posIndex - 1];
                vert.m_texCoord = temp_texcoords[texIndex - 1];
                vert.m_normal   = temp_normals[normIndex - 1];
                m_vertices.push_back(vert);
                m_indices.push_back(m_indices.size());
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
            m_materials[currentMaterialName] = Material();
        } else if (prefix == "Ka") {
            iss >> m_materials[currentMaterialName].Ka.r >> m_materials[currentMaterialName].Ka.g >> m_materials[currentMaterialName].Ka.b;
        } else if (prefix == "Kd") {
            iss >> m_materials[currentMaterialName].Kd.r >> m_materials[currentMaterialName].Kd.g >> m_materials[currentMaterialName].Kd.b;
        } else if (prefix == "Ks") {
            iss >> m_materials[currentMaterialName].Ks.r >> m_materials[currentMaterialName].Ks.g >> m_materials[currentMaterialName].Ks.b;
        } else if (prefix == "Ns") {
            iss >> m_materials[currentMaterialName].Ns;
        } else if (prefix == "map_Kd") {
            std::string texturePath;
            iss >> texturePath;
            m_materials[currentMaterialName].loadTexture(texturePath);
        }
    }
}

void ObjModel::setupMesh() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_texCoord));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}