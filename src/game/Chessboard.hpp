#pragma once
#include <iostream>
#include <vector>
#include "../pieces/AllPieces.hpp"
#include "Square.hpp"
#include "pieces/piece.hpp"

class Chessboard {
private:
    std::vector<std::vector<Square>> m_boardlist;
    std::pair<int, int>              m_selectedPiece{-1, -1}; // Coordonnées de la pièce sélectionnée
    std::vector<std::pair<int, int>> m_highlightedSquares;    // Cases à surligner

public:
    AllPieces m_pieces;

public:
    void        InitializeBoardList();
    void        HandlePieceMove(const std::pair<int, int>& clickedSquare);
    void        CreateBoard();
    void        CapturePiece(const std::pair<int, int>& target);
    void        MovePiece(const std::pair<int, int>& destination);
    void        SetSquareColor(int i, int j);
    void        ResetSelection();
    void        SelectPiece(const std::pair<int, int>& clickedSquare, Piece* selectedPiece);
    void        changeTurn();
    bool        teamPlaying() const;
    bool        m_isGameOver = false;
    std::string m_winnerMessage;
    bool        m_teamPlaying{true}; // true si c'est le tour des blancs, false pour les noirs
    void        PromotePawn(Piece* pawn);
    bool        showPromotionPopup = false; // Flag pour afficher la promotion du pion
    Piece*      selectedPawn       = nullptr;
};