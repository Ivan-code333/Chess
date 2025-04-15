#include "move.h"
#include <cmath>

bool MoveHandler::isMoveLegal(const Board& board, int fromRow, int fromCol, int toRow, int toCol, Color currentTurn) {
    auto piece = board.getPiece(fromRow, fromCol);
    if (!piece || piece->getColor() != currentTurn)
        return false;

    if (!piece->isMoveLegal(fromRow, fromCol, toRow, toCol))
        return false;

    auto target = board.getPiece(toRow, toCol);
    if (target && target->getColor() == currentTurn)
        return false;

    return true;
}

bool MoveHandler::tryMove(Board& board, int fromRow, int fromCol, int toRow, int toCol, Color currentTurn) {
    if (!isMoveLegal(board, fromRow, fromCol, toRow, toCol, currentTurn))
        return false;

    board.movePiece(fromRow, fromCol, toRow, toCol);
    return true;
}

// Проверка, что путь от from до to чист (по горизонтали, вертикали или диагонали)
bool MoveHandler::isPathClear(const Board& board, int fromRow, int fromCol, int toRow, int toCol) {
    int dRow = (toRow - fromRow) == 0 ? 0 : (toRow - fromRow) / std::abs(toRow - fromRow);
    int dCol = (toCol - fromCol) == 0 ? 0 : (toCol - fromCol) / std::abs(toCol - fromCol);

    int r = fromRow + dRow;
    int c = fromCol + dCol;
    while (r != toRow || c != toCol) {
        if (board.getPiece(r, c))
            return false;
        r += dRow;
        c += dCol;
    }
    return true;
}
