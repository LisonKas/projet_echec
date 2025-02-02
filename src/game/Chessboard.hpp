#pragma once
#include <iostream>
#include <vector>
#include "Square.hpp"

class Chessboard {
    private:
        std::vector<std::vector<Square>> m_boardlist;

    public:
        //std::vector<Piece> m_pieces;

    public:
        void InitializeBoardList();
        void CreateBoard();
};