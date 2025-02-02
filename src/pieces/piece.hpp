#pragma once
#include <utility>

class Piece {
private:
    bool                m_team;
    bool                m_status;
    std::pair<int, int> m_coords;

    // DISPLAY
    void displayMovement();

public:
    // ACTIONS
    void move(int x, int y);
    void take(Piece enemy);
    void promote();

    // GETTERS
    std::pair<int, int> getCoords();
    bool                getStatus();

    // CONSTRUCTOR
    Piece();

    // DESTRUCTOR
    ~Piece();
};

class Bishop : public Piece {
    Bishop();
    Bishop(bool team, std::pair<int, int> coords);
    ~Bishop();
};

class King : public Piece {
    King();
    ~King();
};

class Knight : public Piece {
    Knight();
    ~Knight();
};

class Pawn : public Piece {
    Pawn();
    ~Pawn();
};

class Queen : public Piece {
    Queen();
    ~Queen();
};

class Rook : public Piece {
    Rook();
    ~Rook();
};
