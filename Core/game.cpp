//
// Created by Fores on 15.04.2025.
//

#include "game.h"


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
        // Смена игрока
        currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;
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

