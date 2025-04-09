#include "Square.hpp"

bool Square::isOccupied() const
{
    return m_is_occupied;
}

bool Square::isOccupied(const std::pair<int, int>& coords) const
{
    return m_is_occupied;
}
