#pragma once
#include <chrono>
#include "../pieces/AllPieces.hpp"
#include "Camera.hpp"
#include "ObjModel.hpp"
#include "Shader.hpp"
#include "Skybox.hpp"

class Renderer3D {
private:
    Skybox                                m_skybox;
    ObjModel*                             m_chessboard = nullptr;
    std::unordered_map<Piece*, ObjModel*> m_displayedPieces;
    std::unordered_map<Piece*, glm::vec3> m_piecePositions;
    Camera                                m_camera = Camera(glm::vec3(0.0f, 7.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.05f);
    Shader*                               m_Shader = nullptr;

    AllPieces* m_pieces = nullptr;

    glm::vec3 getBoardPosition(int col, int row);

    // Pour conserver la vitesse au fil des cases
    std::map<Piece*, float>               m_pieceSpeeds;
    std::map<Piece*, float>               m_pieceDurations;
    std::chrono::steady_clock::time_point m_startTime = std::chrono::steady_clock::now();

public:
    void initialize();
    void init_pieces();
    void render(bool teamPlaying);
    void close();

    void        setPieces(AllPieces* pieces);
    void        loadPieceModel(Piece& piece, const std::string& color);
    void        update(AllPieces* pieces);
    std::string getModelName(Piece& piece, const std::string& color);
    void        setPieceModel(Piece* piece);
    float       calculateSpeed(Piece* piece, float distance);
};