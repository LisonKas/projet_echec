#include "Renderer3D.hpp"
#include <iostream>
#include <unordered_map>
#include "../laws/maths.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"

void Renderer3D::initialize()
{ // Initialisation des ressources 3D
    m_skybox.InitializeSkybox();
    init_pieces();
    m_Shader     = new Shader("../../src/3D/shaders/model.vs.glsl", "../../src/3D/shaders/model.fs.glsl");
    m_chessboard = new ObjModel("../../models/lison_chessboard.obj", "../../models/lison_chessboard.mtl");
}

void Renderer3D::setPieces(AllPieces* pieces)
{
    m_pieces = pieces;
}

glm::vec3 Renderer3D::getBoardPosition(int col, int row)
{ // Adapter les coords des pièces sur la 3D
    return glm::vec3(-3.5f + col, 0.0f, -3.5f + row);
}

void Renderer3D::init_pieces()
{ // Initialisation des chemins pour les modèles des pièces
    if (!m_pieces)
        return;

    auto loadPieceModel = [this](Piece& piece, const std::string& color) {
        std::string modelName;
        switch (piece.getType())
        {
        case PieceType::Pawn: modelName = color + "_pawn"; break;
        case PieceType::Rook: modelName = color + "_rook"; break;
        case PieceType::Knight: modelName = color + "_knight"; break;
        case PieceType::Bishop: modelName = color + "_bishop"; break;
        case PieceType::Queen: modelName = color + "_queen"; break;
        case PieceType::King: modelName = color + "_king"; break;
        }
        std::string default_path  = "../../models/pieces/";
        std::string modelPath     = default_path + (color == "white" ? "Whites/" : "Blacks/") + modelName + ".obj";
        std::string mtlPath       = default_path + (color == "white" ? "Whites/" : "Blacks/") + modelName + ".mtl";
        m_displayedPieces[&piece] = new ObjModel(modelPath, mtlPath);
    };

    for (Piece& p : m_pieces->m_white_pieces)
        loadPieceModel(p, "white");
    for (Piece& p : m_pieces->m_black_pieces)
        loadPieceModel(p, "black");
}

void Renderer3D::update(AllPieces* pieces)
{ // permet de faire en sorte de garder les bonnes positions des pièces sans bouger tous les pointeurs dès qu'une pièce meurs
    m_pieces = pieces;

    for (Piece& p : m_pieces->m_white_pieces)
    {
        if (p.getStatus())
        {
            if (m_displayedPieces.find(&p) == m_displayedPieces.end())
            {
                std::string modelName = getModelName(p, "white");
                std::string modelPath = "../../models/pieces/Whites/" + modelName + ".obj";
                std::string mtlPath   = "../../models/pieces/Whites/" + modelName + ".mtl";
                m_displayedPieces[&p] = new ObjModel(modelPath, mtlPath);
            }
        }
        else
        {
            if (m_displayedPieces.find(&p) != m_displayedPieces.end())
            {
                delete m_displayedPieces[&p];
                m_displayedPieces.erase(&p);
                m_piecePositions.erase(&p);
            }
        }
    }

    for (Piece& p : m_pieces->m_black_pieces)
    {
        if (p.getStatus())
        {
            if (m_displayedPieces.find(&p) == m_displayedPieces.end())
            {
                std::string modelName = getModelName(p, "black");
                std::string modelPath = "../../models/pieces/Blacks/" + modelName + ".obj";
                std::string mtlPath   = "../../models/pieces/Blacks/" + modelName + ".mtl";
                m_displayedPieces[&p] = new ObjModel(modelPath, mtlPath);
            }
        }
        else
        {
            if (m_displayedPieces.find(&p) != m_displayedPieces.end())
            {
                delete m_displayedPieces[&p];
                m_displayedPieces.erase(&p);
                m_piecePositions.erase(&p);
            }
        }
    }
}

std::string Renderer3D::getModelName(Piece& piece, const std::string& color)
{
    std::string modelName;
    switch (piece.getType())
    {
    case PieceType::Pawn: modelName = color + "_pawn"; break;
    case PieceType::Rook: modelName = color + "_rook"; break;
    case PieceType::Knight: modelName = color + "_knight"; break;
    case PieceType::Bishop: modelName = color + "_bishop"; break;
    case PieceType::Queen: modelName = color + "_queen"; break;
    case PieceType::King: modelName = color + "_king"; break;
    }
    return modelName;
}

void Renderer3D::setPieceModel(Piece* piece)
{ // Pour changer quand un pion est promote
    if (!piece)
        return;

    std::string color     = piece->getTeam() ? "white" : "black";
    std::string modelName = getModelName(*piece, color);

    if (m_displayedPieces.find(piece) != m_displayedPieces.end())
    {
        delete m_displayedPieces[piece];
        m_displayedPieces.erase(piece);
    }

    std::string default_path = "../../models/pieces/";
    std::string modelPath    = default_path + (piece->getTeam() ? "Whites/" : "Blacks/") + modelName + ".obj";
    std::string mtlPath      = default_path + (piece->getTeam() ? "Whites/" : "Blacks/") + modelName + ".mtl";

    m_displayedPieces[piece] = new ObjModel(modelPath, mtlPath);
}

float Renderer3D::calculateSpeed(Piece* piece, float distance)
{
    // Si c'est la première fois que la pièce se déplace, calculer la durée et la vitesse
    if (m_pieceDurations.find(piece) == m_pieceDurations.end())
    {
        // Utiliser logNormalMapped pour obtenir la durée du déplacement (en secondes)
        float duration = logNormalMapped(-1.0f, 0.5f, 1.0f, 3.0f, 100); // Durée entre 1 et 3 secondes

        // Calculer la vitesse nécessaire pour que le mouvement dure la durée calculée
        float speed = distance / duration;

        // Stocker la vitesse et la durée dans une map
        m_pieceSpeeds[piece]    = speed;
        m_pieceDurations[piece] = duration;

        return speed;
    }
    else
    {
        // Si la pièce a déjà une vitesse calculée, la retourner
        return m_pieceSpeeds[piece];
    }
}

void Renderer3D::render(bool teamPlaying)
{
    float elapsed_time = std::chrono::duration<float>(
                             std::chrono::steady_clock::now() - m_startTime
    )
                             .count();

    // La loop pour dessiner les éléments
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glDisable(GL_CULL_FACE);

    m_camera.processInput();

    const glm::mat4 view        = m_camera.getViewMatrix();
    float           aspectRatio = 16.0f / 9.0f;
    const glm::mat4 projection  = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
    glm::mat4       model       = glm::mat4(1.0f);

    m_skybox.DrawSkybox(glm::value_ptr(view), glm::value_ptr(projection));

    m_Shader->use();
    // uniform mvp
    m_Shader->setMat4("view", glm::value_ptr(view));
    m_Shader->setMat4("projection", glm::value_ptr(projection));
    m_Shader->setMat4("model", &model[0][0]);
    // uniform light
    m_Shader->setVec3("lightPos1", glm::vec3(0.0f, 10.0f, 0.0f));
    m_Shader->setVec3("viewPos", m_camera.getPosition());
    if (teamPlaying)
    {
        m_Shader->setVec3("lightColor1", glm::vec3(1.0f, 1.0f, 1.0f));
    }
    else
    {
        m_Shader->setVec3("lightColor1", glm::vec3(0.5f, 0.3f, 0.8f));
    }

    m_Shader->setVec3("lightPos2", glm::vec3(6 * sin(elapsed_time * 0.8f), 2.0f, 6 * cos(elapsed_time * 0.8f)));
    m_Shader->setVec3("lightColor2", glm::vec3(0.8f, 0.5f, 0.5f));

    m_chessboard->draw(*m_Shader);

    for (auto& [piece, model] : m_displayedPieces)
    {
        if (!piece->getStatus())
            break;

        int col = piece->getCoords().first % 8;
        int row = piece->getCoords().second % 8;

        glm::vec3  targetPos  = getBoardPosition(row, col);
        glm::vec3& currentPos = m_piecePositions[piece];

        if (currentPos == glm::vec3(0))
        {
            currentPos = targetPos;
        }

        // Si la pièce n'est pas à sa position cible
        if (glm::distance(currentPos, targetPos) > 0.01f)
        {
            // Calculer la distance à parcourir
            float distance = glm::distance(currentPos, targetPos);

            // Calculer la vitesse nécessaire pour le mouvement
            float speed = calculateSpeed(piece, distance);

            // Mettre à jour la position de la pièce en fonction de la vitesse
            currentPos = glm::mix(currentPos, targetPos, speed * ImGui::GetIO().DeltaTime);
        }
        else
        {
            currentPos = targetPos;

            // Une fois que la pièce a atteint la destination, supprimer les informations stockées
            m_pieceSpeeds.erase(piece);
            m_pieceDurations.erase(piece);
        }

        // Mettre à jour la matrice modèle pour dessiner la pièce
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), currentPos);
        m_Shader->setMat4("model", &modelMatrix[0][0]);
        model->draw(*m_Shader);
    }
}

void Renderer3D::close()
{
    m_skybox.Destroy();

    delete m_chessboard;
    m_chessboard = nullptr;
    delete m_Shader;
    m_Shader = nullptr;

    for (auto& [piece, model] : m_displayedPieces)
    {
        delete model;
    }
    m_displayedPieces.clear();
    m_piecePositions.clear();
}