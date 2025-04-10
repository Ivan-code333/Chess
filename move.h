#ifndef MOVE_H
#define MOVE_H

#include "board.h"
#include "piece.h"

class MoveHandler {
public:
    // Проверка и выполнение хода
    static bool tryMove(Board& board, int fromRow, int fromCol, int toRow, int toCol, Color currentTurn);

private:
    static bool isPathClear(const Board& board, int fromRow, int fromCol, int toRow, int toCol);
};

#endif // MOVE_H
