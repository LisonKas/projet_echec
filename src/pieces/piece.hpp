#pragma once
#include <utility>

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
    bool                m_status;
    std::pair<int, int> m_coords;
    PieceType m_type;

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
    Piece() = default;
    Piece(bool team, std::pair<int, int> coords, PieceType type);

    // DESTRUCTOR
    ~Piece();
};

// class Bishop : public Piece {
//     Bishop();
//     Bishop(bool team, std::pair<int, int> coords)
//     : Piece(team, coords)
//     {
//     }
//     ~Bishop();
// };

// class King : public Piece {
//     King();
//     King(bool team, std::pair<int, int> coords)
//     : Piece(team, coords)
//     {
//     }
//     ~King();
// };

// class Knight : public Piece {
//     Knight();
//     Knight(bool team, std::pair<int, int> coords)
//     : Piece(team, coords)
//     {
//     }
//     ~Knight();
// };

// class Pawn : public Piece {
//     public:
//         Pawn();
//         Pawn(bool team, std::pair<int, int> coords)
//         : Piece(team, coords)
//         {
//         }
//         ~Pawn();
// };

// class Queen : public Piece {
//     Queen();
//     Queen(bool team, std::pair<int, int> coords)
//     : Piece(team, coords)
//     {
//     }
//     ~Queen();
// };

// class Rook : public Piece {
//     Rook();
//     Rook(bool team, std::pair<int, int> coords)
//     : Piece(team, coords)
//     {
//     }
//     ~Rook();
// };
