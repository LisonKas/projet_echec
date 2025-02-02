#include "piece.hpp"

Bishop::Bishop(bool team, std::pair<int, int> coords)
    : m_team(team), m_status(true), m_coords(coords)
{
}