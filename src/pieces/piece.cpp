#include "piece.hpp"

Piece::Piece(bool team, std::pair<int, int> coords, PieceType type) 
    : m_team(team), m_status(true), m_coords(coords), m_type(type) {}

Piece::~Piece() {}