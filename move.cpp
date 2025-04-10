//
// Created by Fores on 10.04.2025.
//

#include "move.h"
// move.cpp


Move::Move() : from(), to(), promotionType(PieceType::NONE), moveType(MoveType::INVALID) {}

Move::Move(Position from, Position to, MoveType moveType, PieceType promotionType)
    : from(from), to(to), moveType(moveType), promotionType(promotionType) {}

Position Move::getFrom() const {
    return from;
}

Position Move::getTo() const {
    return to;
}

MoveType Move::getMoveType() const {
    return moveType;
}

PieceType Move::getPromotionType() const {
    return promotionType;
}

void Move::setMoveType(MoveType type) {
    moveType = type;
}

void Move::setPromotionType(PieceType type) {
    promotionType = type;
}

std::string Move::toNotation() const {
    if (!isValid()) return "";

    std::string notation = from.toNotation() + to.toNotation();

    // Добавляем обозначение фигуры при повышении пешки
    if (moveType == MoveType::PROMOTION) {
        switch (promotionType) {
            case PieceType::QUEEN: notation += "q"; break;
            case PieceType::ROOK: notation += "r"; break;
            case PieceType::BISHOP: notation += "b"; break;
            case PieceType::KNIGHT: notation += "n"; break;
            default: break;
        }
    }

    return notation;
}

bool Move::isValid() const {
    return from.isValid() && to.isValid();
}

bool Move::operator==(const Move& other) const {
    return from == other.from && to == other.to && moveType == other.moveType &&
           promotionType == other.promotionType;
}