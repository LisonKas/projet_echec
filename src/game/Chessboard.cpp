#include "Chessboard.hpp"
#include <imgui.h>
#include <cmath>
#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include "glad/glad.h"
#include "quick_imgui/quick_imgui.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float generateGaussian()
{
    // Tirages uniformes U1, U2 dans ]0,1]
    float u1 = static_cast<float>(rand()) / RAND_MAX;
    float u2 = static_cast<float>(rand()) / RAND_MAX;

    // Méthode de Box-Muller pour une gaussienne de moyenne 0, écart-type 1
    float z0 = sqrt(-2.0f * log(u1)) * cos(2 * M_PI * u2);

    // Centrer la gaussienne autour de 0.1 et l’adapter à une faible variance
    float mean   = 0.1f;
    float stddev = 0.03f;

    float gray = std::clamp(mean + stddev * z0, 0.0f, 0.2f);
    return gray;
}

ImVec4 generateFancyDarkColor()
{
    // Génère une teinte (H) bien espacée, en évitant les teintes trop proches
    float hue = static_cast<float>(rand()) / RAND_MAX;

    // Saturation et valeur : on reste sombre mais saturé pour des couleurs plus visibles
    float saturation = 0.7f + static_cast<float>(rand()) / RAND_MAX * 0.3f; // 0.7 à 1.0
    float value      = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.2f; // 0.2 à 0.4

    // Conversion HSV vers RGB
    int   i = int(hue * 6);
    float f = hue * 6 - i;
    float p = value * (1 - saturation);
    float q = value * (1 - f * saturation);
    float t = value * (1 - (1 - f) * saturation);

    float r, g, b;
    switch (i % 6)
    {
    case 0:
        r = value;
        g = t;
        b = p;
        break;
    case 1:
        r = q;
        g = value;
        b = p;
        break;
    case 2:
        r = p;
        g = value;
        b = t;
        break;
    case 3:
        r = p;
        g = q;
        b = value;
        break;
    case 4:
        r = t;
        g = p;
        b = value;
        break;
    case 5:
        r = value;
        g = p;
        b = q;
        break;
    }

    return ImVec4(r, g, b, 1.0f);
}

// Création du plateau

void Chessboard::InitializeBoardList()
{
    m_teamPlaying = true;                    // Toujours commencer avec les Blancs
    m_isGameOver  = false;                   // Réinitialise l'état du jeu
    m_winnerMessage.clear();                 // Efface le message du gagnant
    m_boardlist.clear();                     // Assure une réinitialisation complète du plateau
    m_highlightedSquares.clear();            // Efface les cases mises en surbrillance
    m_selectedPiece              = {-1, -1}; // Réinitialise la pièce sélectionnée
    std::vector<char> j_as_chars = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    int               id{1};
    for (int i{0}; i < 8; i++)
    {
        std::vector<Square> line_squares;
        for (int j{0}; j < 8; j++)
        {
            Square new_square;
            new_square.m_id          = id;
            bool is_light            = (i + j) % 2 != 0;
            new_square.m_color_light = is_light;

            if (!is_light)
            {
                new_square.m_dark_color = generateFancyDarkColor(); // Couleur unique fancy
            }
            if (i == 0 || i == 1 || i == 6 || i == 7)
            {
                new_square.m_is_occupied = true;
            }
            new_square.m_coords = std::pair<char, int>(j_as_chars[j], 8 - i);
            line_squares.push_back(new_square);
            id++;
        }
        this->m_boardlist.push_back(line_squares);
    }
    m_pieces.InitializeAllPieces();
}

void Chessboard::CreateBoard()
{
    if (m_isGameOver)
    {
        ImGui::Text(m_winnerMessage.c_str());
        return; // Empêche toute interaction avec le plateau
    }
    // Affiche le joueur actuel
    ImGui::Text(m_teamPlaying ? "Tour des Blancs" : "Tour des Noirs");

    // Si la partie est terminée, affiche le message du gagnant tout en permettant de voir le plateau
    if (m_isGameOver)
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), m_winnerMessage.c_str()); // Message en rouge
        // Continue l'affichage du plateau pour que l'utilisateur puisse encore le voir
    }

    for (int i{0}; i < 8; i++)
    {
        for (int j{0}; j < 8; j++)
        {
            SetSquareColor(i, j); // Appliquer la couleur à la case

            ImGui::PushID(m_boardlist[i][j].m_id);
            GLuint piece_label = m_pieces.PiecesAppear(i, j);

            // Vérifie si le clic droit est effectué, pour désélectionner une pièce
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            {
                ResetSelection();
                ImGui::PopID();
                ImGui::PopStyleColor();
                continue; // Passe à la case suivante sans effectuer d'autres actions
            }

            if (piece_label != 0)
            {
                if (ImGui::ImageButton((void*)(intptr_t)piece_label, ImVec2{92.f, 92.f}))
                {
                    std::pair<int, int> clickedSquare = {i, j};
                    HandlePieceMove(clickedSquare);
                }
            }
            else
            {
                if (ImGui::Button(" ", ImVec2{100.f, 100.f}))
                {
                    std::pair<int, int> clickedSquare = {i, j};
                    HandlePieceMove(clickedSquare);
                }
            }

            ImGui::PopID();
            ImGui::PopStyleColor();

            if (j < 7)
            {
                ImGui::SameLine();
            }
        }
    }
}

void Chessboard::SetSquareColor(int i, int j)
{
    bool isHighlighted = std::find(m_highlightedSquares.begin(), m_highlightedSquares.end(), std::make_pair(i, j)) != m_highlightedSquares.end();

    if (isHighlighted)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); // Vert pour cases accessibles
    }
    else if (this->m_boardlist[i][j].m_color_light)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.9f, 0.9f, 0.9f, 1.0f)); // Presque Blanc
    }
    else
    {
        ImVec4 color = m_boardlist[i][j].m_dark_color;
        ImGui::PushStyleColor(ImGuiCol_Button, color);
    }
}

// Gestion de pièces sur le plateau

void Chessboard::SelectPiece(const std::pair<int, int>& clickedSquare, Piece* selectedPiece)
{
    m_selectedPiece = clickedSquare;
    m_highlightedSquares.clear();

    for (const auto& move : selectedPiece->getZone(&m_boardlist))
    {
        Piece* targetPiece = m_pieces.GetPieceAt(move);
        if (!targetPiece || targetPiece->getTeam() != selectedPiece->getTeam())
        {
            m_highlightedSquares.push_back(move);
        }
    }
}

void Chessboard::ResetSelection()
{
    m_selectedPiece = {-1, -1};
    m_highlightedSquares.clear();
}

void Chessboard::MovePiece(const std::pair<int, int>& destination)
{
    Piece* selectedPiece = m_pieces.GetPieceAt(m_selectedPiece);
    if (selectedPiece)
    {
        selectedPiece->move(destination);
        m_boardlist[destination.first][destination.second].m_is_occupied         = true;
        m_boardlist[m_selectedPiece.first][m_selectedPiece.second].m_is_occupied = false;

        // Gestion de la promotion du pion
        if (selectedPiece->getType() == PieceType::Pawn && (destination.first == 0 || destination.first == 7)) // Si le pion arrive à la dernière ligne
        {
            std::cout << "Promoted!" << std::endl;
            PromotePawn(selectedPiece);
        }
    }
}

void Chessboard::CapturePiece(const std::pair<int, int>& target)
{
    Piece* selectedPiece = m_pieces.GetPieceAt(m_selectedPiece);
    Piece* capturedPiece = m_pieces.GetPieceAt(target);

    if (selectedPiece && capturedPiece && selectedPiece->getTeam() != capturedPiece->getTeam())
    {
        if (capturedPiece->getType() == PieceType::King)
        {
            m_isGameOver    = true;
            m_winnerMessage = selectedPiece->getTeam() ? "Les Blancs gagnent !" : "Les Noirs gagnent !";
        }
        m_pieces.RemovePieceAt(target); // Supprime la pièce capturée
        MovePiece(target);
    }
}

bool Chessboard::teamPlaying() const
{
    return m_teamPlaying;
}

void Chessboard::changeTurn()
{
    m_teamPlaying = !m_teamPlaying;
}

void Chessboard::HandlePieceMove(const std::pair<int, int>& clickedSquare)
{
    // Si une pièce est sélectionnée et que le clic est sur une case valide
    if (m_selectedPiece != std::make_pair(-1, -1) && std::find(m_highlightedSquares.begin(), m_highlightedSquares.end(), clickedSquare) != m_highlightedSquares.end())
    {
        (m_pieces.GetPieceAt(clickedSquare)) ? CapturePiece(clickedSquare) : MovePiece(clickedSquare);
        ResetSelection();
        changeTurn();
        return;
    }

    Piece* selectedPiece = m_pieces.GetPieceAt(clickedSquare);

    if (!selectedPiece)
    {
        ResetSelection();
        return;
    }

    // Vérifie si c'est bien le tour de la couleur de la pièce sélectionnée
    if (selectedPiece->getTeam() != m_teamPlaying)
    {
        std::cerr << "Ce n'est pas votre tour !" << std::endl;
        return;
    }

    if (m_selectedPiece == clickedSquare)
    {
        ResetSelection();
    }
    else
    {
        SelectPiece(clickedSquare, selectedPiece);
    }
}

void Chessboard::PromotePawn(Piece* pawn)
{
    std::cout << "Popup Promotion appelé !" << std::endl;

    // On active le flag pour afficher la promotion
    showPromotionPopup = true;
    selectedPawn       = pawn; // On garde une référence à ce pion pour la promotion
}