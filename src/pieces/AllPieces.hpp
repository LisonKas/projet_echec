#pragma once
#include <iostream>
#include <vector>
#include "piece.hpp"

class AllPieces {
public:
    std::vector<Piece> m_white_pieces;
    std::vector<Piece> m_black_pieces;

public:
    void        InitializeAllPieces();
    std::string PiecesAppear(int x, int y);
    Piece*      GetPieceAt(int x, int y);
    // void verifPieceandCase(&boardlist) --> piece.getZone( &boarlist)
};