//
/* Типы фигур, генерация ходов*/

//

#ifndef PIECE_H
#define PIECE_H

#pragma once
#include <memory>
#include <vector>


// Цвет фигуры
enum class Color { WHITE, BLACK };

// Тип фигуры
enum class PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, NONE };

// Позиция на доске (x, y) - от 0 до 7
struct Position {
    int x;
    int y;

    Position(int x = -1, int y = -1) : x(x), y(y) {}
    bool isValid() const { return x >= 0 && x < 8 && y >= 0 && y < 8; }

    // Добавьте оператор сравнения
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

// Предварительное объявление класса Board для использования в Piece
class Board;

// Базовый класс фигуры
class Piece {
protected:
    Color color;
    PieceType type;

public:
    Piece(Color color, PieceType type) : color(color), type(type) {}
    virtual ~Piece() = default;

    Color getColor() const { return color; }
    PieceType getType() const { return type; }

    // Получение всех возможных ходов (без учета шаха)
    virtual std::vector<Position> getPossibleMoves(const Position& from, const Board& board) const = 0;

    // Символ фигуры для отладки (ASCII)
    virtual char getSymbol() const = 0;
};

// Конкретные классы фигур
class King : public Piece {
public:
    King(Color color) : Piece(color, PieceType::KING) {}
    std::vector<Position> getPossibleMoves(const Position& from, const Board& board) const override;
    char getSymbol() const override { return (color == Color::WHITE) ? 'K' : 'k'; }
};

class Queen : public Piece {
public:
    Queen(Color color) : Piece(color, PieceType::QUEEN) {}
    std::vector<Position> getPossibleMoves(const Position& from, const Board& board) const override;
    char getSymbol() const override { return (color == Color::WHITE) ? 'Q' : 'q'; }
};

class Rook : public Piece {
public:
    Rook(Color color) : Piece(color, PieceType::ROOK) {}
    std::vector<Position> getPossibleMoves(const Position& from, const Board& board) const override;
    char getSymbol() const override { return (color == Color::WHITE) ? 'R' : 'r'; }
};

class Bishop : public Piece {
public:
    Bishop(Color color) : Piece(color, PieceType::BISHOP) {}
    std::vector<Position> getPossibleMoves(const Position& from, const Board& board) const override;
    char getSymbol() const override { return (color == Color::WHITE) ? 'B' : 'b'; }
};

class Knight : public Piece {
public:
    Knight(Color color) : Piece(color, PieceType::KNIGHT) {}
    std::vector<Position> getPossibleMoves(const Position& from, const Board& board) const override;
    char getSymbol() const override { return (color == Color::WHITE) ? 'N' : 'n'; }
};

class Pawn : public Piece {
public:
    Pawn(Color color) : Piece(color, PieceType::PAWN) {}
    std::vector<Position> getPossibleMoves(const Position& from, const Board& board) const override;
    char getSymbol() const override { return (color == Color::WHITE) ? 'P' : 'p'; }
};

#endif //PIECE_H
