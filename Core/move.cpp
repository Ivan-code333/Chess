#include "move.h"
#include "piece.h"
#include <cmath>

#include <iostream>

bool MoveHandler::isMoveLegal(const Board& board, int fromRow, int fromCol, int toRow, int toCol, Color currentTurn)
{
    // Если король под шахом, проверяем только ходы, которые защищают от шаха
    if (isKingInCheck(board, currentTurn)) {
        // Симулируем ход
        Board temp = board;
        temp.movePiece(fromRow, fromCol, toRow, toCol);
        // Если после хода король все еще под шахом, ход нелегален
        if (isKingInCheck(temp, currentTurn)) {
            return false;
        }
    }

    auto piece = board.getPiece(fromRow, fromCol);
    if (!piece || piece->getColor() != currentTurn)
        return false;

    char symbol = piece->getSymbol();
    int rowDiff = toRow - fromRow;
    int colDiff = toCol - fromCol;

    // рокировка
    if ((symbol == 'K' || symbol == 'k') && rowDiff == 0 && std::abs(colDiff) == 2)
    {
        bool kingSide = (colDiff > 0);
        // Проверка прав на рокировку
        if (kingSide)
        {
            if (!board.canCastleKingSide(currentTurn))
                return false;
        }
        else
        {
            if (!board.canCastleQueenSide(currentTurn))
                return false;
        }
        // Проверить, что король не под шахом и не проходит через атакуемые поля
        for (int offset = 1; offset <= 2; ++offset)
        {
            Board temp = board;
            temp.movePiece(fromRow, fromCol,
                fromRow,
                fromCol + (kingSide ? offset : -offset));
            if (isKingInCheck(temp, currentTurn))
                return false;
        }
        return true;
    }

    // Проверить базовую легальность хода фигуры
    if (!piece->isMoveLegal(fromRow, fromCol, toRow, toCol))
        return false;

    auto target = board.getPiece(toRow, toCol);
    if (target && target->getColor() == currentTurn)
        return false;

    // Логика пешки:
    if (symbol == 'P' || symbol == 'p')
    {
        int direction = (symbol == 'P') ? -1 : 1;
        int absCol = std::abs(colDiff);

        // диагональное взятие (обычное или на проходе)
        if (absCol == 1 && rowDiff == direction)
        {
            // обычное взятие
            if (target && target->getColor() != currentTurn)
                return true;
            // взятие на проходе: клетка прицела пуста, но соседняя пешка сделала ход в 2 клетки
            const LastMove& lm = board.getLastMove();
            if (!target && lm.toRow == fromRow && lm.toCol == toCol
                && std::abs(lm.fromRow - lm.toRow) == 2)
            {
                return true;
            }
            return false;
        }

        // прямой ход
        if (colDiff == 0)
        {
            if (!board.isEmpty(toRow, toCol))
                return false;
            if (std::abs(rowDiff) == 2)
            {
                int startRow = (symbol == 'P') ? 6 : 1;
                if (fromRow != startRow || !board.isEmpty(fromRow + direction, fromCol))
                    return false;
            }
            else if (std::abs(rowDiff) != 1)
            {
                return false;
            }
        }
    }

    // Проверка, что путь свободен (для всех фигур, кроме коня и пешки)
    if (symbol != 'N' && symbol != 'n' && symbol != 'P' && symbol != 'p') {
        if (!isPathClear(board, fromRow, fromCol, toRow, toCol))
            return false;
    }

    // Симулируем ход и проверяем, не остаётся ли король под шахом
    Board temp = board;

    if ((symbol == 'P' || symbol == 'p') && std::abs(colDiff) == 1 && board.isEmpty(toRow, toCol))
    {
        const LastMove& lm = board.getLastMove();
        temp.setPiece(lm.toRow, lm.toCol, nullptr);
    }
    temp.movePiece(fromRow, fromCol, toRow, toCol);
    if (isKingInCheck(temp, currentTurn))
        return false;

    return true;
}

bool MoveHandler::tryMove(Board& board, int fromRow, int fromCol,
    int toRow, int toCol, Color currentTurn) {
    if (!isMoveLegal(board, fromRow, fromCol, toRow, toCol, currentTurn))
        return false;

    auto piece = board.getPiece(fromRow, fromCol);
    char symbol = piece->getSymbol();

    if ((symbol == 'P' || symbol == 'p') && std::abs(toCol - fromCol) == 1 && board.isEmpty(toRow, toCol)) {
        const LastMove& lm = board.getLastMove();
        if (lm.toRow == fromRow && lm.toCol == toCol && std::abs(lm.fromRow - lm.toRow) == 2)
            board.setPiece(lm.toRow, lm.toCol, nullptr);
    }

    // Стандартный ход (включая обычное взятие)
    board.movePiece(fromRow, fromCol, toRow, toCol);

    // Превращение пешки с выбором фигуры 
    if (symbol == 'P' || symbol == 'p') {
        if ((currentTurn == Color::White && toRow == 0) ||
            (currentTurn == Color::Black && toRow == 7)) {
            // Возвращаем true, чтобы игра знала, что нужно показать окно выбора фигуры
            return true;
        }
    }

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

bool MoveHandler::isKingInCheck(const Board& board, Color kingColor) {
    int kingRow = -1, kingCol = -1;

    // Найти короля
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            auto piece = board.getPiece(row, col);
            if (piece && piece->getColor() == kingColor) {
                char symbol = piece->getSymbol();
                if ((kingColor == Color::White && symbol == 'K') ||
                    (kingColor == Color::Black && symbol == 'k')) {
                    kingRow = row;
                    kingCol = col;
                    break;
                }
            }
        }
        if (kingRow != -1) break;
    }

    if (kingRow == -1 || kingCol == -1)
        return false; // Король не найден

    // Проверить, атакует ли кто-то короля
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            auto piece = board.getPiece(row, col);
            if (piece && piece->getColor() != kingColor) {
                // Специальная проверка для пешек
                if (piece->getSymbol() == 'P' || piece->getSymbol() == 'p') {
                    // Пешки атакуют по диагонали
                    int direction = (piece->getColor() == Color::White) ? -1 : 1;
                    if (row + direction == kingRow && 
                        (col + 1 == kingCol || col - 1 == kingCol)) {
                        return true;
                    }
                    continue; // Пропускаем обычную проверку для пешек
                }
                
                // Для остальных фигур
                if (piece->isMoveLegal(row, col, kingRow, kingCol)) {
                    // Проверяем, что путь до короля свободен
                    if (piece->getSymbol() == 'N' || piece->getSymbol() == 'n' ||
                        isPathClear(board, row, col, kingRow, kingCol)) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool MoveHandler::hasLegalMoves(const Board& board, Color turn) {
    for (int fromRow = 0; fromRow < BOARD_SIZE; ++fromRow) {
        for (int fromCol = 0; fromCol < BOARD_SIZE; ++fromCol) {
            auto piece = board.getPiece(fromRow, fromCol);
            if (!piece || piece->getColor() != turn)
                continue;

            for (int toRow = 0; toRow < BOARD_SIZE; ++toRow) {
                for (int toCol = 0; toCol < BOARD_SIZE; ++toCol) {
                    if (fromRow == toRow && fromCol == toCol)
                        continue;

                    if (isMoveLegal(board, fromRow, fromCol, toRow, toCol, turn)) {
                        // Проверяем, не оставляет ли этот ход короля под шахом
                        Board tempBoard = board;
                        tempBoard.movePiece(fromRow, fromCol, toRow, toCol);
                        if (!isKingInCheck(tempBoard, turn)) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}
