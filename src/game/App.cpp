#include "App.hpp"
#include <imgui.h>
#include <iostream>
#include "quick_imgui/quick_imgui.hpp"

void App::InitializeGame()
{
    m_chessboard.InitializeBoardList();
    m_renderer.setPieces(&m_chessboard.m_pieces);
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

    // Afficher le pop-up de promotion si le flag est activé
    if (m_chessboard.showPromotionPopup)
    {
        // Afficher le pop-up pour la promotion
        ImGui::OpenPopup("Promotion");

        if (ImGui::BeginPopupModal("Promotion", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Modal))
        {
            ImGui::Text("Promouvoir le pion!");

            // Boutons manuels
            if (ImGui::Button("Reine"))
            {
                m_chessboard.selectedPawn->setType(PieceType::Queen);
                m_chessboard.showPromotionPopup = false;
            }
            if (ImGui::Button("Tour"))
            {
                m_chessboard.selectedPawn->setType(PieceType::Rook);
                m_chessboard.showPromotionPopup = false;
            }
            if (ImGui::Button("Fou"))
            {
                m_chessboard.selectedPawn->setType(PieceType::Bishop);
                m_chessboard.showPromotionPopup = false;
            }
            if (ImGui::Button("Cavalier"))
            {
                m_chessboard.selectedPawn->setType(PieceType::Knight);
                m_chessboard.showPromotionPopup = false;
            }

            // Nouveau bouton de promotion aléatoire
            if (ImGui::Button("Promotion Aléatoire 🎲"))
            {
                // Loi exponentielle de paramètre lambda = 0.7675
                float lambda = 0.7675f;
                float u      = static_cast<float>(rand()) / RAND_MAX;
                float x      = -log(u) / lambda; // Tirage exponentiel

                // Discrétisation bornée entre 0 et 3
                int choix = std::min(static_cast<int>(x), 3);

                // Attribution selon le tirage
                switch (choix)
                {
                case 0:
                    m_chessboard.selectedPawn->setType(PieceType::Knight); // Fréquent
                    break;
                case 1:
                    m_chessboard.selectedPawn->setType(PieceType::Bishop);
                    break;
                case 2:
                    m_chessboard.selectedPawn->setType(PieceType::Rook);
                    break;
                case 3:
                    m_chessboard.selectedPawn->setType(PieceType::Queen); // ~10%
                    break;
                }

                m_chessboard.showPromotionPopup = false; // Fermer le pop-up
            }

            ImGui::EndPopup();
        }
    }

    ImGui::End();
    m_renderer.render();
}

void App::CloseGame()
{
    m_renderer.close();
}