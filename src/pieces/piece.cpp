#include "piece.hpp"
#include <iostream>

Piece::Piece(bool team, std::pair<int, int> coords, PieceType type)
    : m_team(team), m_direction(1 - 2 * m_team), m_status(true), m_coords(coords), m_type(type) {}

Piece::~Piece() {}

std::pair<int, int> Piece::getCoords() const
{
    return m_coords;
}

PieceType Piece::getType() const
{
    return m_type;
}

std::vector<std::pair<int, int>> Piece::getZone(std::vector<std::vector<Square>>* chessboard) const
{
    std::vector<std::pair<int, int>> zone;
    switch (m_type)
    {
    case PieceType::Pawn:
        // First round
        if (m_coords.first == 1 + 5 * m_team)
        {
            zone.push_back({m_coords.first + m_direction, m_coords.second});
            zone.push_back({m_coords.first + 2 * m_direction, m_coords.second});
        }
        else
        {
            zone.push_back({m_coords.first + m_direction, m_coords.second - 1});
            zone.push_back({m_coords.first + m_direction, m_coords.second + 1});
        }
    }

    for (const std::pair<int, int>& e : zone) // Utilisation correcte du type
    {
        std::cout << e.first << " " << e.second << " "; // Ajout d'un espace pour la lisibilité
    }
    std::cout << std::endl; // Pour bien séparer chaque affichage

    return zone;
}

void Piece::move(const std::pair<int, int>& newCoords)
{
    m_coords = newCoords;
}