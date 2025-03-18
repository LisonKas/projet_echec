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
        return getKnightMoves(board);
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

std::vector<std::pair<int, int>> Piece::getBishopMoves(std::vector<std::vector<Square>>* chessboard) const
{
    std::vector<std::pair<int, int>> zone;

    // Directions diagonales du fou
    int directions[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    for (const auto& dir : directions)
    {
        int row = m_coords.first;
        int col = m_coords.second;

        while (true)
        {
            row += dir[0];
            col += dir[1];

            // Vérifier si la case est hors du plateau
            if (row < 0 || row >= 8 || col < 0 || col >= 8)
                break;

            // Si la case est vide, on peut y aller
            if (!(*chessboard)[row][col].isOccupied())
            {
                zone.push_back({row, col});
            }
            // Si la case est occupée par une pièce ennemie, on peut prendre la pièce
            else
            {
                zone.push_back({row, col});
                break; // Le fou s'arrête après avoir pris une pièce
            }
        }
    }
    return zone;
}

std::vector<std::pair<int, int>> Piece::getKnightMoves(std::vector<std::vector<Square>>* chessboard) const
{
    std::vector<std::pair<int, int>> zone;

    // Les 8 déplacements possibles du chevalier
    int directions[8][2] = {
        {-2, -1}, {-2, 1}, {2, -1}, {2, 1}, // Haut-Gauche, Haut-Droit, Bas-Gauche, Bas-Droit
        {-1, -2},
        {-1, 2},
        {1, -2},
        {1, 2} // Gauche-Haut, Droite-Haut, Gauche-Bas, Droite-Bas
    };

    for (const auto& dir : directions)
    {
        int row = m_coords.first + dir[0];
        int col = m_coords.second + dir[1];

        // Vérifier si le mouvement reste dans les limites du plateau
        if (row >= 0 && row < 8 && col >= 0 && col < 8)
        {
            // Si la case est vide ou occupée par une pièce ennemie, on peut y aller
            if (!(*chessboard)[row][col].isOccupied())
            {
                zone.push_back({row, col});
            }
            // Si la case est occupée par une pièce ennemie, on peut prendre la pièce
            else
            {
                zone.push_back({row, col});
            }
        }
    }

    return zone;
}

std::vector<std::pair<int, int>> Piece::getQueenMoves(std::vector<std::vector<Square>>* chessboard) const
{
    std::vector<std::pair<int, int>> zone;

    // Directions de la tour (haut, bas, gauche, droite)
    int rookDirections[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    // Directions du fou (diagonales)
    int bishopDirections[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    // Déplacement comme une tour
    for (const auto& dir : rookDirections)
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

    // Déplacement comme un fou
    for (const auto& dir : bishopDirections)
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
