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
bool Piece::getTeam() const
{
    return m_team;
}

std::vector<std::pair<int, int>> Piece::getZone(std::vector<std::vector<Square>>* chessboard) const
{
    switch (m_type)
    {
    case PieceType::Pawn:
        return getPawnMoves(chessboard);
    case PieceType::Rook:
        return getRookMoves(chessboard);
    // Ajoute ici les autres types de pièces (Bishop, Knight, Queen, King)
    default:
        return {};
    }
}

void Piece::move(const std::pair<int, int>& newCoords)
{
    m_coords = newCoords;
}

// 🎯 Fonction spécifique pour le pion
std::vector<std::pair<int, int>> Piece::getPawnMoves(std::vector<std::vector<Square>>* chessboard) const
{
    std::vector<std::pair<int, int>> zone;
    int                              nextRow = m_coords.first + m_direction;
    int                              nextCol = m_coords.second;

    // Avance d'une case si libre
    if (nextRow >= 0 && nextRow < 8 && !(*chessboard)[nextRow][nextCol].isOccupied())
    {
        zone.push_back({nextRow, nextCol});

        // Avance de deux cases si au départ
        int startRow   = (m_team) ? 6 : 1;
        int twoStepRow = nextRow + m_direction;
        if (m_coords.first == startRow && twoStepRow >= 0 && twoStepRow < 8 && !(*chessboard)[twoStepRow][nextCol].isOccupied())
        {
            zone.push_back({twoStepRow, nextCol});
        }
    }

    // Prises en diagonale
    std::vector<std::pair<int, int>> captureMoves = {{nextRow, nextCol - 1}, {nextRow, nextCol + 1}};
    for (const auto& move : captureMoves)
    {
        int r = move.first, c = move.second;
        if (r >= 0 && r < 8 && c >= 0 && c < 8 && (*chessboard)[r][c].isOccupied())
        {
            zone.push_back(move);
        }
    }
    return zone;
}

// 🎯 Fonction spécifique pour la tour
std::vector<std::pair<int, int>> Piece::getRookMoves(std::vector<std::vector<Square>>* chessboard) const
{
    std::vector<std::pair<int, int>> zone;
    int                              directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // haut, bas, gauche, droite

    for (const auto& dir : directions)
    {
        int row = m_coords.first;
        int col = m_coords.second;

        while (true)
        {
            row += dir[0];
            col += dir[1];

            if (row < 0 || row >= 8 || col < 0 || col >= 8)
                break;

            if (!(*chessboard)[row][col].isOccupied())
            {
                zone.push_back({row, col});
            }
            else
            {
                zone.push_back({row, col});
                break;
            }
        }
    }
    return zone;
}