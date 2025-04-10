//
// Created by Fores on 10.04.2025.
//

#ifndef MOVE_H
#define MOVE_H
// move.h
#pragma once
#include "piece.h"
#include <string>

enum class MoveType {
    NORMAL,
    CAPTURE,
    CASTLE_KINGSIDE,
    CASTLE_QUEENSIDE,
    EN_PASSANT,
    PROMOTION,
    INVALID
};

class Move {
private:
    Position from;
    Position to;
    PieceType promotionType;
    MoveType moveType;

public:
    Move();
    Move(Position from, Position to, MoveType moveType = MoveType::NORMAL, PieceType promotionType = PieceType::NONE);

    Position getFrom() const;
    Position getTo() const;
    MoveType getMoveType() const;
    PieceType getPromotionType() const;

    void setMoveType(MoveType type);
    void setPromotionType(PieceType type);

    std::string toNotation() const;
    bool isValid() const;

    bool operator==(const Move& other) const;
};
#endif //MOVE_H
