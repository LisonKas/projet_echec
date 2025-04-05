#include "ObjModel.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

ObjModel::ObjModel(const std::string& path) {
    loadObj(path);
    setupMesh();
}

ObjModel::~ObjModel() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void ObjModel::draw(Shader& shader, const glm::mat4& modelMatrix) {
    shader.use();
    shader.setMat4("uModel", &modelMatrix[0][0]);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
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
            for (int i = 0; i < 3; ++i) { // triangle uniquement
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
            }
        }
    }
}

void ObjModel::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // TexCoords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(1);

    // Normales
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}