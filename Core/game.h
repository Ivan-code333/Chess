#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "move.h"

class Game {
public:
    Game();

    void reset(); // Начать новую игру
    bool makeMove(int fromRow, int fromCol, int toRow, int toCol); // Попытка сделать ход
    Color getCurrentTurn() const;
    bool isGameOver() const;
    void endGame(); // Завершить игру вручную
    const Board& getBoard() const;

private:
    Board board;
    Color currentTurn;
    bool gameOver;
};

#endif // GAME_H
