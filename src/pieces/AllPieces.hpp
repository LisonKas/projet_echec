#pragma once
#include <iostream>
#include <vector>
#include "glad/glad.h"
#include "piece.hpp"

class AllPieces {
public:
    std::vector<Piece> m_white_pieces;
    std::vector<Piece> m_black_pieces;

public:
    void   InitializeAllPieces();
    GLuint PiecesAppear(int x, int y); // GLuint ou //std::string
    Piece* GetPieceAt(std::pair<int, int> coords);
    void   RemovePieceAt(std::pair<int, int> coords);
    // void verifPieceandCase(&boardlist) --> piece.getZone( &boarlist)
};