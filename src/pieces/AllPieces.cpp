#include "AllPieces.hpp"
#include <iostream>

void AllPieces::InitializeAllPieces(){
    for(int j {0}; j<8; j++){
        this->m_black_pieces.push_back(Piece(false, {1, j+1}, PieceType::Pawn));
    }
    for(int i {0}; i<2; i++){
        this->m_black_pieces.push_back(Piece(false, {0, i*7}, PieceType::Rook));
        int pos_knight = i==0 ? 1 : 6, pos_bishop = i==0 ? 2 : 5;
        this->m_black_pieces.push_back(Piece(false, {0, pos_knight}, PieceType::Knight));
        this->m_black_pieces.push_back(Piece(false, {0, pos_bishop}, PieceType::Bishop));
    }
    this->m_black_pieces.push_back(Piece(false, {0, 3}, PieceType::Queen));
    this->m_black_pieces.push_back(Piece(false, {0, 4}, PieceType::King));


    for(int j {0}; j<8; j++){
        this->m_white_pieces.push_back(Piece(true, {6, j+1}, PieceType::Pawn));
    }
    for(int i {0}; i<2; i++){
        this->m_white_pieces.push_back(Piece(true, {7, i*7}, PieceType::Rook));
        int pos_knight = i==0 ? 1 : 6, pos_bishop = i==0 ? 2 : 5;
        this->m_white_pieces.push_back(Piece(true, {7, pos_knight}, PieceType::Knight));
        this->m_white_pieces.push_back(Piece(true, {7, pos_bishop}, PieceType::Bishop));
    }
    this->m_white_pieces.push_back(Piece(true, {7, 3}, PieceType::Queen));
    this->m_white_pieces.push_back(Piece(true, {7, 4}, PieceType::King));

    std::cout << "All pieces are created." << std::endl;
}