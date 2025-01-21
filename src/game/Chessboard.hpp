#pragma once
#include <iostream>
#include <vector>
#include "Square.hpp"

class Chessboard {
    private:
        std::vector<std::vector<Square>> m_boardlist;

    public:
        //std::vector<Piece> m_WhitePieces;
        //std::vector<Piece> m_BlackPieces;

    public:
        void InitializeBoardList();
        void CreateBoard();
};