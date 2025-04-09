#include "piece.hpp"
#include <array>

// CONSTRUCTOR

Piece::Piece(bool team, std::pair<int, int> coords, PieceType type)
    : m_team(team), m_direction(1 - 2 * m_team), m_status(true), m_coords(coords), m_type(type) {}

// DESTRUCTOR

Piece::~Piece() {}

// GETTERS

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

bool Piece::getStatus()
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

// DETECTION DES ZONES

std::vector<std::pair<int, int>> Piece::getZone(std::vector<std::vector<Square>>* board) const
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

std::vector<std::pair<int, int>> Piece::getPawnZone(std::vector<std::vector<Square>>* board) const
{
    std::vector<std::pair<int, int>> zone;
    int                              nextRow = m_coords.first + m_direction;
    int                              col     = m_coords.second;

    if (nextRow >= 0 && nextRow < 8 && !(*board)[nextRow][col].isOccupied())
    {
        zone.push_back({nextRow, col});

        if (m_coords.first == (m_team ? 6 : 1))
        {
            int twoStepRow = nextRow + m_direction;
            if (twoStepRow >= 0 && twoStepRow < 8 && !(*board)[twoStepRow][col].isOccupied())
            {
                zone.push_back({twoStepRow, col});
            }
        }
    }

    int shift;
    for (int i = 0; i < 2; ++i)
    {
        shift          = (i == 0) ? -1 : 1;
        int captureCol = col + shift;
        if (captureCol >= 0 && captureCol < 8 && (*board)[nextRow][captureCol].isOccupied())
        {
            zone.push_back({nextRow, captureCol});
        }
    }

    return zone;
}

std::vector<std::pair<int, int>> Piece::getRookZone(std::vector<std::vector<Square>>* board) const
{
    std::vector<std::pair<int, int>>             zone;
    constexpr std::array<std::pair<int, int>, 4> directions = {{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};

    int row = m_coords.first;
    int col = m_coords.second;

    std::pair<int, int> direction;
    for (int i = 0; i < directions.size(); ++i)
    {
        direction  = directions[i];
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

std::vector<std::pair<int, int>> Piece::getBishopZone(std::vector<std::vector<Square>>* board) const
{
    std::vector<std::pair<int, int>> zone;
    int                              row, col;

    int rShift, cShift;
    for (int i = 0; i < 2; ++i)
    {
        rShift = (i == 0) ? -1 : 1;
        for (int j = 0; j < 2; ++j)
        {
            cShift = (j == 0) ? -1 : 1;
            row    = m_coords.first + rShift;
            col    = m_coords.second + cShift;

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

    std::vector<std::pair<int, int>> directions = {
        {-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}
    };

    std::pair<int, int> direction;
    for (int i = 0; i < directions.size(); ++i)
    {
        direction = directions[i];
        int row   = m_coords.first + direction.first;
        int col   = m_coords.second + direction.second;

        if (row >= 0 && row < 8 && col >= 0 && col < 8)
        {
            zone.push_back({row, col});
        }
    }

    return zone;
}

std::vector<std::pair<int, int>> Piece::getQueenZone(std::vector<std::vector<Square>>* board) const
{
    std::vector<std::pair<int, int>> zone;

    auto rookZone = getRookZone(board);
    zone.insert(zone.end(), rookZone.begin(), rookZone.end());

    auto bishopZone = getBishopZone(board);
    zone.insert(zone.end(), bishopZone.begin(), bishopZone.end());

    return zone;
}

std::vector<std::pair<int, int>> Piece::getKingZone() const
{
    std::vector<std::pair<int, int>>             zone;
    constexpr std::array<std::pair<int, int>, 8> directions = {{{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}}};

    std::pair<int, int> direction;
    for (int i = 0; i < directions.size(); ++i)
    {
        direction = directions[i];
        int row   = m_coords.first + direction.first;
        int col   = m_coords.second + direction.second;

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
