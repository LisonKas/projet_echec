#include "App.hpp"
#include <imgui.h>
#include <iostream>
#include "quick_imgui/quick_imgui.hpp"

// Variables globales pour la gestion des noms et du tirage
static char        joueur1[32]     = "";
static char        joueur2[32]     = "";
static bool        noms_valides    = false;
static std::string joueur_blancs   = "";
static bool        tirage_effectue = false;

float random2(int precision)
{
    float x = 0.0f;
    for (int i = 1; i <= precision; i++)
    {
        int bit = std::rand() % 2; // 0 ou 1
        x += static_cast<float>(bit) / std::pow(2, i);
    }
    return x;
}

bool bernoulli(float p, int precision = 20)
{
    float r = random2(precision);
    return r < p;
}

// Tirage exponentiel basé sur random2()
// Renvoie une chaîne non numérique : "Cavalier", "Fou", "Tour" ou "Reine"
std::string tiragePromotionExponentiel()
{
    float lambda = 0.7675f;
    float u      = random2(20); // Utilise ton générateur binaire maison
    float x      = -std::log(u) / lambda;

    int                      choix  = std::min(static_cast<int>(x), 3); // Borné à 3 max
    std::vector<std::string> pieces = {"Cavalier", "Fou", "Tour", "Reine"};
    return pieces[choix];
}

void App::InitializeGame()
{
    m_chessboard.InitializeBoardList();
    m_renderer.setPieces(&m_chessboard.m_pieces);
    m_renderer.initialize();
}

void App::StartGame()
{
    ImGui::Begin("Game");

    // Interface de saisie des noms et tirage au sort
    if (!noms_valides)
    {
        ImGui::InputText("Nom du joueur 1", joueur1, IM_ARRAYSIZE(joueur1));
        ImGui::InputText("Nom du joueur 2", joueur2, IM_ARRAYSIZE(joueur2));

        if (ImGui::Button("Valider les noms") && strlen(joueur1) > 0 && strlen(joueur2) > 0)
        {
            noms_valides    = true;
            tirage_effectue = true;

            // Loi de Bernoulli pour le tirage au sort
            float p = 0.5f; // probabilité que joueur1 ait les blancs
            if (bernoulli(p))
            {
                joueur_blancs = joueur1;
            }
            else
            {
                joueur_blancs = joueur2;
            }
        }
    }
    else if (tirage_effectue)
    {
        ImGui::Text("%s commence avec les blancs !", joueur_blancs.c_str());
    }

    // Si la partie est terminée, affiche le message et le bouton Rejouer
    if (m_chessboard.m_isGameOver)
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Fin de la partie !");
        ImGui::Text(m_chessboard.m_winnerMessage.c_str());

        if (ImGui::Button("Rejouer"))
        {
            joueur1[0]      = '\0';
            joueur2[0]      = '\0';
            noms_valides    = false;
            tirage_effectue = false;
            joueur_blancs   = "";

            m_chessboard.InitializeBoardList(); // Réinitialise le plateau et les pièces
        }
    }

    // Toujours afficher le plateau
    // N'affiche le plateau que si le tirage a été effectué
    if (tirage_effectue)
    {
        m_chessboard.CreateBoard();
    }
    else
    {
        ImGui::Text("Veuillez valider les noms pour commencer la partie.");
    }

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

                m_renderer.setPieceModel(m_chessboard.selectedPawn);
            }
            if (ImGui::Button("Tour"))
            {
                m_chessboard.selectedPawn->setType(PieceType::Rook);
                m_chessboard.showPromotionPopup = false;

                m_renderer.setPieceModel(m_chessboard.selectedPawn);
            }
            if (ImGui::Button("Fou"))
            {
                m_chessboard.selectedPawn->setType(PieceType::Bishop);
                m_chessboard.showPromotionPopup = false;

                m_renderer.setPieceModel(m_chessboard.selectedPawn);
            }
            if (ImGui::Button("Cavalier"))
            {
                m_chessboard.selectedPawn->setType(PieceType::Knight);
                m_chessboard.showPromotionPopup = false;

                m_renderer.setPieceModel(m_chessboard.selectedPawn);
            }

            // Nouveau bouton de promotion aléatoire
            if (ImGui::Button("Promotion Aléatoire 🎲"))
            {
                std::string pieceChoisie = tiragePromotionExponentiel();

                if (pieceChoisie == "Cavalier")
                    m_chessboard.selectedPawn->setType(PieceType::Knight);
                else if (pieceChoisie == "Fou")
                    m_chessboard.selectedPawn->setType(PieceType::Bishop);
                else if (pieceChoisie == "Tour")
                    m_chessboard.selectedPawn->setType(PieceType::Rook);
                else if (pieceChoisie == "Reine")
                    m_chessboard.selectedPawn->setType(PieceType::Queen);

                m_chessboard.showPromotionPopup = false;

                m_renderer.setPieceModel(m_chessboard.selectedPawn);
            }

            ImGui::EndPopup();
        }
    }

    ImGui::End();
    m_renderer.update(&m_chessboard.m_pieces);
    m_renderer.render();
}

void App::CloseGame()
{
    m_renderer.close();
}