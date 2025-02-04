#pragma once
#include <iostream>
#include <vector>
#include "Square.hpp"
#include "../pieces/AllPieces.hpp"

class Chessboard {
    private:
        std::vector<std::vector<Square>> m_boardlist;

    public:
        AllPieces m_pieces;

    public:
        void InitializeBoardList();
        void CreateBoard();
};