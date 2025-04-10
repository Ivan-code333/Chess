#ifndef PIECE_H
#define PIECE_H

#include <string>

enum class Color {
    White,
    Black
};

class Piece {
public:
    Piece(Color color) : color(color) {}
    virtual ~Piece() {}

    Color getColor() const { return color; }
    virtual char getSymbol() const = 0;

    // В будущем сюда добавим проверку легальности хода
    virtual bool isMoveLegal(int fromRow, int fromCol, int toRow, int toCol) const = 0;

protected:
    Color color;
};

// Классы конкретных фигур
class Pawn : public Piece {
public:
    Pawn(Color color);
    char getSymbol() const override;
    bool isMoveLegal(int fromRow, int fromCol, int toRow, int toCol) const override;
};

class Knight : public Piece {
public:
    Knight(Color color);
    char getSymbol() const override;
    bool isMoveLegal(int fromRow, int fromCol, int toRow, int toCol) const override;
};

class Bishop : public Piece {
public:
    Bishop(Color color);
    char getSymbol() const override;
    bool isMoveLegal(int fromRow, int fromCol, int toRow, int toCol) const override;
};

class Rook : public Piece {
public:
    Rook(Color color);
    char getSymbol() const override;
    bool isMoveLegal(int fromRow, int fromCol, int toRow, int toCol) const override;
};

class Queen : public Piece {
public:
    Queen(Color color);
    char getSymbol() const override;
    bool isMoveLegal(int fromRow, int fromCol, int toRow, int toCol) const override;
};

class King : public Piece {
public:
    King(Color color);
    char getSymbol() const override;
    bool isMoveLegal(int fromRow, int fromCol, int toRow, int toCol) const override;
};

#endif // PIECE_H
