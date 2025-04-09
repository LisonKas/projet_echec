#pragma once

#include <string>
#include <vector>
#include <map>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Material.hpp"
#include "Shader.hpp"

struct Vertex {
    glm::vec3 m_position;     
    glm::vec3 m_normal;      
    glm::vec2 m_texCoord;  
    glm::vec3 Kd;
    glm::vec3 Ka;
    glm::vec3 Ks;
    float Ns;
    int useTexture;
};

class ObjModel {
    private:
        void loadObj(const std::string& path);
        void loadMtl(const std::string& path);
        void setupMesh();
        
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
        
        std::map<std::string, Material> m_materials;
        std::map<std::string, std::vector<unsigned int>> m_materialGroups;
        
        GLuint m_VAO;
        GLuint m_VBO;
        GLuint m_EBO;

    public:
        ObjModel(const std::string& path, const std::string& mtlPath);
        ~ObjModel();

        void draw(Shader& shader);
};