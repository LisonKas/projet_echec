#include "Renderer3D.hpp"
#include <iostream>
#include <unordered_map>
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void Renderer3D::initialize() {
    m_skybox.InitializeSkybox();
    init_pieces();
    m_Shader = new Shader("../../src/3D/shaders/chessboard.vs.glsl",  "../../src/3D/shaders/chessboard.fs.glsl");
    m_chessboard = new ObjModel("../../models/lison_chessboard.obj", "../../models/lison_chessboard.mtl");
}

void Renderer3D::setPieces(AllPieces* pieces) {
    m_pieces = pieces;
}

void Renderer3D::init_pieces() {
    if (!m_pieces) return;

    for (Piece& piece : m_pieces->m_black_pieces) {
        std::string modelName;
        switch (piece.getType()) {
            case PieceType::Pawn:   modelName = "black_pawn"; break;
            case PieceType::Rook:   modelName = "black_rook"; break;
            case PieceType::Knight: modelName = "black_knight"; break;
            case PieceType::Bishop: modelName = "black_bishop"; break;
            case PieceType::Queen:  modelName = "black_queen"; break;
            case PieceType::King:   modelName = "black_king"; break;
        }
        DisplayedPiece dp;
        dp.logicPiece = &piece;
        dp.model = new ObjModel("../../models/pieces/Blacks/" + modelName + ".obj", "../../models/pieces/Blacks/" + modelName + ".mtl");
        m_displayedBlackPieces.push_back(dp);
    }

    for (Piece& piece : m_pieces->m_white_pieces) {
        std::string modelName;
        switch (piece.getType()) {
            case PieceType::Pawn:   modelName = "white_pawn"; break;
            case PieceType::Rook:   modelName = "white_rook"; break;
            case PieceType::Knight: modelName = "white_knight"; break;
            case PieceType::Bishop: modelName = "white_bishop"; break;
            case PieceType::Queen:  modelName = "white_queen"; break;
            case PieceType::King:   modelName = "white_king"; break;
        }
        DisplayedPiece dp;
        dp.logicPiece = &piece;
        dp.model = new ObjModel("../../models/pieces/Whites/" + modelName + ".obj", "../../models/pieces/Whites/" + modelName + ".mtl");
        m_displayedWhitePieces.push_back(dp);
    }
}

void Renderer3D::render() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    m_camera.processInput();

    glm::mat4 view = m_camera.getViewMatrix();
    float aspectRatio = 16.0f / 9.0f;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);

    m_skybox.DrawSkybox(glm::value_ptr(view), glm::value_ptr(projection));

    m_Shader->use();
    //uniform mvp
    m_Shader->setMat4("view", glm::value_ptr(view));
    m_Shader->setMat4("projection", glm::value_ptr(projection));
    m_Shader->setMat4("model", &model[0][0]);
    //uniform light
    m_Shader->setVec3("lightPos", glm::vec3(0.0f, 0.0f, 10.0f));
    m_Shader->setVec3("viewPos", glm::vec3(0.0f, 0.0f, 10.0f));
    m_Shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    m_chessboard->draw(*m_Shader);

    for (int i = 0; i < m_displayedWhitePieces.size(); ++i) {
        int col = m_displayedWhitePieces[i].logicPiece->getCoords().first % 8;
        int row = m_displayedWhitePieces[i].logicPiece->getCoords().second % 8;

        glm::vec3 pos = glm::vec3(-3.5f + row, 0.0f, -3.5f + col); //getBoardPosition(col, row);
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), pos);
        m_Shader->setMat4("model", &modelMatrix[0][0]);
        m_displayedWhitePieces[i].model->draw(*m_Shader);
    }

    for (int i = 0; i < m_displayedBlackPieces.size(); ++i) {
        int col = m_displayedBlackPieces[i].logicPiece->getCoords().first % 8;
        int row = m_displayedBlackPieces[i].logicPiece->getCoords().second % 8;

        glm::vec3 pos = glm::vec3(-3.5f + row, 0.0f, -3.5f + col); 
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), pos);
        m_Shader->setMat4("model", &modelMatrix[0][0]);
        m_displayedBlackPieces[i].model->draw(*m_Shader);
    }
}

void Renderer3D::close() {
    m_skybox.Destroy();

    delete m_chessboard;
    m_chessboard = nullptr;
    delete m_Shader;
    m_Shader = nullptr;

    for (auto& dp : m_displayedBlackPieces) delete dp.model;
    for (auto& dp : m_displayedWhitePieces) delete dp.model;

    m_displayedBlackPieces.clear();
    m_displayedWhitePieces.clear();
}