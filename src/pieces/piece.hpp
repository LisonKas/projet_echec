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

public:
    // CONSTRUCTORS / DESTRUCTOR
    Piece() = default;
    Piece(bool team, const std::pair<int, int>& coords, PieceType type);
    ~Piece();

    // GETTERS
    const std::pair<int, int>&       getCoords() const;
    bool                             getTeam() const;
    bool                             getStatus() const;
    PieceType                        getType() const;
    std::vector<std::pair<int, int>> getZone(const std::vector<std::vector<Square>>* board) const;

    // SETTERS
    void setStatus(bool status);
    void setType(PieceType newType);

    // ACTIONS
    void move(const std::pair<int, int>& newCoords);

private:
    // DETECTION DES ZONES
    std::vector<std::pair<int, int>> getPawnZone(const std::vector<std::vector<Square>>* board) const;
    std::vector<std::pair<int, int>> getRookZone(const std::vector<std::vector<Square>>* board) const;
    std::vector<std::pair<int, int>> getBishopZone(const std::vector<std::vector<Square>>* board) const;
    std::vector<std::pair<int, int>> getKnightZone() const;
    std::vector<std::pair<int, int>> getQueenZone(const std::vector<std::vector<Square>>* board) const;
    std::vector<std::pair<int, int>> getKingZone() const;
};
