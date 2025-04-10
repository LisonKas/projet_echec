#pragma once
#include <algorithm>
#include <iostream>
#include <vector>
#include "../pieces/AllPieces.hpp"
#include "Square.hpp"
#include "pieces/piece.hpp"

class Chessboard {
private:
    std::vector<std::vector<Square>> m_boardlist;
    std::pair<int, int>              m_selectedPiece{-1, -1};
    std::vector<std::pair<int, int>> m_highlightedSquares;

public:
    AllPieces   m_pieces;
    bool        m_isGameOver = false;
    std::string m_winnerMessage;
    bool        m_teamPlaying{true}; // 0 = noir, 1 = blanc
    bool        showPromotionPopup = false;
    Piece*      selectedPawn       = nullptr;

    // Création du plateau
    void Reinitialize();
    void InitializeBoardList();
    void CreateBoard();

    // Gestion des pièces sur le plateau
    void HandlePieceMove(const std::pair<int, int>& clickedSquare);
    void CapturePiece(const std::pair<int, int>& target);
    void MovePiece(const std::pair<int, int>& destination);
    void SelectPiece(const std::pair<int, int>& clickedSquare, Piece* selectedPiece);
    void ResetSelection();

    // Autres
    void SetSquareColor(int i, int j);
    void changeTurn();
    bool GetTeamPlaying() const;
    void PromotePawn(Piece* pawn);
    void PlayCaptureSound();
};
