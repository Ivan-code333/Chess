//
// Created by Fores on 09.04.2025.
//


#include "board.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>

Board::Board() {
    clear();
}

void Board::clear() {
    for (auto& row : grid) {
        for (auto& cell : row) {
            cell.reset();
        }
    }
}

void Board::setupInitialPosition() {
    clear();

    // Белые фигуры
    setPiece(0, 0, std::make_unique<Rook>(Color::WHITE));
    setPiece(1, 0, std::make_unique<Knight>(Color::WHITE));
    setPiece(2, 0, std::make_unique<Bishop>(Color::WHITE));
    setPiece(3, 0, std::make_unique<Queen>(Color::WHITE));
    setPiece(4, 0, std::make_unique<King>(Color::WHITE));
    setPiece(5, 0, std::make_unique<Bishop>(Color::WHITE));
    setPiece(6, 0, std::make_unique<Knight>(Color::WHITE));
    setPiece(7, 0, std::make_unique<Rook>(Color::WHITE));

    for (int x = 0; x < 8; ++x) {
        setPiece(x, 1, std::make_unique<Pawn>(Color::WHITE));
    }

    // Черные фигуры
    setPiece(0, 7, std::make_unique<Rook>(Color::BLACK));
    setPiece(1, 7, std::make_unique<Knight>(Color::BLACK));
    setPiece(2, 7, std::make_unique<Bishop>(Color::BLACK));
    setPiece(3, 7, std::make_unique<Queen>(Color::BLACK));
    setPiece(4, 7, std::make_unique<King>(Color::BLACK));
    setPiece(5, 7, std::make_unique<Bishop>(Color::BLACK));
    setPiece(6, 7, std::make_unique<Knight>(Color::BLACK));
    setPiece(7, 7, std::make_unique<Rook>(Color::BLACK));

    for (int x = 0; x < 8; ++x) {
        setPiece(x, 6, std::make_unique<Pawn>(Color::BLACK));
    }
}

Piece* Board::getPiece(int x, int y) const {
    if (x < 0 || x >= 8 || y < 0 || y >= 8) return nullptr;
    return grid[x][y].get();
}

void Board::setPiece(int x, int y, std::unique_ptr<Piece> piece) {
    if (x < 0 || x >= 8 || y < 0 || y >= 8) {
        throw std::out_of_range("Invalid board position");
    }
    grid[x][y] = std::move(piece);
}

void Board::movePiece(const Position& from, const Position& to) {
    if (!from.isValid() || !to.isValid()) {
        throw std::invalid_argument("Invalid position");
    }

    grid[to.x][to.y] = std::move(grid[from.x][from.y]);
}

bool Board::isMoveLegal(const Position& from, const Position& to, Color player) const {
    // 1. Проверка базовых условий
    if (!from.isValid() || !to.isValid()) return false;
    if (from == to) return false;

    Piece* piece = getPiece(from.x, from.y);
    if (!piece || piece->getColor() != player) return false;

    // 2. Получаем возможные ходы фигуры (без учета шаха)
    std::vector<Position> possibleMoves = piece->getPossibleMoves(from, *this);
    if (std::find(possibleMoves.begin(), possibleMoves.end(), to) == possibleMoves.end()) {
        return false;
    }

    // 3. Проверка особых правил (рокировка, взятие на проходе)
    if (piece->getType() == PieceType::KING && abs(from.x - to.x) == 2) {
        return isValidCastling(from, to, player);
    }

    // 4. Проверка, не оставляет ли ход короля под шахом
    Board testBoard = *this;
    testBoard.movePiece(from, to);
    return !testBoard.isInCheck(player);
}

bool Board::isInCheck(Color color) const {
    Position kingPos = findKing(color);
    if (!kingPos.isValid()) return false;

    return isSquareAttacked(kingPos.x, kingPos.y, (color == Color::WHITE) ? Color::BLACK : Color::WHITE);
}

bool Board::isCheckmate(Color color) const {
    if (!isInCheck(color)) return false;

    // Проверяем все возможные ходы цвета
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            Piece* piece = getPiece(x, y);
            if (!piece || piece->getColor() != color) continue;

            Position from(x, y);
            std::vector<Position> moves = piece->getPossibleMoves(from, *this);
            for (const Position& to : moves) {
                Board testBoard = *this;
                testBoard.movePiece(from, to);
                if (!testBoard.isInCheck(color)) {
                    return false; // Найден хотя бы один легальный ход
                }
            }
        }
    }

    return true; // Нет легальных ходов
}

bool Board::isEmpty(int x, int y) const {
    return getPiece(x, y) == nullptr;
}

bool Board::isEnemy(int x, int y, Color color) const {
    Piece* piece = getPiece(x, y);
    return piece && piece->getColor() != color;
}

bool Board::isFriend(int x, int y, Color color) const {
    Piece* piece = getPiece(x, y);
    return piece && piece->getColor() == color;
}

void Board::print() const {
    const std::string pieces = "KQRBNPkqrbnp.";

    for (int y = 7; y >= 0; --y) {
        std::cout << (y + 1) << " ";
        for (int x = 0; x < 8; ++x) {
            char c = '.';
            Piece* piece = getPiece(x, y);
            if (piece) {
                c = piece->getSymbol();
            }
            std::cout << c << " ";
        }
        std::cout << "\n";
    }
    std::cout << "  a b c d e f g h\n";
}

// Приватные методы
bool Board::isValidCastling(const Position& kingPos, const Position& rookPos, Color color) const {
    // TODO: Реализовать проверку рокировки
    return false;
}

bool Board::isValidEnPassant(const Position& pawnPos, const Position& targetPos) const {
    // TODO: Реализовать взятие на проходе
    return false;
}

bool Board::isSquareAttacked(int x, int y, Color byColor) const {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* piece = getPiece(i, j);
            if (!piece || piece->getColor() != byColor) continue;

            Position attackerPos(i, j);
            std::vector<Position> moves = piece->getPossibleMoves(attackerPos, *this);
            if (std::find(moves.begin(), moves.end(), Position(x, y)) != moves.end()) {
                return true;
            }
        }
    }
    return false;
}

Position Board::findKing(Color color) const {
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            Piece* piece = getPiece(x, y);
            if (piece && piece->getType() == PieceType::KING && piece->getColor() == color) {
                return Position(x, y);
            }
        }
    }
    return Position(-1, -1); // Король не найден (невозможно при корректной доске)
}