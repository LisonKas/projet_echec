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

    // INTERNAL
    void                             displayMovement();
    std::vector<std::pair<int, int>> getPawnZone(std::vector<std::vector<Square>>* board) const;
    std::vector<std::pair<int, int>> getRookZone(std::vector<std::vector<Square>>* board) const;
    std::vector<std::pair<int, int>> getBishopZone(std::vector<std::vector<Square>>* board) const;
    std::vector<std::pair<int, int>> getQueenZone(std::vector<std::vector<Square>>* board) const;
    std::vector<std::pair<int, int>> getKnightZone() const;
    std::vector<std::pair<int, int>> getKingZone() const;

public:
    // CONSTRUCTORS / DESTRUCTOR
    Piece() = default;
    Piece(bool team, std::pair<int, int> coords, PieceType type);
    ~Piece();

    // GETTERS
    std::pair<int, int>              getCoords() const;
    bool                             getTeam() const;
    bool                             getStatus();
    PieceType                        getType() const;
    std::vector<std::pair<int, int>> getZone(std::vector<std::vector<Square>>* board) const;

    // SETTERS
    void setStatus(bool status);
    void setType(PieceType newType);

    // ACTIONS
    void move(const std::pair<int, int>& newCoords);
    void take(Piece enemy);
    void promote();
};
