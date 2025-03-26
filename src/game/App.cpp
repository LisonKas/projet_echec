#include "App.hpp"
#include <imgui.h>
#include <iostream>
#include "quick_imgui/quick_imgui.hpp"

void App::InitializeGame()
{
    m_chessboard.InitializeBoardList();
    m_renderer.initialize();
}

void App::StartGame()
{
    ImGui::Begin("Game");

    // Si la partie est terminée, affiche le message et le bouton Rejouer
    if (m_chessboard.m_isGameOver)
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Fin de la partie !");
        ImGui::Text(m_chessboard.m_winnerMessage.c_str());

        if (ImGui::Button("Rejouer"))
        {
            m_chessboard.InitializeBoardList(); // Réinitialise le plateau et les pièces
        }
    }

    // Toujours afficher le plateau
    m_chessboard.CreateBoard();

    ImGui::End();
    m_renderer.render();
}

void App::CloseGame()
{
    m_renderer.close();
}