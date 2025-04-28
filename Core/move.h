//
#ifndef MOVE_H
#define MOVE_H

#include "board.h"
#include "piece.h"

class MoveHandler {
public:
    static bool isMoveLegal(const Board& board,
        int fromRow, int fromCol, int toRow, int toCol, Color currentTurn);
    static bool tryMove(Board& board,
        int fromRow, int fromCol, int toRow, int toCol, Color currentTurn);
    static bool isKingInCheck(const Board& board, Color kingColor);
    static bool hasLegalMoves(const Board& board, Color turn);


private:
    static bool isPathClear(const Board& board, int fromRow, int fromCol, int toRow, int toCol);
};

#endif // MOVE_H
