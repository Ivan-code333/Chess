//
// Created by Fores on 09.04.2025.
//

#ifndef BOARD_H
#define BOARD_H
#pragma once
#include <array>
#include <memory>
#include <vector>
#include "piece.h"

class Board {
public:
    Board();

    // Управление доской
    void setupInitialPosition();
    void clear();

    // Работа с фигурами
    Piece* getPiece(int x, int y) const;
    void setPiece(int x, int y, std::unique_ptr<Piece> piece);
    void movePiece(const Position& from, const Position& to);

    // Проверка правил
    bool isMoveLegal(const Position& from, const Position& to, Color player) const;
    bool isInCheck(Color color) const;
    bool isCheckmate(Color color) const;

    // Вспомогательные методы
    bool isEmpty(int x, int y) const;
    bool isEnemy(int x, int y, Color color) const;
    bool isFriend(int x, int y, Color color) const;

    // Отладочный вывод
    void print() const;

private:
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> grid;

    // Проверка особых ходов
    bool isValidCastling(const Position& kingPos, const Position& rookPos, Color color) const;
    bool isValidEnPassant(const Position& pawnPos, const Position& targetPos) const;

    // Внутренние проверки
    bool isSquareAttacked(int x, int y, Color byColor) const;
    Position findKing(Color color) const;
};

#endif //BOARD_H
