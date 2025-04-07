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

    auto loadPieceModel = [this](Piece& piece, const std::string& color) {
        std::string modelName;
        switch (piece.getType()) {
            case PieceType::Pawn:   modelName = color + "_pawn"; break;
            case PieceType::Rook:   modelName = color + "_rook"; break;
            case PieceType::Knight: modelName = color + "_knight"; break;
            case PieceType::Bishop: modelName = color + "_bishop"; break;
            case PieceType::Queen:  modelName = color + "_queen"; break;
            case PieceType::King:   modelName = color + "_king"; break;
        }
        std::string default_path = "../../models/pieces/";
        std::string modelPath = default_path + (color == "white" ? "Whites/" : "Blacks/") + modelName + ".obj";
        std::string mtlPath = default_path + (color == "white" ? "Whites/" : "Blacks/") + modelName + ".mtl";
        m_displayedPieces[&piece] = new ObjModel(modelPath, mtlPath);
    };

    for (Piece& p : m_pieces->m_white_pieces) loadPieceModel(p, "white");
    for (Piece& p : m_pieces->m_black_pieces) loadPieceModel(p, "black");
}

void Renderer3D::update(AllPieces* pieces) {
    m_pieces = pieces; 

    for (Piece& p : m_pieces->m_white_pieces) {
        if (p.getStatus()) {  
            if (m_displayedPieces.find(&p) == m_displayedPieces.end()) {
                std::string modelName = getModelName(p, "white");
                std::string modelPath = "../../models/pieces/Whites/" + modelName + ".obj";
                std::string mtlPath = "../../models/pieces/Whites/" + modelName + ".mtl";
                m_displayedPieces[&p] = new ObjModel(modelPath, mtlPath);
            }
        } else {
            if (m_displayedPieces.find(&p) != m_displayedPieces.end()) {
                delete m_displayedPieces[&p];
                m_displayedPieces.erase(&p);
                m_piecePositions.erase(&p);
            }
        }
    }

    for (Piece& p : m_pieces->m_black_pieces) {
        if (p.getStatus()) {  
            if (m_displayedPieces.find(&p) == m_displayedPieces.end()) {
                std::string modelName = getModelName(p, "black");
                std::string modelPath = "../../models/pieces/Blacks/" + modelName + ".obj";
                std::string mtlPath = "../../models/pieces/Blacks/" + modelName + ".mtl";
                m_displayedPieces[&p] = new ObjModel(modelPath, mtlPath);
            }
        } else {
            if (m_displayedPieces.find(&p) != m_displayedPieces.end()) {
                delete m_displayedPieces[&p];
                m_displayedPieces.erase(&p);
                m_piecePositions.erase(&p);
            }
        }
    }
}

std::string Renderer3D::getModelName(Piece& piece, const std::string& color) {
    std::string modelName;
    switch (piece.getType()) {
        case PieceType::Pawn:   modelName = color + "_pawn"; break;
        case PieceType::Rook:   modelName = color + "_rook"; break;
        case PieceType::Knight: modelName = color + "_knight"; break;
        case PieceType::Bishop: modelName = color + "_bishop"; break;
        case PieceType::Queen:  modelName = color + "_queen"; break;
        case PieceType::King:   modelName = color + "_king"; break;
    }
    return modelName;
}

void Renderer3D::setPieceModel(Piece* piece){
    if (!piece) return;

    // Déterminer la couleur avec m_team : true = white, false = black
    std::string color = piece->getTeam() ? "white" : "black";
    std::string modelName = getModelName(*piece, color);

    // Supprimer l'ancien modèle s'il existe
    if (m_displayedPieces.find(piece) != m_displayedPieces.end()) {
        delete m_displayedPieces[piece];
        m_displayedPieces.erase(piece);
    }

    // Construire les chemins vers les fichiers .obj et .mtl
    std::string default_path = "../../models/pieces/";
    std::string modelPath = default_path + (piece->getTeam() ? "Whites/" : "Blacks/") + modelName + ".obj";
    std::string mtlPath = default_path + (piece->getTeam() ? "Whites/" : "Blacks/") + modelName + ".mtl";

    // Charger le nouveau modèle
    m_displayedPieces[piece] = new ObjModel(modelPath, mtlPath);
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
    m_Shader->setVec3("lightPos", glm::vec3(0.0f, 10.0f, 0.0f));
    m_Shader->setVec3("viewPos", m_camera.getPosition());
    m_Shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    m_chessboard->draw(*m_Shader);

    for (auto& [piece, model] : m_displayedPieces) {
        if (!piece->getStatus()) break;  
    
        int col = piece->getCoords().first % 8;
        int row = piece->getCoords().second % 8;
    
        // glm::vec3 pos = getBoardPosition(row, col);
        // glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), pos);
        /////////////////////////////
        glm::vec3 targetPos = getBoardPosition(row, col);

        // Récupère la position actuelle (sinon initialise à la cible directement)
        glm::vec3& currentPos = m_piecePositions[piece];
        if (currentPos == glm::vec3(0)) {
            currentPos = targetPos;
        }

        // Interpolation : approche de la position cible
        float speed = 5.0f * ImGui::GetIO().DeltaTime; // ou un facteur fixe genre 0.1f
        currentPos = glm::mix(currentPos, targetPos, speed);

        // Création de la matrice à partir de la position interpolée
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), currentPos);
        //////////////////////////////////
        m_Shader->setMat4("model", &modelMatrix[0][0]);
        model->draw(*m_Shader);
    }
}

void Renderer3D::close() {
    m_skybox.Destroy();

    delete m_chessboard;
    m_chessboard = nullptr;
    delete m_Shader;
    m_Shader = nullptr;

    for (auto& [piece, model] : m_displayedPieces) {
        delete model;
    }
    m_displayedPieces.clear();
    m_piecePositions.clear();
}