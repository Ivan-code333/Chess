//
// Created by Fores on 15.04.2025.
//

#include "game.h"
#include <iostream>
#include "move.h"


Game::Game() {
    reset();
}

void Game::reset() {
    board = Board(); // автоматически вызывает setupBoard()
    currentTurn = Color::White;
    gameOver = false;
    waitingForPromotion = false;
    promotionRow = -1;
    promotionCol = -1;
}

bool Game::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    if (gameOver || waitingForPromotion)
        return false;

    if (MoveHandler::tryMove(board, fromRow, fromCol, toRow, toCol, currentTurn)) {
        // Проверяем, нужен ли выбор фигуры для превращения пешки
        auto piece = board.getPiece(toRow, toCol);
        if (piece && (piece->getSymbol() == 'P' || piece->getSymbol() == 'p') &&
            ((currentTurn == Color::White && toRow == 0) ||
             (currentTurn == Color::Black && toRow == 7))) {
            waitingForPromotion = true;
            promotionRow = toRow;
            promotionCol = toCol;
            return true;
        }

        // Смена хода
        currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;

        // Проверяем шах и мат после хода
        if (MoveHandler::isKingInCheck(board, currentTurn)) {
            if (!MoveHandler::hasLegalMoves(board, currentTurn)) {
                gameOver = true; // Мат
            }
        } else if (!MoveHandler::hasLegalMoves(board, currentTurn)) {
            gameOver = true; // Пат
        }

        return true;
    }

    return false;
}

void Game::promotePawn(char pieceType) {
    if (!waitingForPromotion)
        return;

    std::shared_ptr<Piece> promoted;
    switch (pieceType) {
        case 'Q': promoted = std::make_shared<Queen>(currentTurn); break;
        case 'R': promoted = std::make_shared<Rook>(currentTurn); break;
        case 'B': promoted = std::make_shared<Bishop>(currentTurn); break;
        case 'N': promoted = std::make_shared<Knight>(currentTurn); break;
        default: return;
    }

    board.setPiece(promotionRow, promotionCol, promoted);
    waitingForPromotion = false;
    promotionRow = -1;
    promotionCol = -1;
    
    // Смена хода после превращения
    currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;
}

bool Game::isWaitingForPromotion() const {
    return waitingForPromotion;
}

// Добавляем методы для проверки состояния игры
bool Game::isCheck() const {
    return MoveHandler::isKingInCheck(board, currentTurn);
}

bool Game::isCheckmate() const {
    return isCheck() && !MoveHandler::hasLegalMoves(board, currentTurn);
}

bool Game::isStalemate() const {
    return !isCheck() && !MoveHandler::hasLegalMoves(board, currentTurn);
}

Color Game::getCurrentTurn() const {
    return currentTurn;
}

bool Game::isGameOver() const {
    return gameOver;
}

void Game::endGame() {
    gameOver = true;
}

const Board& Game::getBoard() const {
    return board;
}

