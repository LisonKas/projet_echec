#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include "Material.hpp"
#include "Shader.hpp"

class ObjModel {
    public:
        ObjModel(const std::string& path, const std::string& mtlPath);
        ~ObjModel();

        void draw(Shader& shader);

    private:
        struct Vertex {
            glm::vec3 m_position;
            glm::vec3 m_normal;
            glm::vec2 m_texCoord;
        };

        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
        std::unordered_map<std::string, Material> m_materials;
        GLuint m_VAO = 0, m_VBO = 0, m_EBO = 0;

        void loadObj(const std::string& path);
        void loadMtl(const std::string& path);
        void setupMesh();
};