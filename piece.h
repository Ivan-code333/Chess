//
/* Типы фигур, генерация ходов*/

//

#ifndef PIECE_H
#define PIECE_H
// piece.h
#pragma once
#include <string>
#include <vector>

enum class PieceType {
    NONE,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

enum class Color {
    WHITE,
    BLACK,
    NONE
};

// Позиция на доске
struct Position {
    int x; // Столбец (0-7)
    int y; // Строка (0-7)

    Position() : x(-1), y(-1) {}
    Position(int x, int y) : x(x), y(y) {}

    bool isValid() const {
        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    std::string toNotation() const {
        if (!isValid()) return "";
        return std::string(1, 'a' + x) + std::string(1, '8' - y);
    }

    static Position fromNotation(const std::string& notation) {
        if (notation.length() != 2) return Position();
        int x = notation[0] - 'a';
        int y = '8' - notation[1];
        return Position(x, y);
    }
};

class Piece {
private:
    PieceType type;
    Color color;
    bool hasMoved;

public:
    Piece();
    Piece(PieceType type, Color color);

    PieceType getType() const;
    Color getColor() const;
    bool getHasMoved() const;
    void setHasMoved(bool moved);

    char getSymbol() const;
    std::string getName() const;

    bool isEmpty() const;
    bool isOpponent(const Piece& other) const;
};
#endif //PIECE_H
