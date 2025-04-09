#include "ObjModel.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "glm/fwd.hpp"
#include <map>

ObjModel::ObjModel(const std::string& path, const std::string& mtlPath) {
    loadMtl(mtlPath);
    loadObj(path);
    setupMesh();
}

ObjModel::~ObjModel() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void ObjModel::draw(Shader& shader) { 
    glBindVertexArray(m_VAO);
    
    if (m_materials.empty()) {
        shader.setVec3("Ka", glm::vec3(0.2f));
        shader.setVec3("Kd", glm::vec3(0.8f));
        shader.setVec3("Ks", glm::vec3(1.0f));
        shader.setFloat("Ns", 32.0f);
        shader.setBool("useTexture", false);
        
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    } 
    else {
        auto& material = m_materials.begin()->second;

        for(auto& material : m_materials) {
            bool useTexture = (material.second.textureID != 0);
            if (useTexture) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, material.second.textureID);
                shader.setInt("texture1", 0);
            }
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
    std::string currentMaterial;
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
        } else if (prefix == "usemtl") {
            iss >> currentMaterial;
        } else if (prefix == "f") {
            std::string vertex;
            std::vector<unsigned int> face_indices;
            
            for (int i = 0; i < 3; ++i) {
                iss >> vertex;
                std::replace(vertex.begin(), vertex.end(), '/', ' ');
                std::istringstream vss(vertex);

                unsigned int posIndex, texIndex, normIndex;
                vss >> posIndex >> texIndex >> normIndex;

                posIndex--; 
                texIndex--;
                normIndex--;

                Vertex vert;
                vert.m_position = temp_positions[posIndex];
                
                if (texIndex < temp_texcoords.size()) {
                    vert.m_texCoord = temp_texcoords[texIndex];
                } else {
                    vert.m_texCoord = glm::vec2(0.0f, 0.0f);
                }
                
                if (normIndex < temp_normals.size()) {
                    vert.m_normal = temp_normals[normIndex];
                } else {
                    vert.m_normal = glm::vec3(0.0f, 1.0f, 0.0f);
                }

                vert.Kd = m_materials[currentMaterial].Kd;
                vert.Ka = m_materials[currentMaterial].Ka;
                vert.Ks = m_materials[currentMaterial].Ks;
                vert.Ns = m_materials[currentMaterial].Ns;
                vert.useTexture = m_materials[currentMaterial].textureID != 0 ? 1 : 0;
                
                m_vertices.push_back(vert);
                m_indices.push_back(m_indices.size());
                
                if (!currentMaterial.empty()) {
                    m_materialGroups[currentMaterial].push_back(m_indices.size() - 1);
                }
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
            m_materials[currentMaterialName].name = currentMaterialName;
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

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Kd));
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Ks));
    glEnableVertexAttribArray(4);

    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Ka));
    glEnableVertexAttribArray(5);

    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Ns));
    glEnableVertexAttribArray(6);

    glVertexAttribPointer(7, 1, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, useTexture));
    glEnableVertexAttribArray(7);

    glBindVertexArray(0);
}