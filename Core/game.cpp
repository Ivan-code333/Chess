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
}

bool Game::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    if (gameOver)
        return false;

    if (MoveHandler::tryMove(board, fromRow, fromCol, toRow, toCol, currentTurn)) {
        // Смена хода
        currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;

        // Проверка шаха, мата или пата
        if (MoveHandler::isKingInCheck(board, currentTurn)) {
            if (!MoveHandler::hasLegalMoves(board, currentTurn)) {
                std::cout << "Checkmate! ";
                std::cout << ((currentTurn == Color::White) ? "Black" : "White") << " wins.\n";
                gameOver = true;
            } else {
                std::cout << "Check!\n";
            }
        } else if (!MoveHandler::hasLegalMoves(board, currentTurn)) {
            std::cout << "Stalemate! It's a draw.\n";
            gameOver = true;
        }

        return true;
    }

    return false;
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

