#include "AllPieces.hpp"
#include <iostream>
#include "piece.hpp"
#include "../lois/Uniform_Discreet_Law.hpp"
#include "quick_imgui/quick_imgui.hpp"

void AllPieces::InitializeAllPieces()
{
    for (int j{0}; j < 8; j++)
    {
        this->m_black_pieces.push_back(Piece(false, {1, j}, PieceType::Pawn));
    }
    for (int i{0}; i < 2; i++)
    {
        this->m_black_pieces.push_back(Piece(false, {0, i * 7}, PieceType::Rook));
        int pos_knight = i == 0 ? 1 : 6, pos_bishop = i == 0 ? 2 : 5;
        this->m_black_pieces.push_back(Piece(false, {0, pos_knight}, PieceType::Knight));
        this->m_black_pieces.push_back(Piece(false, {0, pos_bishop}, PieceType::Bishop));
    }
    this->m_black_pieces.push_back(Piece(false, {0, 4}, PieceType::Queen));
    this->m_black_pieces.push_back(Piece(false, {0, 3}, PieceType::King));

    for (int j{0}; j < 8; j++)
    {
        this->m_white_pieces.push_back(Piece(true, {6, j}, PieceType::Pawn));
    }
    for (int i{0}; i < 2; i++)
    {
        this->m_white_pieces.push_back(Piece(true, {7, i * 7}, PieceType::Rook));
        int pos_knight = i == 0 ? 1 : 6, pos_bishop = i == 0 ? 2 : 5;
        this->m_white_pieces.push_back(Piece(true, {7, pos_knight}, PieceType::Knight));
        this->m_white_pieces.push_back(Piece(true, {7, pos_bishop}, PieceType::Bishop));
    }
    this->m_white_pieces.push_back(Piece(true, {7, 4}, PieceType::Queen));
    this->m_white_pieces.push_back(Piece(true, {7, 3}, PieceType::King));
}

std::string AllPieces::PiecesAppear(int x, int y)
{
    for (const auto& piece : m_black_pieces)
    {
        if (piece.getCoords() == std::make_pair(x, y))
        {
            switch (piece.getType())
            {
            case PieceType::Pawn: return "B_Pawn";
            case PieceType::Rook: return "B_Rook";
            case PieceType::Knight: return "B_Knight";
            case PieceType::Bishop: return "B_Bishop";
            case PieceType::Queen: return "B_Queen";
            case PieceType::King: return "B_King";
            default:
                return "";
                break;
            }
        }
    }

    for (const auto& piece : m_white_pieces)
    {
        if (piece.getCoords() == std::make_pair(x, y))
        {
            switch (piece.getType())
            {
            case PieceType::Pawn: return "W_Pawn";
            case PieceType::Rook: return "W_Rook";
            case PieceType::Knight: return "W_Knight";
            case PieceType::Bishop: return "W_Bishop";
            case PieceType::Queen: return "W_Queen";
            case PieceType::King: return "W_King";
            default:
                return "";
                break;
            }
        }
    }
    return "";
}

Piece* AllPieces::GetPieceAt(std::pair<int, int> coords)
{
    for (auto& piece : m_black_pieces)
    {
        if (piece.getCoords() == coords)
        {
            return &piece;
        }
    }
    for (auto& piece : m_white_pieces)
    {
        if (piece.getCoords() == coords)
        {
            return &piece;
        }
    }
    return nullptr;
}
