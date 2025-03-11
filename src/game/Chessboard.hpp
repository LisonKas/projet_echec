#pragma once
#include <iostream>
#include <vector>
#include "../pieces/AllPieces.hpp"
#include "Square.hpp"

class Chessboard {
private:
    std::vector<std::vector<Square>> m_boardlist;
    std::pair<int, int>              m_selectedPiece{-1, -1}; // Coordonnées de la pièce sélectionnée
    std::vector<std::pair<int, int>> m_highlightedSquares;    // Cases à surligner

public:
    AllPieces m_pieces;

public:
    void InitializeBoardList();
    void CreateBoard();
};