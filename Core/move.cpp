#include "move.h"
#include "piece.h"
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

    auto piece = board.getPiece(fromRow, fromCol);
    if (!piece) return false;

    char symbol = piece->getSymbol();

    // === Обработка логики пешки ===
    if (symbol == 'P' || symbol == 'p') {
        int direction = (currentTurn == Color::White) ? 1 : -1;
        int startRow = (currentTurn == Color::White) ? 1 : 6;
        int rowDiff = toRow - fromRow;
        int colDiff = toCol - fromCol;
        auto destPiece = board.getPiece(toRow, toCol);

        // Пешка бьёт по диагонали — проверка есть ли вражеская фигура
        if (std::abs(colDiff) == 1 && rowDiff == direction) {
            if (!destPiece || destPiece->getColor() == currentTurn) {
                return false; // нельзя бить пустое поле или союзника
            }
        }


        // Пешка ходит вперёд — только если клетка пуста
        if (colDiff == 0) {
            if (!board.isEmpty(toRow, toCol)) return false;

            // На 2 клетки — только с начальной позиции и обе клетки пусты
            if (rowDiff == 2 * direction) {
                if (fromRow != startRow || !board.isEmpty(fromRow + direction, fromCol))
                    return false;
            }
        }
    }

    // === Общая проверка для остальных фигур (кроме коня) ===
    if (symbol != 'N' && symbol != 'n' && symbol != 'P' && symbol != 'p') {
        if (!isPathClear(board, fromRow, fromCol, toRow, toCol))
            return false;
    }

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
