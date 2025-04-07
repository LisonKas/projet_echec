#pragma once
#include "Camera.hpp"
#include "Shader.hpp"
#include "Skybox.hpp"
#include "ObjModel.hpp"
#include <vector>

#include "../pieces/AllPieces.hpp" 

class Renderer3D {
    private:
        Skybox m_skybox;
        ObjModel* m_chessboard = nullptr;
        std::unordered_map<Piece*, ObjModel*> m_displayedPieces;
        Camera m_camera = Camera(glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.05f);
        Shader* m_Shader = nullptr;

        AllPieces* m_pieces = nullptr;

        glm::vec3 getBoardPosition(int col, int row) {
            return glm::vec3(-3.5f + col, 0.0f, -3.5f + row);
        }

    public:
        void initialize();
        void init_pieces();
        void render();
        void close();

        void setPieces(AllPieces* pieces);
        void loadPieceModel(Piece& piece, const std::string& color);
        void update(AllPieces* pieces);
        std::string getModelName(Piece& piece, const std::string& color);
        void setPieceModel(Piece* piece);
};