#pragma once
#include <utility>
#include <vector>
#include "../game/Square.hpp"

enum class PieceType {
    Pawn,
    Bishop,
    Rook,
    Knight,
    Queen,
    King
};

class Piece {
private:
    bool                m_team;
    int                 m_direction;
    bool                m_status;
    std::pair<int, int> m_coords;
    PieceType           m_type;

    // DISPLAY
    void displayMovement();

public:
    // ACTIONS
    void move(const std::pair<int, int>& newCoords);
    void take(Piece enemy);
    void promote();

    // GETTERS
    std::pair<int, int>              getCoords() const;
    bool                             getStatus();
    PieceType                        getType() const;
    std::vector<std::pair<int, int>> getZone(std::vector<std::vector<Square>>* chessboard) const;
    bool                             getTeam() const;
    std::vector<std::pair<int, int>> getRookMoves(std::vector<std::vector<Square>>* chessboard) const;
    std::vector<std::pair<int, int>> getPawnMoves(std::vector<std::vector<Square>>* chessboard) const;
    std::vector<std::pair<int, int>> getBishopMoves(std::vector<std::vector<Square>>* chessboard) const;
    std::vector<std::pair<int, int>> getQueenMoves(std::vector<std::vector<Square>>* chessboard) const;
    std::vector<std::pair<int, int>> getKnightMoves(std::vector<std::vector<Square>>* chessboard) const;
    std::vector<std::pair<int, int>> getKingMoves(std::vector<std::vector<Square>>* chessboard) const;

    // CONSTRUCTOR
    Piece() = default;
    Piece(bool team, std::pair<int, int> coords, PieceType type);

    // DESTRUCTOR
    ~Piece();
};