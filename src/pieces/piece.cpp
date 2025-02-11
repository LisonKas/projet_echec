#include "piece.hpp"
#include <algorithm>
#include "../game/Chessboard.hpp"

Piece::Piece(bool team, std::pair<int, int> coords, PieceType type)
    : m_team(team), m_status(true), m_coords(coords), m_type(type) {}

Piece::~Piece() {}

std::pair<int, int> Piece::getCoords() const
{
    return m_coords;
}

PieceType Piece::getType() const
{
    return m_type;
}

std::vector<std::pair<int, int>> Piece::getZone() const
{
    std::vector<std::pair<int, int>> zone;
    switch (m_type)
    {
    case PieceType::Pawn:
        zone.push_back({m_coords.first, m_coords.second + 1});
    }

    return zone;
};