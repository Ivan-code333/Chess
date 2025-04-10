// piece.cpp
#include "piece.h"

Piece::Piece() : type(PieceType::NONE), color(Color::NONE), hasMoved(false) {}

Piece::Piece(PieceType type, Color color) : type(type), color(color), hasMoved(false) {}

PieceType Piece::getType() const {
    return type;
}

Color Piece::getColor() const {
    return color;
}

bool Piece::getHasMoved() const {
    return hasMoved;
}

void Piece::setHasMoved(bool moved) {
    hasMoved = moved;
}

char Piece::getSymbol() const {
    switch (type) {
        case PieceType::PAWN: return color == Color::WHITE ? 'P' : 'p';
        case PieceType::KNIGHT: return color == Color::WHITE ? 'N' : 'n';
        case PieceType::BISHOP: return color == Color::WHITE ? 'B' : 'b';
        case PieceType::ROOK: return color == Color::WHITE ? 'R' : 'r';
        case PieceType::QUEEN: return color == Color::WHITE ? 'Q' : 'q';
        case PieceType::KING: return color == Color::WHITE ? 'K' : 'k';
        default: return ' ';
    }
}

std::string Piece::getName() const {
    switch (type) {
        case PieceType::PAWN: return "Pawn";
        case PieceType::KNIGHT: return "Knight";
        case PieceType::BISHOP: return "Bishop";
        case PieceType::ROOK: return "Rook";
        case PieceType::QUEEN: return "Queen";
        case PieceType::KING: return "King";
        default: return "";
    }
}

bool Piece::isEmpty() const {
    return type == PieceType::NONE;
}

bool Piece::isOpponent(const Piece& other) const {
    return !isEmpty() && !other.isEmpty() && color != other.color;
}