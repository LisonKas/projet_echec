#include "Chessboard.hpp"
#include <imgui.h>
#include <iostream>
#include <utility>
#include <vector>
#include "glad/glad.h"
#include "quick_imgui/quick_imgui.hpp"

// Création du plateau

void Chessboard::InitializeBoardList()
{
    std::vector<char> j_as_chars = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    int               id{1};
    for (int i{0}; i < 8; i++)
    {
        std::vector<Square> line_squares;
        for (int j{0}; j < 8; j++)
        {
            Square new_square;
            new_square.m_id = id;
            if (i % 2 == 0)
            {
                new_square.m_color_light = j % 2 == 0 ? false : true;
            }
            else
            {
                new_square.m_color_light = j % 2 == 0 ? true : false;
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
    // Affiche le joueur actuel
    ImGui::Text(m_teamPlaying ? "Tour des Blancs" : "Tour des Noirs");

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
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.1f, 1.0f)); // Presque Noir
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
    }
}

void Chessboard::CapturePiece(const std::pair<int, int>& target)
{
    Piece* selectedPiece = m_pieces.GetPieceAt(m_selectedPiece);
    Piece* capturedPiece = m_pieces.GetPieceAt(target);

    if (selectedPiece && capturedPiece && selectedPiece->getTeam() != capturedPiece->getTeam())
    {
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