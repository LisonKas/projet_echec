#pragma once
#include "piece.hpp"
#include <vector>

class AllPieces {
    public :
        std::vector<Piece> m_white_pieces;
        std::vector<Piece> m_black_pieces;

    public :
        void InitializeAllPieces();
        void PiecesAppear();
};