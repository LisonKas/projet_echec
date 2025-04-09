#pragma once
#include "../3D/Renderer3D.hpp"
#include "Chessboard.hpp"


class App {
private:
    Renderer3D m_renderer;
    Chessboard m_chessboard;

    // Nouvelles fonctions privées
    void handleNameInput();
    void whoPlaysFirst();
    void handleGameOver();
    void resetGame();
    void displayPromotion();
    void promotionButton(const char* label, PieceType type);
    void promoteTo(const std::string& pieceChoisie);

public:
    void InitializeGame();
    void StartGame();
    void Update();
    void CloseGame();
};
