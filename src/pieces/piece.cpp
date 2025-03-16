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
    {
        int nextRow = m_coords.first + m_direction;
        int nextCol = m_coords.second;

        // Vérifier si la case en avant est libre (le pion peut avancer d'une case)
        if (nextRow >= 0 && nextRow < 8 && !(*chessboard)[nextRow][nextCol].isOccupied())
        {
            zone.push_back({nextRow, nextCol});

            // Vérifier si le pion est sur sa ligne de départ pour avancer de 2 cases
            int startRow = (m_team) ? 6 : 1;
            if (m_coords.first == startRow)
            {
                int twoStepRow = nextRow + m_direction;
                // Vérifier que la case à deux pas est aussi libre
                if (twoStepRow >= 0 && twoStepRow < 8 && !(*chessboard)[twoStepRow][nextCol].isOccupied())
                {
                    zone.push_back({twoStepRow, nextCol});
                }
            }
        }

        // Vérifier les prises en diagonale
        std::vector<std::pair<int, int>> captureMoves = {
            {nextRow, nextCol - 1}, {nextRow, nextCol + 1}
        };

        for (const auto& move : captureMoves)
        {
            int r = move.first, c = move.second;
            if (r >= 0 && r < 8 && c >= 0 && c < 8)
            {
                if ((*chessboard)[r][c].isOccupied())
                {
                    zone.push_back(move);
                }
            }
        }
    }
    break;

    default:
        // Gérer les autres types de pièces plus tard
        break;
    }
    return zone;
}

void Piece::move(const std::pair<int, int>& newCoords)
{
    m_coords = newCoords;
}