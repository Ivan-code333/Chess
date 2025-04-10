//
// Created by Fores on 09.04.2025.
//

#ifndef BOARD_H
#define BOARD_H
// board.h
#pragma once
#include "piece.h"
#include "move.h"
#include <vector>
#include <string>

class Board {
private:
    Piece pieces[8][8];
    std::vector<Move> moveHistory;

    // Состояние для взятия на проходе
    Position enPassantTarget;

    // Для определения возможности рокировки
    bool whiteKingMoved;
    bool blackKingMoved;
    bool whiteKingsideRookMoved;
    bool whiteQueensideRookMoved;
    bool blackKingsideRookMoved;
    bool blackQueensideRookMoved;

public:
    Board();

    // Инициализация доски
    void setupNewGame();
    void clear();

    // Доступ к фигурам
    Piece getPiece(const Position& pos) const;
    void setPiece(const Position& pos, const Piece& piece);
    void removePiece(const Position& pos);

    // Перемещение фигур
    bool makeMove(const Move& move);
    void undoLastMove();

    // Проверка ходов
    bool isValidMove(const Move& move, Color currentPlayer) const;
    std::vector<Move> getAllLegalMoves(Color currentPlayer) const;

    // Специальные проверки
    bool isCheck(Color color) const;
    bool isCheckmate(Color color) const;
    bool isStalemate(Color color) const;

    // Проверка окончания игры
    bool isGameOver() const;

    // Генерация нотации FEN
    std::string toFEN() const;

    // Получение последнего хода
    Move getLastMove() const;

    // Получение истории ходов
    const std::vector<Move>& getMoveHistory() const;

    // Поиск короля
    Position findKing(Color color) const;

    // Генерация всех ходов для фигуры
    std::vector<Move> generatePieceMoves(const Position& pos) const;

private:
    // Вспомогательные методы
    bool isUnderAttack(const Position& pos, Color attackerColor) const;
    bool moveResultsInCheck(const Move& move, Color currentPlayer) const;

    // Генераторы ходов для конкретных типов фигур
    std::vector<Move> generatePawnMoves(const Position& pos) const;
    std::vector<Move> generateKnightMoves(const Position& pos) const;
    std::vector<Move> generateBishopMoves(const Position& pos) const;
    std::vector<Move> generateRookMoves(const Position& pos) const;
    std::vector<Move> generateQueenMoves(const Position& pos) const;
    std::vector<Move> generateKingMoves(const Position& pos) const;
};
#endif //BOARD_H
