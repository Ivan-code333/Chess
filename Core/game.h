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

    // Новые методы для проверки состояния игры
    bool isCheck() const;
    bool isCheckmate() const;
    bool isStalemate() const;

    // Методы для превращения пешки
    bool isWaitingForPromotion() const;
    void promotePawn(char pieceType);

private:
    Board board;
    Color currentTurn;
    bool gameOver;
    bool waitingForPromotion;
    int promotionRow;
    int promotionCol;
};

#endif // GAME_H
