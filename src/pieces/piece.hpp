#pragma once
#include <utility>
#include <vector>

enum class PieceType {
    Pawn,
    Bishop,
    Rook,
    Knight,
    Queen,
    King
};

class Piece {
private:
    bool                m_team;
    int                 m_direction;
    bool                m_status;
    std::pair<int, int> m_coords;
    PieceType           m_type;

    // DISPLAY
    void displayMovement();

public:
    // ACTIONS
    void move(std::pair<int, int>);
    void take(Piece enemy);
    void promote();

    // GETTERS
    std::pair<int, int>              getCoords() const;
    bool                             getStatus();
    PieceType                        getType() const;
    std::vector<std::pair<int, int>> getZone() const;

    // CONSTRUCTOR
    Piece() = default;
    Piece(bool team, std::pair<int, int> coords, PieceType type);

    // DESTRUCTOR
    ~Piece();
};