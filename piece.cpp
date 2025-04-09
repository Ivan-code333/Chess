//
// Created by Fores on 09.04.2025.
//

#include "piece.h"
#include "board.h"
#include <algorithm>

// ===== King =====
std::vector<Position> King::getPossibleMoves(const Position& from, const Board& board) const {
    std::vector<Position> moves;

    // Все 8 направлений вокруг короля
    const std::vector<Position> directions = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };

    for (const auto& dir : directions) {
        Position to(from.x + dir.x, from.y + dir.y);
        if (!to.isValid()) continue;

        Piece* target = board.getPiece(to.x, to.y);
        if (!target || target->getColor() != color) {
            moves.push_back(to);
        }
    }

    // TODO: Добавить рокировку
    return moves;
}

// ===== Queen =====
std::vector<Position> Queen::getPossibleMoves(const Position& from, const Board& board) const {
    std::vector<Position> moves;

    // Комбинация направлений ладьи и слона
    const std::vector<Position> directions = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };

    for (const auto& dir : directions) {
        for (int step = 1; step < 8; ++step) {
            Position to(from.x + dir.x * step, from.y + dir.y * step);
            if (!to.isValid()) break;

            Piece* target = board.getPiece(to.x, to.y);
            if (!target) {
                moves.push_back(to);
            } else {
                if (target->getColor() != color) {
                    moves.push_back(to);
                }
                break;
            }
        }
    }

    return moves;
}

// ===== Rook =====
std::vector<Position> Rook::getPossibleMoves(const Position& from, const Board& board) const {
    std::vector<Position> moves;

    const std::vector<Position> directions = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

    for (const auto& dir : directions) {
        for (int step = 1; step < 8; ++step) {
            Position to(from.x + dir.x * step, from.y + dir.y * step);
            if (!to.isValid()) break;

            Piece* target = board.getPiece(to.x, to.y);
            if (!target) {
                moves.push_back(to);
            } else {
                if (target->getColor() != color) {
                    moves.push_back(to);
                }
                break;
            }
        }
    }

    return moves;
}

// ===== Bishop =====
std::vector<Position> Bishop::getPossibleMoves(const Position& from, const Board& board) const {
    std::vector<Position> moves;

    const std::vector<Position> directions = {
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    for (const auto& dir : directions) {
        for (int step = 1; step < 8; ++step) {
            Position to(from.x + dir.x * step, from.y + dir.y * step);
            if (!to.isValid()) break;

            Piece* target = board.getPiece(to.x, to.y);
            if (!target) {
                moves.push_back(to);
            } else {
                if (target->getColor() != color) {
                    moves.push_back(to);
                }
                break;
            }
        }
    }

    return moves;
}

// ===== Knight =====
std::vector<Position> Knight::getPossibleMoves(const Position& from, const Board& board) const {
    std::vector<Position> moves;

    const std::vector<Position> offsets = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2},  {1, 2},  {2, -1},  {2, 1}
    };

    for (const auto& offset : offsets) {
        Position to(from.x + offset.x, from.y + offset.y);
        if (!to.isValid()) continue;

        Piece* target = board.getPiece(to.x, to.y);
        if (!target || target->getColor() != color) {
            moves.push_back(to);
        }
    }

    return moves;
}

// ===== Pawn =====
std::vector<Position> Pawn::getPossibleMoves(const Position& from, const Board& board) const {
    std::vector<Position> moves;

    int direction = (color == Color::WHITE) ? 1 : -1;

    // Ход на 1 клетку вперед
    Position oneStep(from.x, from.y + direction);
    if (oneStep.isValid() && !board.getPiece(oneStep.x, oneStep.y)) {
        moves.push_back(oneStep);

        // Ход на 2 клетки из начальной позиции
        if ((color == Color::WHITE && from.y == 1) || (color == Color::BLACK && from.y == 6)) {
            Position twoSteps(from.x, from.y + 2 * direction);
            if (!board.getPiece(twoSteps.x, twoSteps.y)) {
                moves.push_back(twoSteps);
            }
        }
    }

    // Взятие по диагонали
    for (int dx : {-1, 1}) {
        Position capturePos(from.x + dx, from.y + direction);
        if (!capturePos.isValid()) continue;

        Piece* target = board.getPiece(capturePos.x, capturePos.y);
        if (target && target->getColor() != color) {
            moves.push_back(capturePos);
        }
        // TODO: Добавить взятие на проходе
    }

    return moves;
}
