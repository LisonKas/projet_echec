#include "piece.hpp"
#include <array>

// CONSTRUCTORS
Piece::Piece(bool team, const std::pair<int, int>& coords, PieceType type)
    : m_team(team), m_direction(1 - 2 * m_team), m_status(true), m_coords(coords), m_type(type) {}

// DESTRUCTOR
Piece::~Piece() {}

// GETTERS
const std::pair<int, int>& Piece::getCoords() const
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

bool Piece::getStatus() const
{
    return m_status;
}

// SETTERS
void Piece::setStatus(bool status)
{
    m_status = status;
}

void Piece::setType(PieceType newType)
{
    m_type = newType;
}

// DÉTECTION DES ZONES

std::vector<std::pair<int, int>> Piece::getZone(const std::vector<std::vector<Square>>* board) const
{
    switch (m_type)
    {
    case PieceType::Pawn:
        return getPawnZone(board);
    case PieceType::Rook:
        return getRookZone(board);
    case PieceType::Bishop:
        return getBishopZone(board);
    case PieceType::Queen:
        return getQueenZone(board);
    case PieceType::Knight:
        return getKnightZone();
    case PieceType::King:
        return getKingZone();
    default:
        return {};
    }
}

std::vector<std::pair<int, int>> Piece::getPawnZone(const std::vector<std::vector<Square>>* board) const
{
    std::vector<std::pair<int, int>> zone;
    int                              nextRow = m_coords.first + m_direction;
    int                              col     = m_coords.second;

    // Déplacements en avant
    if (nextRow >= 0 && nextRow < 8 && !(*board)[nextRow][col].isOccupied())
    {
        zone.push_back({nextRow, col});

        // 1er mouvement
        if (m_coords.first == (m_team ? 6 : 1))
        {
            int twoStepRow = nextRow + m_direction;
            if (twoStepRow >= 0 && twoStepRow < 8 && !(*board)[twoStepRow][col].isOccupied())
            {
                zone.push_back({twoStepRow, col});
            }
        }
    }

    // Captures en diagonal
    for (int i = 0; i < 2; ++i)
    {
        int shift      = (i == 0) ? -1 : 1;
        int captureCol = col + shift;
        if (captureCol >= 0 && captureCol < 8 && (*board)[nextRow][captureCol].isOccupied())
        {
            zone.push_back({nextRow, captureCol});
        }
    }

    return zone;
}

std::vector<std::pair<int, int>> Piece::getRookZone(const std::vector<std::vector<Square>>* board) const
{
    std::vector<std::pair<int, int>> zone;
    // Respectivement haut, bas, gauche, droite
    constexpr std::array<std::pair<int, int>, 4> directions = {{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};

    int row = m_coords.first;
    int col = m_coords.second;

    for (const auto& direction : directions)
    {
        int rShift = direction.first;
        int cShift = direction.second;

        int nextRow = row + rShift;
        int nextCol = col + cShift;

        while (nextRow >= 0 && nextRow < 8 && nextCol >= 0 && nextCol < 8)
        {
            zone.push_back({nextRow, nextCol});
            if ((*board)[nextRow][nextCol].isOccupied())
                break;

            nextRow += rShift;
            nextCol += cShift;
        }
    }

    return zone;
}

std::vector<std::pair<int, int>> Piece::getBishopZone(const std::vector<std::vector<Square>>* board) const
{
    std::vector<std::pair<int, int>> zone;

    // Diagonales
    for (int rShift : {-1, 1})
    {
        for (int cShift : {-1, 1})
        {
            int row = m_coords.first + rShift;
            int col = m_coords.second + cShift;

            while (row >= 0 && row < 8 && col >= 0 && col < 8)
            {
                zone.push_back({row, col});
                if ((*board)[row][col].isOccupied())
                    break;

                row += rShift;
                col += cShift;
            }
        }
    }

    return zone;
}

std::vector<std::pair<int, int>> Piece::getKnightZone() const
{
    std::vector<std::pair<int, int>> zone;
    // Déplacements en L possibles
    const std::vector<std::pair<int, int>> directions = {
        {-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}
    };

    for (const auto& dir : directions)
    {
        int row = m_coords.first + dir.first;
        int col = m_coords.second + dir.second;

        if (row >= 0 && row < 8 && col >= 0 && col < 8)
        {
            zone.push_back({row, col});
        }
    }

    return zone;
}

std::vector<std::pair<int, int>> Piece::getQueenZone(const std::vector<std::vector<Square>>* board) const
{
    std::vector<std::pair<int, int>> zone       = getRookZone(board);
    std::vector<std::pair<int, int>> bishopZone = getBishopZone(board);
    zone.insert(zone.end(), bishopZone.begin(), bishopZone.end());
    return zone;
}

std::vector<std::pair<int, int>> Piece::getKingZone() const
{
    std::vector<std::pair<int, int>>             zone;
    constexpr std::array<std::pair<int, int>, 8> directions = {
        {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}}
    };

    for (const auto& dir : directions)
    {
        int row = m_coords.first + dir.first;
        int col = m_coords.second + dir.second;

        if (row >= 0 && row < 8 && col >= 0 && col < 8)
        {
            zone.push_back({row, col});
        }
    }

    return zone;
}

void Piece::move(const std::pair<int, int>& newCoords)
{
    m_coords = newCoords;
}
