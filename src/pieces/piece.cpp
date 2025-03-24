#include "piece.hpp"
#include <array>
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

// DETECTION DES ZONES

std::vector<std::pair<int, int>> Piece::getZone(std::vector<std::vector<Square>>* board) const
{
    switch (m_type)
    {
    case PieceType::Pawn:
        return getPawnMoves(board);
    case PieceType::Rook:
        return getRookMoves(board);
    case PieceType::Bishop:
        return getBishopMoves(board);
    case PieceType::Queen:
        return getQueenMoves(board);
    case PieceType::Knight:
        return getKnightMoves();
    case PieceType::King:
        return getKingMoves(board);
    default:
        return {};
    }
}

void Piece::move(const std::pair<int, int>& newCoords)
{
    m_coords = newCoords;
}

std::vector<std::pair<int, int>> Piece::getPawnMoves(std::vector<std::vector<Square>>* board) const
{
    std::vector<std::pair<int, int>> zone;
    int                              nextRow = m_coords.first + m_direction;
    int                              col     = m_coords.second;

    // Mouvements d'une deux cases vers l'avant
    if (nextRow >= 0 && nextRow < 8 && !(*board)[nextRow][col].isOccupied())
    {
        zone.push_back({nextRow, col});

        if (m_coords.first == (m_team ? 6 : 1)) // Détection du 1er tour (position de départ)
        {
            int twoStepRow = nextRow + m_direction;
            if (twoStepRow >= 0 && twoStepRow < 8 && !(*board)[twoStepRow][col].isOccupied())
            {
                zone.push_back({twoStepRow, col});
            }
        }
    }

    // Prises en diagonale
    for (int shift : {-1, 1})
    {
        int captureCol = col + shift;
        if (captureCol >= 0 && captureCol < 8 && (*board)[nextRow][captureCol].isOccupied())
        {
            zone.push_back({nextRow, captureCol});
        }
    }

    return zone;
}

std::vector<std::pair<int, int>> Piece::getRookMoves(std::vector<std::vector<Square>>* board) const
{
    std::vector<std::pair<int, int>>             zone;
    constexpr std::array<std::pair<int, int>, 4> directions = {{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}}; // haut, bas, gauche, droite

    int row = m_coords.first;
    int col = m_coords.second;

    for (const std::pair<int, int>& direction : directions)
    {
        int rShift = direction.first;
        int cShift = direction.second;

        for (int nextRow = row + rShift, nextCol = col + cShift; nextRow >= 0 && nextRow < 8 && nextCol >= 0 && nextCol < 8; nextRow += rShift, nextCol += cShift)
        {
            zone.push_back({nextRow, nextCol});
            if ((*board)[nextRow][nextCol].isOccupied())
                break;
        }
    }

    return zone;
}

std::vector<std::pair<int, int>> Piece::getBishopMoves(std::vector<std::vector<Square>>* board) const
{
    std::vector<std::pair<int, int>> zone;
    int                              row{0};
    int                              col{0};

    for (int rShift : {-1, 1})
    {
        for (int cShift : {-1, 1})
        {
            row = m_coords.first + rShift;
            col = m_coords.second + cShift;

            while (row >= 0 && row < 8 && col >= 0 && col < 8)
            {
                if (!(*board)[row][col].isOccupied())
                {
                    zone.push_back({row, col});
                }
                else
                {
                    zone.push_back({row, col});
                    break;
                }
                row += rShift;
                col += cShift;
            }
        }
    }
    return zone;
}

std::vector<std::pair<int, int>> Piece::getKnightMoves() const
{
    std::vector<std::pair<int, int>> zone;

    std::vector<std::pair<int, int>> directions = {
        {-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}
    };

    for (const auto& direction : directions)
    {
        int row = m_coords.first + direction.first;
        int col = m_coords.second + direction.second;

        // Vérifier si le mouvement reste dans les limites du plateau
        if (row >= 0 && row < 8 && col >= 0 && col < 8)
        {
            // Ajouter la case à la zone si elle est vide ou occupée par une pièce ennemie
            zone.push_back({row, col});
        }
    }

    return zone;
}

std::vector<std::pair<int, int>> Piece::getQueenMoves(std::vector<std::vector<Square>>* board) const
{
    std::vector<std::pair<int, int>> zone;

    // Ajouter les déplacements comme une tour
    auto rookMoves = getRookMoves(board);
    zone.insert(zone.end(), rookMoves.begin(), rookMoves.end());

    // Ajouter les déplacements comme un fou
    auto bishopMoves = getBishopMoves(board);
    zone.insert(zone.end(), bishopMoves.begin(), bishopMoves.end());

    return zone;
}

std::vector<std::pair<int, int>> Piece::getKingMoves(std::vector<std::vector<Square>>* chessboard) const
{
    std::vector<std::pair<int, int>> zone;
    int                              directions[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}}; // Toutes les directions du roi

    for (const auto& dir : directions)
    {
        int row = m_coords.first + dir[0];
        int col = m_coords.second + dir[1];

        if (row >= 0 && row < 8 && col >= 0 && col < 8) // Vérifie que la case est dans l'échiquier
        {
            zone.push_back({row, col});
        }
    }
    return zone;
}
