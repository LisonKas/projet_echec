#include "Chessboard.hpp"
#include <imgui.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <vector>
#include "../laws/maths.hpp"
#include "glad/glad.h"

#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <filesystem>
#endif

// Création du plateau

void Chessboard::Reinitialize()
{
    m_teamPlaying = true;
    m_isGameOver  = false;
    m_winnerMessage.clear();
    m_boardlist.clear();
    m_highlightedSquares.clear();
    m_selectedPiece = {-1, -1};
}

ImVec4 Chessboard::generateFancyDarkColor()
{
    float base = 0.1f;
    float r    = std::clamp(base + gaussian() * 0.2f, 0.0f, 1.0f);
    float g    = std::clamp(base + gaussian() * 0.2f, 0.0f, 1.0f);
    float b    = std::clamp(base + gaussian() * 0.2f, 0.0f, 1.0f);

    return ImVec4(r, g, b, 1.0f);
}

void Chessboard::InitializeBoardList()
{
    Reinitialize();

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
                new_square.m_dark_color = generateFancyDarkColor();
            }

            if (i == 0 || i == 1 || i == 6 || i == 7)
            {
                new_square.m_is_occupied = true;
            }

            new_square.m_coords = std::pair<char, int>(j_as_chars[j], 8 - i);
            line_squares.push_back(new_square);
            id++;
        }
        m_boardlist.push_back(line_squares);
    }

    m_pieces.InitializeAllPieces();
}

void Chessboard::CreateBoard()
{
    if (m_isGameOver)
    {
        return; // Pour ne plus pouvoir bouger les pièces
    }

    ImGui::Text(m_teamPlaying ? "Tour des Blancs" : "Tour des Noirs");

    for (int i{0}; i < 8; i++)
    {
        for (int j{0}; j < 8; j++)
        {
            SetSquareColor(i, j);

            ImGui::PushID(m_boardlist[i][j].m_id);
            GLuint piece_label = m_pieces.PiecesAppear(i, j);

            // Annulation avec clic droit
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            {
                ResetSelection();
                ImGui::PopID();
                ImGui::PopStyleColor();
                continue;
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
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    }
    else if (this->m_boardlist[i][j].m_color_light)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
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

        if (selectedPiece->getType() == PieceType::Pawn && (destination.first == 0 || destination.first == 7) && !m_isGameOver)
        {
            std::cout << "Promoted!" << std::endl;
            PromotePawn(selectedPiece);
        }
    }
}

void Chessboard::PlayCaptureSound()
{
    // Choisir un fichier audio en fonction de la loi de Poisson
    int         numEvents = generatePoisson(2.0f); // Par exemple, lambda = 2 pour une moyenne de 2 événements
    std::string fileToPlay;

    // En fonction du nombre d'événements générés, on choisit un fichier
    switch (numEvents)
    {
    case 0:
        fileToPlay = "..\\..\\sound\\munch.wav";
        break;
    case 1:
        fileToPlay = "..\\..\\sound\\error.wav";
        break;
    case 2:
        fileToPlay = "..\\..\\sound\\minecraft.wav";
        break;
    default:
        fileToPlay = "..\\..\\sound\\pop.wav";
        break;
    }

#ifdef _WIN32
    ShellExecute(nullptr, "open", fileToPlay.c_str(), nullptr, nullptr, SW_HIDE);
#elif __APPLE__
    std::string command = "afplay " + fileToPlay + " &";
    system(command.c_str());
#else // Linux
    std::string command = "aplay " + fileToPlay + " &";
    system(command.c_str());
#endif
}

void Chessboard::CapturePiece(const std::pair<int, int>& target)
{
    Piece* selectedPiece = m_pieces.GetPieceAt(m_selectedPiece);
    Piece* capturedPiece = m_pieces.GetPieceAt(target);

    if (selectedPiece && capturedPiece && selectedPiece->getTeam() != capturedPiece->getTeam())
    {
        PlayCaptureSound();

        // Détection de la victoire
        if (capturedPiece->getType() == PieceType::King)
        {
            m_isGameOver    = true;
            m_winnerMessage = selectedPiece->getTeam() ? "Les Blancs gagnent !" : "Les Noirs gagnent !";
        }
        m_pieces.RemovePieceAt(target);
        MovePiece(target);
    }
}

bool Chessboard::GetTeamPlaying() const
{
    return m_teamPlaying;
}

void Chessboard::changeTurn()
{
    m_teamPlaying = !m_teamPlaying;
}

void Chessboard::HandlePieceMove(const std::pair<int, int>& clickedSquare)
{
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
    showPromotionPopup = true;
    selectedPawn       = pawn;
}
