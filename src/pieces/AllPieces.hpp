#pragma once
#include <vector>
#include "glad/glad.h"
#include "piece.hpp"

class AllPieces {
public:
    std::vector<Piece> m_white_pieces;
    std::vector<Piece> m_black_pieces;

    void   InitializeAllPieces();
    GLuint PiecesAppear(int x, int y);
    Piece* GetPieceAt(std::pair<int, int> coords);
    void   RemovePieceAt(std::pair<int, int> coords);
};